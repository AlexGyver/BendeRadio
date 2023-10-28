#include "core0.h"

#include <EEManager.h>
#include <EncButton.h>
#include <FastLED.h>
#include <GyverMAX7219.h>
#include <VolAnalyzer.h>

#include "soc/timer_group_reg.h"
#include "soc/timer_group_struct.h"
#include "tmr.h"

const char* stations[] = {
    "https://uk3.internet-radio.com/proxy/majesticjukebox?mp=/live",
    "http://prmstrm.1.fm:8000/electronica",
    "http://prmstrm.1.fm:8000/x",
    "http://stream81.metacast.eu/radio1rock128",
};

// data
MAX7219<5, 1, MTRX_CS, MTRX_DAT, MTRX_CLK> mtrx;
Tmr square_tmr;
Data data;
EEManager memory(data);
Audio audio;
String streamname;
const char* reconnect = nullptr;

// func
// ========================= MATRIX =========================
void upd_bright() {
    uint8_t m = data.bright_mouth, e = data.bright_eyes;
    uint8_t br[] = {m, m, m, e, e};
    mtrx.setBright(br);
}
void print_val(char c, uint8_t v) {
    mtrx.rect(0, 0, ANALYZ_WIDTH - 1, 7, GFX_CLEAR);
    mtrx.setCursor(8 * 0 + 2, 1);
    mtrx.print(c);
    mtrx.setCursor(8 * 1 + 2, 1);
    mtrx.print(v / 10);
    mtrx.setCursor(8 * 2 + 2, 1);
    mtrx.print(v % 10);
    mtrx.update();
}
// ========================= EYES =========================
void draw_eye(uint8_t i) {
    uint8_t x = ANALYZ_WIDTH + i * 8;
    mtrx.rect(1 + x, 1, 6 + x, 6, GFX_FILL);
    mtrx.lineV(0 + x, 2, 5);
    mtrx.lineV(7 + x, 2, 5);
    mtrx.lineH(0, 2 + x, 5 + x);
    mtrx.lineH(7, 2 + x, 5 + x);
}
void draw_eyeb(uint8_t i, int x, int y, int w = 2) {
    x += ANALYZ_WIDTH + i * 8;
    mtrx.rect(x, y, x + w - 1, y + w - 1, GFX_CLEAR);
}
void anim_search() {
    static int8_t pos = 4, dir = 1;
    static Tmr tmr(50);
    if (tmr) {
        pos += dir;
        if (pos >= 6) dir = -1;
        if (pos <= 0) dir = 1;
        mtrx.rect(ANALYZ_WIDTH, 2, ANALYZ_WIDTH + 16 - 1, 5, GFX_FILL);
        draw_eyeb(0, pos, 3);
        draw_eyeb(1, pos, 3);
        mtrx.update();
    }
}
void change_state() {
    mtrx.clear();
    if (data.state) {
        upd_bright();
        square_tmr.start(600);
        draw_eye(0);
        draw_eye(1);
        draw_eyeb(0, 2, 2, 4);
        draw_eyeb(1, 2, 2, 4);
    } else {
        mtrx.setBright((uint8_t)0);
        draw_eye(0);
        draw_eye(1);
        mtrx.rect(ANALYZ_WIDTH, 0, ANALYZ_WIDTH + 16 - 1, 3, GFX_CLEAR);
        draw_eyeb(0, 3, 5);
        draw_eyeb(1, 3, 5);
    }
    mtrx.update();
}

// ========================= ANALYZ =========================
void analyz0(uint8_t vol) {
    static uint16_t offs;
    offs += 20 * vol / 100;
    for (uint8_t i = 0; i < ANALYZ_WIDTH; i++) {
        int16_t val = inoise8(i * 50, offs);
        val -= 128;
        val = val * vol / 100;
        val += 128;
        val = map(val, 45, 255 - 45, 0, 7);
        mtrx.dot(i, val);
    }
}
void analyz1(uint8_t vol) {
    static uint8_t prevs[ANALYZ_WIDTH];
    for (uint8_t i = 0; i < ANALYZ_WIDTH - 1; i++) prevs[i] = prevs[i + 1];
    prevs[ANALYZ_WIDTH - 1] = 9 * vol / (100 + 1);
    for (uint8_t i = 0; i < ANALYZ_WIDTH; i++) {
        uint8_t mask = ((1 << prevs[i]) - 1) << ((8 - prevs[i]) >> 1);
        // 0-00000000-4
        // 1-00001000-3
        // 2-00011000-3
        // 3-00011100-2
        // 4-00111100-2
        // 5-00111110-1
        // 6-01111110-1
        // 7-01111111-0
        // 8-11111111-0
        for (uint8_t n = 0; n < 8; n++) {
            if (mask & 1) mtrx.dot(i, n);
            mask >>= 1;
        }
    }
}

// ========================= SYSTEM =========================
void audio_showstreamtitle(const char* info) {
}

void core0(void* p) {
    // ========================= SETUP =========================
    EncButton eb(ENC_S1, ENC_S2, ENC_BTN);
    VolAnalyzer sound(ANALYZ_PIN);
    sound.setAmpliDt(300);
    sound.setTrsh(data.trsh);
    sound.setPulseMin(40);
    sound.setPulseMax(80);

    Tmr eye_tmr(150);
    Tmr matrix_tmr(1000);
    Tmr angry_tmr(800);
    square_tmr.timerMode(1);
    matrix_tmr.timerMode(1);
    angry_tmr.timerMode(1);
    bool pulse = 0;

    EEPROM.begin(memory.blockSize());
    memory.begin(0, 'b');

    mtrx.begin();
    upd_bright();
    mtrx.clear();
    mtrx.update();

    audio.setBufsize(RADIO_BUFFER, -1);
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(data.state ? data.vol : 0);
    data.station = constrain(data.station, 0, sizeof(stations) / sizeof(char*) - 1);
    reconnect = stations[data.station];

    // ========================= LOOP =========================
    for (;;) {
        square_tmr.tick();
        matrix_tmr.tick();
        angry_tmr.tick();
        memory.tick();

        if (data.state && !square_tmr.state()) {
            if (eye_tmr) {
                draw_eye(0);
                draw_eye(1);
                if (angry_tmr.state()) {
                    draw_eyeb(0, 3, 3);
                    draw_eyeb(1, 3, 3);
                    mtrx.lineH(0, ANALYZ_WIDTH, ANALYZ_WIDTH + 16 - 1, GFX_CLEAR);
                    mtrx.lineH(1, ANALYZ_WIDTH + 5, ANALYZ_WIDTH + 5 + 6 - 1, GFX_CLEAR);
                    mtrx.lineH(2, ANALYZ_WIDTH + 6, ANALYZ_WIDTH + 6 + 4 - 1, GFX_CLEAR);
                    mtrx.lineH(3, ANALYZ_WIDTH + 7, ANALYZ_WIDTH + 7 + 2 - 1, GFX_CLEAR);
                } else {
                    if (eb.pressing()) {
                        draw_eyeb(0, 4, 3, 3);
                        draw_eyeb(1, 1, 3, 3);
                    } else {
                        static uint16_t pos;
                        pos += 15;
                        uint8_t x = inoise8(pos);
                        uint8_t y = inoise8(pos + UINT16_MAX / 4);
                        x = constrain(x, 40, 255 - 40);
                        y = constrain(y, 40, 255 - 40);
                        x = map(x, 40, 255 - 40, 2, 5);
                        y = map(y, 40, 255 - 40, 2, 5);
                        if (pulse) {
                            pulse = 0;
                            int8_t sx = random(-1, 1);
                            int8_t sy = random(-1, 1);
                            draw_eyeb(0, x + sx, y + sy, 3);
                            draw_eyeb(1, x + sx, y + sy, 3);
                        } else {
                            draw_eyeb(0, x, y);
                            draw_eyeb(1, x, y);
                        }
                    }
                }
                mtrx.update();
            }
        }

        if (sound.tick() && data.state && !matrix_tmr.state()) {
            if (sound.pulse()) pulse = 1;
            // Serial.print(sound.getVol());  // громкость 0-100
            // Serial.print(',');
            // Serial.print(sound.getRaw());  // сырая величина
            // Serial.print(',');
            // Serial.print(sound.getTrsh());
            // Serial.print(',');
            // Serial.println(sound.getMax());  // амплитудная огибающая

            mtrx.rect(0, 0, ANALYZ_WIDTH - 1, 7, GFX_CLEAR);
            switch (data.mode) {
                case 0:
                    analyz0(sound.getVol());
                    break;
                case 1:
                    analyz1(sound.getVol());
                    break;
            }
            mtrx.update();
        }

        if (eb.tick()) {
            static bool station_changed = 0;

            if (eb.turn()) {
                if (eb.pressing()) {
                    switch (eb.getClicks()) {
                        case 0:
                            data.station += eb.dir();
                            data.station = constrain(data.station, 0, sizeof(stations) / sizeof(char*) - 1);
                            print_val('s', data.station);
                            matrix_tmr.start();
                            station_changed = 1;
                            break;
                        case 1:
                            data.bright_mouth += eb.dir();
                            data.bright_mouth = constrain(data.bright_mouth, 0, 16);
                            upd_bright();
                            break;
                        case 2:
                            data.bright_eyes += eb.dir();
                            data.bright_eyes = constrain(data.bright_eyes, 0, 16);
                            upd_bright();
                            break;
                    }
                } else {
                    if (data.state) {
                        angry_tmr.start();
                        data.vol += eb.dir();
                        data.vol = constrain(data.vol, 0, 21);
                        audio.setVolume(data.vol);
                        print_val('v', data.vol);
                        matrix_tmr.start();
                    }
                }
            }

            if (eb.hasClicks()) {
                switch (eb.getClicks()) {
                    case 1:
                        data.state = !data.state;
                        audio.setVolume(data.state ? data.vol : 0);
                        change_state();
                        break;
                    case 2:
                        if (++data.mode >= 2) data.mode = 0;
                        break;
                    case 3:
                        data.trsh = sound.getMax() * 2 / 3;
                        sound.setTrsh(data.trsh);
                        break;
                }
            }

            if (eb.release()) {
                if (station_changed) {
                    station_changed = 0;
                    reconnect = stations[data.station];
                    if (audio.isRunning()) audio.pauseResume();
                }
            }
            memory.update();
        }

        // vTaskDelay(1);
        TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;  // write enable
        TIMERG0.wdt_feed = 1;                        // feed dog
        TIMERG0.wdt_wprotect = 0;                    // write protect
    }
}
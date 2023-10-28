/*
    Cамая резкая библиотека для матриц MAX7219 на диком западе
    Документация:
    GitHub: https://github.com/GyverLibs/GyverMAX7219
    Возможности:
    - Наследует графику из GyverGFX
    - Подключение матриц зигзагом
    - Аппаратный и программный SPI
    - Невероятная оптимизация

    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
    v1.1 - оптимизирован SPI
    v1.2 - переделан FastIO
    v1.2.1 - исправлен баг в SPI (с 1.2)
    v1.2.2 - убран FastIO
    v1.3 - мелкие доработки и оптимизация, добавил поворот матриц
    v1.4 - добавил поддержку матричных дисплеев любой конфигурации (точка подключения, направление, чередование)
    v1.5 - добавил раздельное управление яркостью и питанием матриц
*/

#ifndef _GyverMAX7219_h
#define _GyverMAX7219_h

#include <Arduino.h>
#include <GyverGFX.h>
#include <SPI.h>

#define GM_ZIGZAG 0
#define GM_SERIES 1

#define GM_LEFT_TOP_RIGHT 0
#define GM_RIGHT_TOP_LEFT 1
#define GM_RIGHT_BOTTOM_LEFT 2
#define GM_LEFT_BOTTOM_RIGHT 3

#define GM_LEFT_TOP_DOWN 4
#define GM_RIGHT_TOP_DOWN 5
#define GM_RIGHT_BOTTOM_UP 6
#define GM_LEFT_BOTTOM_UP 7

#ifndef MAX_SPI_SPEED
#define MAX_SPI_SPEED 1000000
#endif

static SPISettings MAX_SPI_SETT(MAX_SPI_SPEED, MSBFIRST, SPI_MODE0);

template <uint8_t WIDTH, uint8_t HEIGHT, uint8_t CSpin, uint8_t DATpin = 0, uint8_t CLKpin = 0>
class MAX7219 : public GyverGFX {
   public:
    MAX7219() : GyverGFX(WIDTH * 8, HEIGHT * 8) {
        begin();
    }

    // запустить
    void begin() {
        pinMode(CSpin, OUTPUT);
        if (DATpin == CLKpin) {
            SPI.begin();
        } else {
            pinMode(DATpin, OUTPUT);
            pinMode(CLKpin, OUTPUT);
        }
        sendCMD(0x0f, 0x00);  // отключить режим теста
        sendCMD(0x09, 0x00);  // выключить декодирование
        sendCMD(0x0a, 0x00);  // яркость
        sendCMD(0x0b, 0x0f);  // отображаем всё
        sendCMD(0x0C, 0x01);  // включить
        clearDisplay();       // очистить
    }

    // установить яркость [0-15]
    void setBright(int value) {
        sendCMD(0x0a, value);  // 8x8: 0/8/15 - 30/310/540 ma
    }
    void setBright(uint8_t* values) {
        sendCMDs(0x0a, values);
    }

    // переключить питание
    void setPower(bool value) {
        sendCMD(0x0c, value);
    }
    void setPower(bool* values) {
        sendCMDs(0x0c, (uint8_t*)values);
    }

    // очистить
    void clear() {
        fillByte(0);
    }

    // залить
    void fill() {
        fillByte(255);
    }

    // залить байтом
    void fillByte(uint8_t data) {
        for (uint16_t i = 0; i < WIDTH * HEIGHT * 8; i++) buffer[i] = data;
    }

    // установить точку
    void dot(int x, int y, uint8_t fill = 1) {
        int16_t pos = getPosition(x, y);
        if (pos >= 0) bitWrite(buffer[pos], _bx, fill);
    }

    // получить точку
    bool get(int16_t x, int16_t y) {
        int16_t pos = getPosition(x, y);
        if (pos >= 0) return bitRead(buffer[pos], _bx);
        else return 0;
    }

    // обновить
    void update() {
        uint16_t count = 0;
        for (uint8_t k = 0; k < 8; k++) {
            beginData();
            for (uint16_t i = 0; i < _amount; i++) sendData(8 - k, buffer[count++]);
            endData();
        }
    }

    // начать отправку
    void beginData() {
        if (DATpin == CLKpin) SPI.beginTransaction(MAX_SPI_SETT);
        fastWrite(CSpin, 0);
    }

    // закончить отправку
    void endData() {
        fastWrite(CSpin, 1);
        if (DATpin == CLKpin) SPI.endTransaction();
    }

    // отправка данных напрямую в матрицу (строка, байт)
    void sendByte(uint8_t address, uint8_t value) {
        beginData();
        sendData(address + 1, value);
        endData();
    }

    // очистить дисплей (не буфер)
    void clearDisplay() {
        for (uint8_t k = 0; k < 8; k++) {
            beginData();
            for (uint16_t i = 0; i < _amount; i++) sendData(8 - k, 0);
            endData();
        }
    }

    // поворот матриц (8x8): 0, 1, 2, 3 на 90 град по часовой стрелке
    void setRotation(uint8_t rot) {
        _rot = rot;
    }

    // зеркальное отражение матриц (8x8) по x и y
    void setFlip(bool x, bool y) {
        _flip = x | (y << 1);
    }

    // тип дисплея: построчный последовательный (GM_SERIES) или зигзаг GM_ZIGZAG
    void setType(bool type) {
        _type = type;
    }

    // ориентация (точка подключения дисплея)
    void setConnection(uint8_t conn) {
        _conn = conn;
        if (_conn <= 3) size(_maxX, _maxY);
        else size(_maxY, _maxX);
    }

    uint8_t buffer[WIDTH * HEIGHT * 8];

   private:
    int16_t getPosition(int16_t x, int16_t y) {
        switch (_conn) {
            // case GM_LEFT_TOP_RIGHT: break;
            case GM_RIGHT_TOP_LEFT:
                flipX(x);
                flip(y);
                break;
            case GM_RIGHT_BOTTOM_LEFT:
                flipY(y);
                flipX(x);
                break;
            case GM_LEFT_BOTTOM_RIGHT:
                flipY(y);
                flip(y);
                break;
            case GM_LEFT_TOP_DOWN:
                swap(x, y);
                flip(y);
                break;
            case GM_RIGHT_TOP_DOWN:
                swap(x, y);
                flipY(y);
                break;
            case GM_RIGHT_BOTTOM_UP:
                swap(x, y);
                flipX(x);
                flip(y);
                flipY(y);
                break;
            case GM_LEFT_BOTTOM_UP:
                swap(x, y);
                flipX(x);
                break;
        }
        if (x < 0 || x >= _maxX || y < 0 || y >= _maxY) return -1;

        int16_t b = y;
        switch (_rot) {
            // case 0: break;
            case 1:
                y = (y & 0xF8) + (x & 7);
                x = (x & 0xF8) + 7 - (b & 7);
                break;
            case 2:
                flip(x);
                flip(y);
                break;
            case 3:
                y = (y & 0xF8) + 7 - (x & 7);
                x = (x & 0xF8) + (b & 7);
                break;
        }

        switch (_flip) {
            // case 0: break;
            case 1:
                flip(x);
                break;
            case 2:
                flip(y);
                break;
            case 3:
                flip(x);
                flip(y);
                break;
        }

        if (_type == GM_ZIGZAG) {
            if ((y >> 3) & 1) {                  // если это нечётная матрица: (y / 8) % 2
                x = _maxX - 1 - x;               // отзеркалить x
                y = (y & 0xF8) + (7 - (y & 7));  // отзеркалить y: (y / 8) * 8 + (7 - (y % 8));
            }
        }

        _bx = x & 7;
        return WIDTH * (HEIGHT - 1 - (y >> 3)) + (WIDTH - 1 - (x >> 3)) + (y & 7) * WIDTH * HEIGHT;  // позиция в буфере
    }

    void flip(int16_t& v) {
        v = (v & 0xF8) + 7 - (v & 7);  // (v / 8 + 1) * 8 - 1 - (v % 8)
    }
    void flipX(int16_t& v) {
        v = _maxX - v - 1;
    }
    void flipY(int16_t& v) {
        v = _maxY - v - 1;
    }
    void swap(int16_t& x, int16_t& y) {
        int16_t b = y;
        y = x;
        x = b;
    }

    void fastWrite(const uint8_t pin, bool val) {
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
        if (pin < 8) bitWrite(PORTD, pin, val);
        else if (pin < 14) bitWrite(PORTB, (pin - 8), val);
        else if (pin < 20) bitWrite(PORTC, (pin - 14), val);
#elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13__)
        bitWrite(PORTB, pin, val);
#else
        digitalWrite(pin, val);
#endif
    }

    void F_fastShiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t data) {
#if defined(AVR)
        volatile uint8_t* _clk_port = portOutputRegister(digitalPinToPort(clockPin));
        volatile uint8_t* _dat_port = portOutputRegister(digitalPinToPort(dataPin));
        uint8_t _clk_mask = digitalPinToBitMask(clockPin);
        uint8_t _dat_mask = digitalPinToBitMask(dataPin);
        for (uint8_t i = 0; i < 8; i++) {
            if (bitOrder == MSBFIRST) {
                if (data & (1 << 7)) *_dat_port |= _dat_mask;
                else *_dat_port &= ~_dat_mask;
                data <<= 1;
            } else {
                if (data & 1) *_dat_port |= _dat_mask;
                else *_dat_port &= ~_dat_mask;
                data >>= 1;
            }
            *_clk_port |= _clk_mask;
            *_clk_port &= ~_clk_mask;
        }
#else
        shiftOut(dataPin, clockPin, bitOrder, data);
#endif
    }
    void sendData(uint8_t address, uint8_t value) {
        if (DATpin == CLKpin) {
            SPI.transfer(address);
            SPI.transfer(value);
        } else {
            F_fastShiftOut(DATpin, CLKpin, MSBFIRST, address);
            F_fastShiftOut(DATpin, CLKpin, MSBFIRST, value);
        }
    }
    void sendCMD(uint8_t address, uint8_t value) {
        beginData();
        for (uint16_t i = 0; i < _amount; i++) sendData(address, value);
        endData();
    }
    void sendCMDs(uint8_t address, uint8_t* values) {
        beginData();
        for (uint16_t i = 0; i < _amount; i++) sendData(address, values[_amount - i - 1]);
        endData();
    }

    const uint16_t _amount = WIDTH * HEIGHT;
    const int16_t _maxX = WIDTH * 8;
    const int16_t _maxY = HEIGHT * 8;
    uint8_t _rot = 0, _bx = 0, _flip = 0, _conn = 0;
    bool _type = GM_ZIGZAG;
};
#endif
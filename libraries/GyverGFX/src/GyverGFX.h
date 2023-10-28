/*
    Лёгкая библиотека двухмерной графики для дисплеев и матриц
    Документация:
    GitHub: https://github.com/GyverLibs/GyverGFX
    Возможности:
    - Точки
    - Линии
    - Прямоугольники
    - Скруглённые прямоугольники
    - Круги
    - Кривая Безье
    - Битмап
    - Вывод текста (русский, английский) нескольких размеров

    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License
*/

#ifndef _GyverGFX_h
#define _GyverGFX_h

#include <Arduino.h>

#define GFX_CLEAR 0
#define GFX_FILL 1
#define GFX_STROKE 2

#define GFX_REPLACE 0
#define GFX_ADD 1

#ifndef GFX_NO_PRINT
#include <Print.h>

#include "fonts/font5x8.h"
#include "fonts/icons8x8.h"
struct gfx_config_t {
    int16_t x = 0, y = 0;
    uint8_t scale = 1;
    bool invert = 0, println = 0, tmode = 0;
    int16_t tx0, tx1;
};
#endif

#ifdef GFX_NO_PRINT
class GyverGFX {
#else
class GyverGFX : public Print {
#endif
   public:
    GyverGFX() {
        size(0, 0);
    }
    GyverGFX(uint16_t x, uint16_t y) {
        size(x, y);
    }

    // установить размер
    void size(uint16_t x, uint16_t y) {
        _w = x;
        _h = y;
#ifndef GFX_NO_PRINT
        resetTextBound();
#endif
    }

    // получить ширину
    uint16_t width() {
        return _w;
    }
    // получить высоту
    uint16_t height() {
        return _h;
    }

    // =================== INTERFACE ===================
    // точка
    virtual void dot(int x, int y, uint8_t fill = GFX_FILL) {
    }

    // залить экран
    virtual void fill(uint8_t fill = GFX_FILL) {
        for (uint16_t i = 0; i < _w; i++) {
            for (uint16_t j = 0; j < _h; j++) {
                dotSecure(i, j, fill);
            }
        }
    }

    // очистить экран
    virtual void clear() {
        fill(0);
    }

    // обновить (интерфейсная)
    virtual void update() = 0;

    // =================== GFX ===================

    // вертикальная линия
    void lineH(int y, int x0, int x1, uint8_t fill = GFX_FILL) {
        swap(x0, x1);
        for (int x = x0; x <= x1; x++) dotSecure(x, y, fill);
    }

    // вертикальная линия
    void lineV(int x, int y0, int y1, uint8_t fill = GFX_FILL) {
        swap(y0, y1);
        for (int y = y0; y <= y1; y++) dotSecure(x, y, fill);
    }

    // линия
    void line(int x0, int y0, int x1, int y1, uint8_t fill = GFX_FILL) {
        if (x0 == x1) lineV(x0, y0, y1, fill);
        else if (y0 == y1) lineH(y0, x0, x1, fill);
        else {
            int8_t sx = (x0 < x1) ? 1 : -1;
            int8_t sy = (y0 < y1) ? 1 : -1;
            int dx = abs(x1 - x0);
            int dy = abs(y1 - y0);
            int err = dx - dy;
            int e2 = 0;
            for (;;) {
                dotSecure(x0, y0, fill);
                if (x0 == x1 && y0 == y1) return;
                e2 = err << 1;
                if (e2 > -dy) {
                    err -= dy;
                    x0 += sx;
                }
                if (e2 < dx) {
                    err += dx;
                    y0 += sy;
                }
            }
        }
    }

    // прямоугольник (x0, y0, x1, y1, fill)
    void rect(int x0, int y0, int x1, int y1, uint8_t fill = GFX_FILL) {
        swap(y0, y1);
        swap(x0, x1);
        if (fill == GFX_STROKE) {
            lineH(y0, x0 + 1, x1 - 1);
            lineH(y1, x0 + 1, x1 - 1);
            lineV(x0, y0, y1);
            lineV(x1, y0, y1);
        } else {
            for (int y = y0; y <= y1; y++) lineH(y, x0, x1, fill);
        }
    }

    // прямоугольник (x0, y0, w, h, fill)
    void rectWH(int x0, int y0, int w, int h, uint8_t fill = GFX_FILL) {
        rect(x0, y0, x0 + w - 1, y0 + h - 1, fill);
    }

    // скруглённый прямоугольник (x0, y0, x1, y1, fill)
    void roundRect(int x0, int y0, int x1, int y1, uint8_t fill = GFX_FILL) {
        swap(y0, y1);
        swap(x0, x1);
        if (fill == GFX_STROKE) {
            lineV(x0, y0 + 2, y1 - 2);
            lineV(x1, y0 + 2, y1 - 2);
            lineH(y0, x0 + 2, x1 - 2);
            lineH(y1, x0 + 2, x1 - 2);
            dotSecure(x0 + 1, y0 + 1);
            dotSecure(x1 - 1, y0 + 1);
            dotSecure(x1 - 1, y1 - 1);
            dotSecure(x0 + 1, y1 - 1);
        } else {
            lineV(x0, y0 + 2, y1 - 2, fill);
            lineV(x0 + 1, y0 + 1, y1 - 1, fill);
            lineV(x1 - 1, y0 + 1, y1 - 1, fill);
            lineV(x1, y0 + 2, y1 - 2, fill);
            rect(x0 + 2, y0, x1 - 2, y1, fill);
        }
    }

    // скруглённый прямоугольник (x0, y0, x1, y1, fill)
    void roundRectWH(int x0, int y0, int w, int h, uint8_t fill = GFX_FILL) {
        roundRect(x0, y0, x0 + w - 1, y0 + h - 1, fill);
    }

    // окружность
    void circle(int x, int y, int radius, uint8_t fill = GFX_FILL) {
        int f = 1 - radius;
        int ddF_x = 1;
        int ddF_y = -2 * radius;
        int x1 = 0;
        int y1 = radius;
        uint8_t fillLine = (fill == GFX_CLEAR) ? 0 : 1;
        dotSecure(x, y + radius, fillLine);
        dotSecure(x, y - radius, fillLine);
        dotSecure(x + radius, y, fillLine);
        dotSecure(x - radius, y, fillLine);
        if (fill != GFX_STROKE) lineV(x, y - radius, y + radius - 1, fillLine);
        while (x1 < y1) {
            if (f >= 0) {
                y1--;
                ddF_y += 2;
                f += ddF_y;
            }
            x1++;
            ddF_x += 2;
            f += ddF_x;
            if (fill == GFX_STROKE) {
                dotSecure(x + x1, y + y1);
                dotSecure(x - x1, y + y1);
                dotSecure(x + x1, y - y1);
                dotSecure(x - x1, y - y1);
                dotSecure(x + y1, y + x1);
                dotSecure(x - y1, y + x1);
                dotSecure(x + y1, y - x1);
                dotSecure(x - y1, y - x1);
            } else {
                lineV(x + x1, y - y1, y + y1, fillLine);
                lineV(x - x1, y - y1, y + y1, fillLine);
                lineV(x + y1, y - x1, y + x1, fillLine);
                lineV(x - y1, y - x1, y + x1, fillLine);
            }
        }
    }

    // кривая Безье
    void bezier(uint8_t *arr, uint8_t size, uint8_t dense, uint8_t fill = GFX_FILL) {
        uint16_t a[size * 2];
        for (uint16_t i = 0; i < (uint16_t)(1 << dense); i++) {
            for (uint16_t j = 0; j < size * 2; j++) a[j] = arr[j] << 3;
            for (uint16_t j = (size - 1) * 2 - 1; j > 0; j -= 2) {
                for (uint16_t k = 0; k <= j; k++) {
                    a[k] = a[k] + (((a[k + 2] - a[k]) * i) >> dense);
                }
            }
            dotSecure(a[0] >> 3, a[1] >> 3, fill);
        }
    }

    // кривая Безье 16 бит
    void bezier16(int16_t *arr, uint8_t size, uint8_t dense, uint8_t fill = GFX_FILL) {
        uint16_t a[size * 2];
        for (uint16_t i = 0; i < (uint16_t)(1 << dense); i++) {
            for (uint16_t j = 0; j < size * 2; j++) a[j] = arr[j];
            for (uint16_t j = (size - 1) * 2 - 1; j > 0; j -= 2) {
                for (uint16_t k = 0; k <= j; k++) {
                    a[k] = a[k] + (((a[k + 2] - a[k]) * i) >> dense);
                }
            }
            dotSecure(a[0], a[1], fill);
        }
    }

    // битмап
    void drawBitmap(int x, int y, const uint8_t *frame, int width, int height, uint8_t invert = 0, uint8_t mode = GFX_REPLACE) {
        uint8_t bytes = width >> 3;
        uint8_t left = width & 0b111;
        if (left) bytes++;

        for (int yy = 0; yy < height; yy++) {
            for (int xx = 0; xx < (width >> 3); xx++) {
                uint8_t thisByte = pgm_read_word(&(frame[xx + yy * bytes])) ^ invert;
                for (uint8_t k = 0; k < 8; k++) {
                    uint8_t val = thisByte & 0b10000000;
                    if (val || !mode) dotSecure((xx << 3) + k + x, yy + y, val);
                    thisByte <<= 1;
                }
            }
            if (left) {
                uint8_t thisByte = pgm_read_byte(&(frame[(width >> 3) + yy * bytes])) ^ invert;
                for (uint8_t k = 0; k < left; k++) {
                    uint8_t val = thisByte & 0b10000000;
                    if (val || !mode) dotSecure(((width >> 3) << 3) + k + x, yy + y, val);
                    thisByte <<= 1;
                }
            }
        }
    }

    // ==================== TEXT =====================
#ifndef GFX_NO_PRINT
    // определить длину строки с любыми символами (в т.ч. русскими)
    uint16_t strlen_fix(const char *str) {
        uint16_t i = 0, count = 0;
        while (str[i]) {
            if ((str[i] & 0xc0) != 0x80) count++;
            i++;
        }
        return count;
    }

    // определить длину PGM строки с любыми символами (в т.ч. русскими)
    uint16_t strlen_fix_P(PGM_P str) {
        uint16_t i = 0, count = 0;
        char c;
        while (1) {
            c = pgm_read_byte(str + i);
            if (!c) break;
            if ((c & 0xc0) != 0x80) count++;
            i++;
        }
        return count;
    }

    // установить курсор
    void setCursor(int x, int y) {
        cfg.x = x;
        cfg.y = y;
    }

    // получить курсор x
    int getCursorX() {
        return cfg.x;
    }

    // получить курсор y
    int getCursorY() {
        return cfg.y;
    }

    // установить масштаб текста (1-4)
    void setScale(uint8_t scale) {
        scale = constrain(scale, 1, 4);
        cfg.scale = scale;
    }

    // получить масштаб текста
    uint8_t getScale() {
        return cfg.scale;
    }

    // установить инверсию текста
    void invertText(bool inv) {
        cfg.invert = inv;
    }

    // получить инверсию текста
    bool getInvertText() {
        return cfg.invert;
    }

    // установить автоматический перенос текста
    void autoPrintln(bool mode) {
        cfg.println = mode;
    }

    // получить автоматический перенос текста
    bool getAutoPrintln() {
        return cfg.println;
    }

    // установить режим вывода текста GFX_ADD/GFX_REPLACE
    void textDisplayMode(bool mode) {
        cfg.tmode = mode;
    }

    // получить режим вывода текста
    bool getTextDisplayMode() {
        return cfg.tmode;
    }

    // установить границы вывода текста по х
    void setTextBound(int x0, int x1) {
        cfg.tx0 = x0;
        cfg.tx1 = x1;
    }

    // получить границу вывода 0
    int getTextBoundX0() {
        return cfg.tx0;
    }

    // получить границу вывода 1
    int getTextBoundX1() {
        return cfg.tx1;
    }

    // сбросить границы вывода текста до (0, ширина)
    void resetTextBound() {
        cfg.tx0 = 0;
        cfg.tx1 = _w - 1;
    }

    // ================== WRITE ===================
    size_t write(uint8_t data) {
        bool newPos = false;
        if (data == '\r') return 1;

        if (data == '\n') {  // получен перевод строки
            cfg.y += (cfg.scale << 3);
            cfg.x = 0;
            newPos = true;
            data = 0;
        }
        if (cfg.println && (cfg.x + 6 * cfg.scale) >= (int16_t)_w) {
            cfg.x = 0;  // строка переполненена, перевод и возврат
            cfg.y += (cfg.scale << 3);
            newPos = true;
        }
        if (newPos) setCursor(cfg.x, cfg.y);  // переставляем курсор
        // if (cfg.y + (cfg.scale << 3) >= _h) data = 0;                 // дисплей переполнен
        if (cfg.println && data == ' ' && cfg.x == 0) data = 0;  // первый пробел

        // фикс русских букв и некоторых символов
        if (data > 127) {
            uint8_t thisData = data;
            // data = 0 - флаг на пропуск
            if (data > 191) data = 0;
            else if (_lastChar == 209 && data == 145) data = 192;  // ё кастомная
            else if (_lastChar == 208 && data == 129) data = 149;  // Е вместо Ё
            else if (_lastChar == 226 && data == 128) data = 0;    // тире вместо длинного тире (начало)
            else if (_lastChar == 128 && data == 148) data = 45;   // тире вместо длинного тире
            _lastChar = thisData;
        }
        if (data == 0) return 1;
        // если тут не вылетели - печатаем символ

        int newX = cfg.x + cfg.scale * 6;
        if (newX < cfg.tx0 || cfg.x > cfg.tx1) {  // пропускаем вывод "за экраном"
            cfg.x = newX;
        } else {
            for (uint8_t col = 0; col < 6; col++) {  // 6 столбиков буквы
                uint8_t bits = getFont(data, col);   // получаем байт
                drawByte(bits);
            }
        }
        return 1;
    }

    // вывести столбик-байт в текущий курсор с учётом масштаба и режима текста, автоматически перенесёт курсор
    void drawBytes_P(const uint8_t *bytes, int amount) {
        for (int i = 0; i < amount; i++) drawByte(pgm_read_byte(&bytes[i]));
    }
    void drawBytes(uint8_t *bytes, int amount) {
        for (int i = 0; i < amount; i++) drawByte(bytes[i]);
    }
    void drawByte(uint8_t bits) {
        if (cfg.invert) bits = ~bits;
        if (cfg.scale == 1) {                         // если масштаб 1
            if (cfg.x >= 0 && cfg.x < (int16_t)_w) {  // внутри дисплея
                for (uint8_t y = 0; y < 8; y++) {
                    bool bit = bitRead(bits, y);
                    if ((bit || !cfg.tmode) && (cfg.x >= cfg.tx0 && cfg.x <= cfg.tx1)) dotSecure(cfg.x, cfg.y + y, bit);
                }
            }
            cfg.x++;
        } else {  // масштаб 2, 3 или 4 - растягиваем шрифт
            uint32_t buf = 0;
            for (uint8_t i = 0, count = 0; i < 8; i++) {
                for (uint8_t j = 0; j < cfg.scale; j++, count++) {
                    bitWrite(buf, count, bitRead(bits, i));  // пакуем растянутый шрифт
                }
            }

            for (uint8_t i = 0; i < cfg.scale; i++) {
                for (uint8_t j = 0; j < (cfg.scale << 3); j++) {
                    bool bit = bitRead(buf, j);
                    if ((bit || !cfg.tmode) && (cfg.x + i >= cfg.tx0 && cfg.x + i <= cfg.tx1)) dotSecure(cfg.x + i, cfg.y + j, bit);
                }
            }
            cfg.x += cfg.scale;
        }
    }

    gfx_config_t cfg;
#endif

    // ============= DEPRECATED =============
    int W() {
        return _w;
    }
    int H() {
        return _h;
    }
    void fastLineH(int y, int x0, int x1, uint8_t fill = GFX_FILL) {
        lineH(y, x0, x1, fill);
    }
    void fastLineV(int x, int y0, int y1, uint8_t fill = GFX_FILL) {
        lineV(x, y0, y1, fill);
    }

   protected:
    void swap(int &a, int &b) {
        if (a > b) {
            int c = a;
            a = b;
            b = c;
        }
    }
    void dotSecure(int x, int y, uint8_t fill = 1) {
        if (x < 0 || x >= (int16_t)_w || y < 0 || y >= (int16_t)_h) return;
        dot(x, y, fill);
    }
#ifndef GFX_NO_PRINT
    uint8_t getFont(uint8_t font, uint8_t row) {
        if (row > 4) return 0;
        font = font - '0' + 16;                                                                // перевод код символа из таблицы ASCII
        if (font <= 95) return pgm_read_byte(&(charMap[font][row]));                           // для английских букв и символов
        else if (font >= 96 && font <= 111) return pgm_read_byte(&(charMap[font + 47][row]));  // для русских
        else if (font <= 159) return pgm_read_byte(&(charMap[font - 17][row]));
        else return pgm_read_byte(&(charMap[font - 1][row]));  // для кастомных (ё)
    }
    uint8_t _lastChar;
#endif

    uint16_t _w, _h;
};
#endif
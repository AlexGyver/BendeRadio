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

    Версии:
    v1.0 - релиз
    v1.1 - оптимизация памяти
    v1.2 - небольшая оптимизация
    v1.3 - добавил фичи
    v1.4 - мелкие фиксы
    v1.5 - добавлено отключение модуля вывода текста GFX_NO_PRINT
    v1.5.1 - мелкие фиксы
    v1.6 - 
*/

#ifndef _GyverGFX_h
#define _GyverGFX_h

#include <Arduino.h>

#ifndef GFX_NO_PRINT
#include <Print.h>

#include "charMap.h"
#endif

#define GFX_CLEAR 0
#define GFX_FILL 1
#define GFX_STROKE 2

#define GFX_ADD 0
#define GFX_REPLACE 1

#ifdef GFX_NO_PRINT
class GyverGFX {
#else
class GyverGFX : public Print {
#endif
   public:
    GyverGFX() {
        size(0, 0);
    }
    GyverGFX(int x, int y) {
        size(x, y);
    }

    // установить размер
    void size(int x, int y) {
        _w = x;
        _h = y;
        resetTextBound();
    }

    // точка
    virtual void dot(int x, int y, uint8_t fill = GFX_FILL) {
    }

    // залить
    virtual void fill(uint8_t fill = GFX_FILL) {
        for (int i = 0; i < _w; i++)
            for (int j = 0; j < _h; j++)
                dotSecure(i, j, fill);
    }

    // очистить
    virtual void clear() {
        fill(0);
    }

    // обновить (интерфейсная)
    virtual void update() = 0;

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

    // прямоугольник
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

    // скруглённый прямоугольник
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
        int a[size * 2];
        for (int i = 0; i < (1 << dense); i++) {
            for (int j = 0; j < size * 2; j++) a[j] = arr[j] << 3;
            for (int j = (size - 1) * 2 - 1; j > 0; j -= 2) {
                for (int k = 0; k <= j; k++) {
                    a[k] = a[k] + (((a[k + 2] - a[k]) * i) >> dense);
                }
            }
            dotSecure(a[0] >> 3, a[1] >> 3, fill);
        }
    }

    // кривая Безье 16 бит
    void bezier16(int *arr, uint8_t size, uint8_t dense, uint8_t fill = GFX_FILL) {
        int a[size * 2];
        for (int i = 0; i < (1 << dense); i++) {
            for (int j = 0; j < size * 2; j++) a[j] = arr[j];
            for (int j = (size - 1) * 2 - 1; j > 0; j -= 2) {
                for (int k = 0; k <= j; k++) {
                    a[k] = a[k] + (((a[k + 2] - a[k]) * i) >> dense);
                }
            }
            dotSecure(a[0], a[1], fill);
        }
    }

    // битмап
    void drawBitmap(int x, int y, const uint8_t *frame, int width, int height, uint8_t invert = 0, uint8_t mode = GFX_FILL) {
        byte bytes = width >> 3;
        byte left = width & 0b111;
        if (left) bytes++;

        for (int yy = 0; yy < height; yy++) {
            for (int xx = 0; xx < (width >> 3); xx++) {
                byte thisByte = pgm_read_word(&(frame[xx + yy * bytes])) ^ invert;
                for (byte k = 0; k < 8; k++) {
                    byte val = thisByte & 0b10000000;
                    if (val || mode) dotSecure((xx << 3) + k + x, yy + y, val);
                    thisByte <<= 1;
                }
            }
            if (left) {
                byte thisByte = pgm_read_byte(&(frame[(width >> 3) + yy * bytes])) ^ invert;
                for (byte k = 0; k < left; k++) {
                    byte val = thisByte & 0b10000000;
                    if (val || mode) dotSecure(((width >> 3) << 3) + k + x, yy + y, val);
                    thisByte <<= 1;
                }
            }
        }
    }

    // установить курсор
    void setCursor(int x, int y) {
        _x = x;
        _y = y;
    }

    // масштаб текста
    void setScale(uint8_t scale) {
        scale = constrain(scale, 1, 4);
        _scaleX = scale;
        _scaleY = scale * 8;
    }

    // инвертировать текст
    void invertText(bool inv) {
        _invert = inv;
    }

    // автоматический перенос строки
    void autoPrintln(bool mode) {
        _println = mode;
    }

    // режим вывода текста GFX_ADD/GFX_REPLACE
    void textDisplayMode(bool mode) {
        _mode = mode;
    }

    size_t write(uint8_t data) {
#ifndef GFX_NO_PRINT
        bool newPos = false;
        if (data == '\r') return 1;

        if (data == '\n') {  // получен перевод строки
            _y += _scaleY;
            _x = 0;
            newPos = true;
            data = 0;
        }
        if (_println && (_x + 6 * _scaleX) >= _w) {
            _x = 0;  // строка переполненена, перевод и возврат
            _y += _scaleY;
            newPos = true;
        }
        if (newPos) setCursor(_x, _y);  // переставляем курсор
        // if (_y + _scaleY >= _h) data = 0;                 // дисплей переполнен
        if (_println && data == ' ' && _x == 0) data = 0;  // первый пробел

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

        int newX = _x + _scaleX * 6;
        if (newX < _tx0 || _x >= _tx1) {  // пропускаем вывод "за экраном"
            _x = newX;
        } else {
            for (uint8_t col = 0; col < 6; col++) {  // 6 столбиков буквы
                uint8_t bits = getFont(data, col);   // получаем байт
                if (_invert) bits = ~bits;
                if (_scaleX == 1) {            // если масштаб 1
                    if (_x >= 0 && _x < _w) {  // внутри дисплея
                        for (uint8_t y = 0; y < 8; y++) {
                            bool bit = bitRead(bits, y);
                            if ((bit || _mode) && (_x >= _tx0 && _x <= _tx1)) dotSecure(_x, _y + y, bit);
                        }
                    }
                    _x++;
                } else {  // масштаб 2, 3 или 4 - растягиваем шрифт
                    uint32_t buf = 0;
                    for (uint8_t i = 0, count = 0; i < 8; i++) {
                        for (uint8_t j = 0; j < _scaleX; j++, count++) {
                            bitWrite(buf, count, bitRead(bits, i));  // пакуем растянутый шрифт
                        }
                    }

                    for (uint8_t i = 0; i < _scaleX; i++) {
                        for (uint8_t j = 0; j < _scaleY; j++) {
                            bool bit = bitRead(buf, j);
                            if ((bit || _mode) && (_x + i >= _tx0 && _x + i <= _tx1)) dotSecure(_x + i, _y + j, bit);
                        }
                    }
                    _x += _scaleX;
                }
            }
        }
#endif
        return 1;
    }

    // установить границы вывода текста по х
    void setTextBound(int x0, int x1) {
#ifndef GFX_NO_PRINT
        _tx0 = x0;
        _tx1 = x1;
#endif
    }

    // сбросить границы вывода текста до (0, ширина)
    void resetTextBound() {
#ifndef GFX_NO_PRINT
        _tx0 = 0;
        _tx1 = _w - 1;
#endif
    }

    // получить ширину
    int W() {
        return _w;
    }

    // получить высоту
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
        if (x < 0 || x >= _w || y < 0 || y >= _h) return;
        dot(x, y, fill);
    }
    uint8_t getFont(uint8_t font, uint8_t row) {
#ifndef GFX_NO_PRINT
        if (row > 4) return 0;
        font = font - '0' + 16;                                                                // перевод код символа из таблицы ASCII
        if (font <= 95) return pgm_read_byte(&(charMap[font][row]));                           // для английских букв и символов
        else if (font >= 96 && font <= 111) return pgm_read_byte(&(charMap[font + 47][row]));  // для русских
        else if (font <= 159) return pgm_read_byte(&(charMap[font - 17][row]));
        else return pgm_read_byte(&(charMap[font - 1][row]));  // для кастомных (ё)
#endif
    }

    int _x = 0, _y = 0;
    uint8_t _scaleX = 1, _scaleY = 8;
    bool _invert = 0, _println = 0, _mode = 1;
    uint8_t _lastChar;
    int _w, _h;

#ifndef GFX_NO_PRINT
    int _tx0, _tx1;
#endif
};
#endif
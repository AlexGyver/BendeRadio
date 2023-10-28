[![latest](https://img.shields.io/github/v/release/GyverLibs/GyverGFX.svg?color=brightgreen)](https://github.com/GyverLibs/GyverGFX/releases/latest/download/GyverGFX.zip)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/GyverGFX?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# GyverGFX
Движок двухмерной графики для дисплеев и матриц
- Точка
- Линии
- Прямоугольники
- Окружность
- Кривая Безье
- Битмап
- Вывод текста (русский, английский) нескольких размеров
- Вшиты 155 иконок
- Бегущая строка

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Установка](#install)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **GyverGFX** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/GyverGFX/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!

<a id="usage"></a>
## Использование
### GyverGFX

```cpp
// конструктор
GyverGFX();
GyverGFX(int x, int y); // с указанием размеров "экрана"

// методы
void size(int x, int y);    // установить размер
int width();                // получить ширину
int height();               // получить высоту

// ============ PRIMITIVE ============
// fill:
// GFX_CLEAR - очистить фигуру
// GFX_FILL - залить фигуру
// GFX_STROKE - обвести фигуру
virtual void dot(int x, int y, uint8_t fill = 1);                   // точка
void fill(uint8_t fill = 1);                                        // залить экран
void clear();                                                       // очистить экран
void lineH(int y, int x0, int x1, uint8_t fill = 1);                // вертикальная линия
void lineV(int x, int y0, int y1, uint8_t fill = 1);                // горизонтальная линия
void line(int x0, int y0, int x1, int y1, uint8_t fill = 1);        // линия
void rect(int x0, int y0, int x1, int y1, uint8_t fill = 1);        // прямоугольник (координаты углов)
void rectWH(int x0, int y0, int w, int h, uint8_t fill = 1);        // прямоугольник (координаты угла и размер)
void roundRect(int x0, int y0, int x1, int y1, uint8_t fill = 1);   // скруглённый прямоугольник (координаты углов)
void roundRectWH(int x0, int y0, int w, int h, uint8_t fill = 1);   // скруглённый прямоугольник (координаты угла и размер)
void circle(int x, int y, int radius, uint8_t fill = 1);            // окружность
void bezier(uint8_t *arr, uint8_t size, uint8_t dense, uint8_t fill = 1);     // кривая Безье
void bezier16(int16_t *arr, uint8_t size, uint8_t dense, uint8_t fill = 1);   // кривая Безье 16 бит

// ============ TEXT ============
// наследует Print.h
print(любой тип);
println(любой тип);

// вывести столбик-байт в текущий курсор с учётом масштаба и режима текста, автоматически перенесёт курсор
void drawByte(uint8_t bits);
void drawBytes(uint8_t *bytes, int amount);           // из массива
void drawBytes_P(const uint8_t *bytes, int amount);   // из PROGMEM массива

// mode:
// GFX_ADD - добавить к буферу
// GFX_REPLACE - заменить информацию в буфере
void drawBitmap(int x, int y, const uint8_t *frame, int width, int height, uint8_t invert = 0, byte mode = 0);  // битмап (mode: GFX_ADD/GFX_REPLACE)
void textDisplayMode(bool mode);        // установить режим вывода текста
bool getTextDisplayMode();              // получить режим вывода текста

void setCursor(int x, int y);           // установить курсор
int getCursorX();                       // получить курсор x
int getCursorY();                       // получить курсор y

void setScale(uint8_t scale);           // установить масштаб текста (1-4)
uint8_t getScale();                     // получить масштаб текста

void invertText(bool inv);              // установить инверсию текста
bool getInvertText();                   // получить инверсию текста

void autoPrintln(bool mode);            // установить автоматический перенос текста
bool getAutoPrintln();                  // получить автоматический перенос текста

void setTextBound(int x0, int x1);      // установить границы вывода текста по х
void resetTextBound();                  // сбросить границы вывода текста до (0, ширина)
int getTextBoundX0();                   // получить границу вывода 0
int getTextBoundX1();                   // получить границу вывода 1

uint16_t strlen_fix(const char *str);   // определить длину строки с любыми символами (в т.ч. русскими)
uint16_t strlen_fix_P(PGM_P str);       // определить длину PGM строки с любыми символами (в т.ч. русскими)

// ============ DEFINE ============
// дефайны настроек
// (перед подключением библиотеки)
#define GFX_NO_PRINT                    // отключить модуль вывода текста (экономия памяти)
```

#### Встроенные иконки
В библиотеке идёт набор из 155 иконок 8х8, файл *fonts/icons8x8.h*. Выводятся следующим образом:
```cpp
obj.drawBytes_P(GFX_icons::alarm, 8);
obj.drawBytes_P(GFX_icons::email, 8);
```

### RunningGFX
Асинхронная бегущая строка. Не хранит строку внутри себя, поэтому по очевидным причинам поддерживает работу с:
- Глобально созданными `String` - строками
- Глобально созданными `PROGMEM` - строками
- Глобально созданными `char[]` - строками
- Строковыми константами (они изначально являются глобальными)

Особенности:
- Для автоматического обновления строки в классе должен быть реализован метод `update()`
- Строку можно изменять во время движения: `setText()` не запускает движение заново, поэтому при обновлении String-строки можно вызвать setText для автоматического пересчёта длины
- Настройки бегущей строки (масштаб, режим текста) не влияют на дисплей, точнее автоматически устанавливаются обратно
- Один объект управляет одной строкой. если нужно несколько одновременно бегущих строк - создай несколько обьектов, пример ниже

```cpp
// конструктор
RunningGFX(GyverGFX* gfx);

// методы
void setText(const char* str);      // установить текст const char*
void setText(String& str);          // установить текст String
void setText_P(PGM_P str);          // установить текст из PROGMEM (глобальный)

void invertText(bool inv);          // инвертировать текст
void setScale(uint8_t scale);       // масштаб текста
void textDisplayMode(bool mode);    // режим вывода текста GFX_ADD/GFX_REPLACE
void setWindow(int16_t x0, int16_t x1, int16_t y);  // установить окно (x0, x1, y)
void setSpeed(uint16_t pixPerSec);  // установить скорость (пикселей в секунду)

void start();                       // запустить бегущую строку с начала
void stop();                        // остановить бегущую строку
void resume();                      // продолжить движение с момента остановки

// тикер. Вернёт 0 в холостом, 1 при новом шаге, 2 при завершении движения
// Можно передать false чтобы дисплей не обновлялся сам
uint8_t tick(bool update = true);

// сдвинуть строку на 1 пиксель. Можно передать false чтобы дисплей не обновлялся сам
uint8_t tickManual(bool update = true);
```

<a id="example"></a>
## Пример

```cpp
// пример наследования в класс. Нужно реализовать методы dot и update
class MAX7219 : public GyverGFX {
public:
  MAX7219(int width, int height) : GyverGFX(width * 8, height * 8) {}
  void dot(int x, int y, uint8_t fill = GFX_FILL) {
    // ...
  }
  void update() {
    // ...
  }
};
```

```cpp
// пример RunningGFX
// RunningGFX работает с классом, который наследует GyverGFX. Например библиотека GyverMAX7219

MAX7219<4, 1, 5> mtrx;  // одна матрица (1х1), пин CS на D5
RunningGFX run1(&mtrx);
RunningGFX run2(&mtrx);
const char pstr_g[] PROGMEM = "Global pgm string";
String str_g = "123";

void setup() {
  mtrx.begin();       // запускаем
  mtrx.setBright(5);  // яркость 0..15

  run1.setText("hello");  // строковая константа
  // run1.setText(str_g); // глобальная стринга
  run1.setSpeed(15);
  run1.setWindow(0, 16, 0);
  run1.start();

  run1.setText_P(pstr_g); // глобальная PGM строка
  run2.setSpeed(10);
  run2.setWindow(8, 16, 0);
  run2.start();
}

void loop() {
  run1.tick();
  run2.tick();
}
```

<a id="versions"></a>
## Версии
- v1.0
- v1.1 - оптимизация памяти
- v1.2 - небольшая оптимизация
- v1.3 - добавил фичи
- v1.4 - мелкие фиксы
- v1.5 - добавлено отключение модуля вывода текста GFX_NO_PRINT
- v1.5.1 - мелкие фиксы
- v1.6 - мелкие улучшения, добавлен движок бегущей строки
- v1.7 
  - Улучшена бегущая строка
  - Добавлены настройки
  - Добавлены новые инструменты вывода графики (drawByte, drawBytes, drawBytes_p)
  - Добавлены иконки
  - Улучшен шрифт

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код
[![latest](https://img.shields.io/github/v/release/GyverLibs/GyverMAX7219.svg?color=brightgreen)](https://github.com/GyverLibs/GyverMAX7219/releases/latest/download/GyverMAX7219.zip)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/GyverMAX7219?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# GyverMAX7219
Cамая резкая библиотека для матриц MAX7219 на диком западе
- Наследует графику из GyverGFX
- Подключение матриц зигзагом
- Аппаратный и программный SPI
- Невероятная оптимизация
- Работает с дисплеями любой конструкции

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Для работы нужна [GyverGFX](https://github.com/GyverLibs/GyverGFX)
- Библиотеку можно найти по названию **GyverMAX7219** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/GyverMAX7219/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!

<a id="init"></a>
## Инициализация

```cpp
MAX7219 < W, H, CS > mtrx;            // подключение к аппаратному SPI
// пример: UNO / Nano (CLK - D13, DI - D11, CS - любой пин)

MAX7219 < W, H, CS, DATA, CLK > mtrx; // подключение к любым пинам (софт SPI)
// W и H - количество МАТРИЦ по горизонтали и вертикали
// CS, DATA, CLK - номера пинов
```

## Дисплей
![setType](/doc/setType.png)
![setConnection](/doc/setConnection.png)

<a id="usage"></a>

## Использование
```cpp
// настройка
void begin();                       // запустить
void setBright(byte value);         // установить яркость [0-15]
void setBright(uint8_t* values);    // установить разную яркость
void setPower(bool value);          // переключить питание
void setPower(bool* values);        // переключить питание у выбранных
void clearDisplay();                // очистить дисплей (не буфер)

// ориентация
void setRotation(uint8_t rot);      // поворот МАТРИЦ (8x8): 0, 1, 2, 3 на 90 град по часовой стрелке
void setFlip(bool x, bool y);       // зеркальное отражение МАТРИЦ (8x8) по x и y
void setType(bool type);            // конструкция дисплея (тип строчности)
void setConnection(uint8_t conn);   // точка подключения дисплея

// графика
void fillByte(uint8_t data);        // залить байтом
void fill();                        // залить буфер (включить все)
void clear();                       // очистить буфер
void update();                      // обновить дисплей

void dot(int x, int y, uint8_t fill = 1); // установить точку
bool get(int16_t x, int16_t y);           // получить точку из буфера

// + наследует возможности GyverGFX https://github.com/GyverLibs/GyverGFX

// custom
void beginData();     // начать отправку
void endData();       // закончить отправку
// отправка данных напрямую в матрицу (строка, байт)
void sendByte(uint8_t address, uint8_t value);
```

<a id="example"></a>
## Пример
Остальные примеры смотри в **examples**!
```cpp
#include <GyverMAX7219.h>
MAX7219 < 1, 1, 5 > mtrx;   // одна матрица (1х1), пин CS на D5

void setup() {
  mtrx.begin();       // запускаем
  mtrx.setBright(5);  // яркость 0..15
  //mtrx.rotate(1);   // можно повернуть 0..3, по 90 град по часовой стрелке

  mtrx.dot(0, 0);     // пиксель на координатах 0,0
  mtrx.update();      // показать
  delay(1000);
  mtrx.clear();

  // линии крест накрест
  mtrx.line(0, 0, 7, 7);  // (x0, y0, x1, y1)
  mtrx.line(7, 0, 0, 7);
  mtrx.update();
  delay(1000);
  mtrx.clear();

  // круг
  mtrx.circle(3, 3, 3, GFX_FILL); // х, у, радиус, заливка
  mtrx.update();
  delay(1000);
  mtrx.clear();

  // окружность
  mtrx.circle(3, 3, 3, GFX_STROKE);
  mtrx.update();
  delay(1000);
  mtrx.clear();

  // остальную геометрию смотри в документации
}

void loop() {
}
```

<a id="versions"></a>
## Версии
- v1.0 - релиз
- v1.1 - оптимизирован SPI
- v1.2 - переделан FastIO
- v1.2.1 - исправлен баг в SPI (с 1.2)
- v1.2.2 - убран FastIO
- v1.3 - мелкие доработки и оптимизация, добавил поворот матриц
- v1.4 - добавил поддержку матричных дисплеев любой конфигурации (точка подключения, направление, чередование)
- v1.5 - добавил раздельное управление яркостью и питанием матриц

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

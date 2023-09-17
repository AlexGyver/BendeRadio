[![Foo](https://img.shields.io/badge/Version-1.9-brightgreen.svg?style=flat-square)](#versions)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)

[![Foo](https://img.shields.io/badge/README-ENGLISH-brightgreen.svg?style=for-the-badge)](https://github-com.translate.goog/GyverLibs/VolAnalyzer?_x_tr_sl=ru&_x_tr_tl=en)

# VolAnalyzer
Библиотека для амплитудного анализа звука на Arduino
- Хитрый адаптивный алгоритм
- Встроенные фильтры для плавного потока значений
- Получение громкости в указанном диапазоне независимо от диапазона входного сигнала
- Получение сигнала на резкие изменения звука
- Работа в режиме виртуального анализатора (без привязки к АЦП МК)

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
- Библиотеку можно найти по названию **VolAnalyzer** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/VolAnalyzer/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Инициализация
```cpp
VolAnalyzer analyzer(A0);   // указать пин
VolAnalyzer analyzer;       // "виртуальный" анализатор
```

<a id="usage"></a>
## Использование
```cpp
// =========================== TICK ===========================
// опрашивать как можно чаще. Может принимать значение, если это виртуальный анализатор
// вернёт true при окончании анализа выборки
bool tick(int read = -1);

// ========================== SETTINGS ==========================
void setPin(int8_t pin);        // указать пин АЦП
void setDt(uint16_t dt);        // установить время между опросами АЦП (мкс) (по умолч. 500) 
void setWindow(uint8_t window); // установка ширины окна выборки (по умолч. 20)
void setTrsh(uint16_t trsh);    // установить порог громкости в единицах АЦП (умолч 40)

// ========================== VOLUME ==========================
void setVolDt(uint8_t dt);      // установить период фильтрации громкости (умолч 20)
void setVolK(uint8_t vk);       // установить коэффициент плавности громкости 0-31 (умолч 25)
uint16_t getVol();              // получить громкость в пределах setVolMin.. setVolMax
void setVolMin(uint8_t vol);    // установить минимальную величину громкости (умолч 0)
void setVolMax(uint8_t vol);    // установить максимальную величину громкости (умолч 100)

// ========================= AMPLITUDE =========================
void setAmpliDt(uint8_t dt);    // установить период фильтрации амплитудных огибающих
void setAmpliK(uint8_t rk);     // установить коэффициент плавности амплитуды 0-31 (умолч 31)
uint16_t getMin();              // получить текущее значение огибающей минимумов (с v1.5 - 0)
uint16_t getMax();              // получить текущее значение огибающей максимумов

// =========================== PULSE ===========================
void setPulseMax(uint8_t maxV); // верхний порог срабатывания пульса (по шкале громкости)
void setPulseMin(uint8_t minV); // нижний порог перезагрузки пульса (по шкале громкости)
void setPulseTimeout(uint16_t tout);    // таймаут пульса, мс
bool pulse();                   // резкий скачок громкости (true)

// ========================== RAW DATA ===========================
uint16_t getRaw();              // получить значение сырого сигнала за выборку
uint16_t getTrsh();             // получить порог громкости в единицах АЦП
```

<a id="example"></a>
## Пример
```cpp
// амплитудный анализ звука

#include "VolAnalyzer.h"
VolAnalyzer analyzer(A0);


void setup() {
  Serial.begin(115200);
  analyzer.setVolK(20);
  analyzer.setTrsh(10);
  analyzer.setVolMin(10);
  analyzer.setVolMax(100);
}

void loop() {
  if (analyzer.tick()) {
    //Serial.print(analyzer.getVol());
    //Serial.print(',');
    Serial.print(analyzer.getRaw());
    Serial.print(',');
    Serial.print(analyzer.getMin());
    Serial.print(',');
    Serial.println(analyzer.getMax());
  }
}
```

<a id="versions"></a>
## Версии
- v1.0
- v1.1 - более резкое падение при пропадании звука
- v1.2 - +совместимость. Вернул плавное падение звука
- v1.3 - новый обработчик импульсов
- v1.4 - улучшение алгоритма
- v1.5 - сильное облегчение и улучшение алгоритма
- v1.6 - более резкая реакция на звук
- v1.7 - исключено деление на 0 в map
- v1.8 - теперь работает с 12 бит АЦП
- v1.9 - облегчил SRAM

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!
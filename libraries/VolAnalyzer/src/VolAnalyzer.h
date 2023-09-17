/*
    Библиотека для амплитудного анализа звука на Arduino
    Документация: 
    GitHub: https://github.com/GyverLibs/VolAnalyzer
    Возможности:
    - Хитрый адаптивный алгоритм
    - Встроенные фильтры для плавного потока значений
    - Получение громкости в указанном диапазоне независимо от диапазона входного сигнала
    - Получение сигнала на резкие изменения звука
    - Работа в режиме виртуального анализатора (без привязки к АЦП МК)
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
    v1.1 - более резкое падение при пропадании звука
    v1.2 - +совместимость. Вернул плавное падение звука
    v1.3 - упрощение алгоритма. Новый обработчик импульсов
    v1.4 - улучшение алгоритма
    v1.5 - сильное облегчение и улучшение алгоритма
    v1.6 - более резкая реакция на звук
    v1.7 - исключено деление на 0 в map
	v1.8 - теперь работает с 12 бит АЦП
    v1.9 - облегчил SRAM
*/

#ifndef _VolAnalyzer_h
#define _VolAnalyzer_h
#include <Arduino.h>

// ========================== FFilter ==========================
struct FFilter {
    bool compute(bool force = false) {
        if (force || (millis() & 0xFF) - tmr >= dt) {
            tmr = millis() & 0xFF;
            uint8_t kk = (raw < fil) ? k : (k >> 1);            // вверх с коэффициентом /2
            fil = ((uint32_t)kk * fil + (32 - kk) * raw) >> 5;  // целочисленный фильтр 5 бит
            return 1;
        }
        return 0;
    }
    uint8_t k = 20, dt = 0, tmr = 0;
    uint16_t fil = 0, raw = 0;
};

// ========================== VolAnalyzer ==========================
class VolAnalyzer {
public:
    // создать с указанием пина. Если не указывать - будет виртуальный анализатор
    VolAnalyzer(int pin = -1) {
        setVolDt(20);
        setVolK(25);
        setAmpliDt(150);
        setAmpliK(30);
        if (pin != -1) setPin(pin);
    }
    
    // =========================== TICK ===========================
    // опрашивать как можно чаще. Может принимать значение, если это виртуальный анализатор
    // вернёт true при окончании анализа выборки
    bool tick(int read = -1) {
        if (_pulse) _pulse = 0;
        volF.compute();                     // сглаживание громкости
        if (ampF.compute()) _ampli = 0;     // сглаживание амплитуды, сброс максимума

        // таймер выборки
        if (!_dt || micros() - _tmrDt >= _dt) {
            if (_dt) _tmrDt = micros();
            if (read == -1) read = analogRead(_pin);
            _max = max(_max, (uint16_t)read);   // поиск макс за выборку
            _min = min(_min, (uint16_t)read);   // поиск мин за выборку

            if (++_count >= _window) {          // выборка завершена
                _raw = _max - _min;             // сырая громкость
                _ampli = max(_ampli, _raw);     // амплитудная огибающая
                ampF.raw = _ampli;              // передаём в фильтр

                if (_raw > ampF.fil) ampF.compute(true);    // форсируем фильтр
                
                if (_raw > _trsh && ampF.fil > _trsh) {     // если звук громкий + в map не будет 0
                    // от порога _trsh до сглаженной амплитуды в (_volMin, _volMax)
                    volF.raw = map(_raw, _trsh, ampF.fil, _volMin, _volMax);
                    volF.raw = constrain(volF.raw, _volMin, _volMax);
                    volF.compute(true);    // форсируем фильтр
                } else volF.raw = 0;
                
                // обработка пульса
                if (!_pulseState) {
                    if (volF.raw <= _pulseMin && millis() - _tmrPulse >= _pulseTout) _pulseState = 1;
                } else {
                    if (volF.raw > _pulseMax) {
                        _pulseState = 0;
                        _pulse = 1;
                        _tmrPulse = millis();
                    }
                }
                _max = _count = 0;
                _min = 60000;
                return true;        // выборка завершена
            }
        }
        return false;
    }
    
    // ========================== SETTINGS ==========================
    // указать пин АЦП
    void setPin(int8_t pin) {
        _pin = pin;
        pinMode(_pin, INPUT);
    }
    
    // установить время между опросами АЦП (мкс) (по умолч. 500) 
    void setDt(uint16_t dt) {
        _dt = dt;
    }
    
    // установка ширины окна выборки (по умолч. 20)
    void setWindow(uint8_t window) {
        _window = window;
    }
    
    // установить порог громкости в единицах АЦП (умолч 40)
    void setTrsh(uint16_t trsh) {
        _trsh = trsh;
    }
    
    // ========================== VOLUME ==========================
    // установить период фильтрации громкости (умолч 20)
    void setVolDt(uint8_t dt) {
        volF.dt = dt;
    }
    
    // установить коэффициент плавности громкости 0-31 (умолч 25)
    void setVolK(uint8_t vk) {
        volF.k = vk;
    }
    
    // получить громкость в пределах setVolMin.. setVolMax
    uint16_t getVol() {
        return volF.fil;
    }
    
    // установить минимальную величину громкости (умолч 0)
    void setVolMin(uint8_t vol) {
        _volMin = vol;
    }
    
    // установить максимальную величину громкости (умолч 100)
    void setVolMax(uint8_t vol) {
        _volMax = vol;
    }
    
    // ========================= AMPLITUDE =========================
    // установить период фильтрации амплитудных огибающих
    void setAmpliDt(uint8_t dt) {
        ampF.dt = dt;
    }
    
    // установить коэффициент плавности амплитуды 0-31 (умолч 31)
    void setAmpliK(uint8_t rk) {
        ampF.k = rk;
    }
    
    // получить текущее значение огибающей минимумов (с v1.5 - 0)
    uint16_t getMin() {
        return 0;
    }

    // получить текущее значение огибающей максимумов
    uint16_t getMax() {
        return ampF.fil;
    }
    
    // =========================== PULSE ===========================
    // верхний порог срабатывания пульса (по шкале громкости)
    void setPulseMax(uint8_t maxV) {
        _pulseMax = maxV;
    }
    
    // нижний порог перезагрузки пульса (по шкале громкости)
    void setPulseMin(uint8_t minV) {
        _pulseMin = minV;
    }
    
    // таймаут пульса, мс
    void setPulseTimeout(uint16_t tout) {
        _pulseTout = tout;
    }
    
    // резкий скачок громкости (true)
    bool pulse() {
        return _pulse;
    }
    
    // ========================== RAW DATA ===========================
    // получить значение сырого сигнала за выборку
    uint16_t getRaw() {
        return _raw;
    }
    
    // получить порог громкости в единицах АЦП
    uint16_t getTrsh() {
        return _trsh;
    }
    
    // ========================= DEPRECATED =========================
    void setPeriod(__attribute__((unused)) uint16_t v) {}   // установить период между выборками
    uint16_t getRawMax() { return _raw; }    // получить максимальное значение сырого сигнала за выборку
    bool getPulse() { return pulse(); }
    void setPulseTrsh(uint16_t trsh) { setPulseMax(trsh); }
    FFilter volF, ampF;
    
private:
    int8_t _pin = -1;
    uint16_t _dt = 500, _trsh = 40;
    uint8_t _window = 20, _count = 0;
    uint8_t _volMin = 0, _volMax = 100;
    uint32_t _tmrPulse = 0, _tmrDt = 0;
    uint16_t _min = 60000, _max = 0, _ampli = 0, _raw = 0;
    
    uint8_t _pulseMax = 80, _pulseMin = 20;
    uint16_t _pulseTout = 100;
    bool _pulse = 0, _pulseState = 0;
};
#endif
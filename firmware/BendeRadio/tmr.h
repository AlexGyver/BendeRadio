#pragma once
#include <Arduino.h>

class Tmr {
   public:
    Tmr() {}
    Tmr(uint16_t ms) {
        start(ms);
    }

    void start(uint16_t ms) {
        _prd = ms;
        if (_prd) start();
    }
    void start() {
        if (!_prd) return;
        _tmr = millis();
        if (!_tmr) _tmr = 1;
    }
    void timerMode(bool mode) {
        _mode = mode;
    }
    void stop() {
        _tmr = 0;
    }
    bool state() {
        return _tmr;
    }
    bool tick() {
        return (_tmr && millis() - _tmr >= _prd) ? ((_mode ? stop() : start()), true) : false;
    }
    operator bool() {
        return tick();
    }

   private:
    uint32_t _tmr = 0, _prd = 0;
    bool _mode = 0;
};
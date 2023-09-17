#pragma once

class Ampli {
   public:
    bool tick(uint16_t val) {
        if (_tmin > val) _tmin = val;
        if (_tmax < val) _tmax = val;
        if (++_count >= _window) {
            _count = 0;
            _max = _tmax;
            _min = _tmin;
            _tmin = 65000;
            _tmax = 0;
            return 1;
        }
        return 0;
    }
    uint16_t min() {
        return _min;
    }
    uint16_t max() {
        return _max;
    }
    uint16_t ampli() {
        return _max - _min;
    }

   private:
    uint16_t _tmin = 65000, _tmax = 0;
    uint16_t _min = 65000, _max = 0;
    uint16_t _window = 30, _count = 0;
};

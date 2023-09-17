#pragma once
#include <Arduino.h>

#include "GyverGFX.h"

class RunningGFX {
   public:
    RunningGFX(GyverGFX* gfx) : _gfx(gfx) {}

   private:
    GyverGFX* _gfx;
};
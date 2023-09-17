#pragma once
#include <Arduino.h>
#include <Audio.h>

#include "config.h"

struct Data {
    bool state = 0;
    int8_t vol = 10;
    int8_t bright_eyes = 5;
    int8_t bright_mouth = 2;
    uint16_t trsh = 50;
    uint8_t mode = 0;
    int8_t station = 0;
};

extern Audio audio;
extern const char* reconnect;

void change_state();
void anim_search();
void core0(void *p);
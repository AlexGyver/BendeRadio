#include <Arduino.h>

#include <GyverMAX7219.h>
#include <RunningGFX.h>

MAX7219<4, 1, 5> mtrx;  // одна матрица (1х1), пин CS на D5
RunningGFX run1(&mtrx);
RunningGFX run2(&mtrx);
const char pstr_g[] PROGMEM = "Global pgm string";

void setup() {
    mtrx.begin();       // запускаем
    mtrx.setBright(5);  // яркость 0..15

    run1.setSpeed(10);
    run2.setSpeed(10);
    run1.setText("hello");
    run2.setText_P(pstr_g);
    run1.setWindow(0, 15, 0);
    run2.setWindow(16, 31, 0);
    run1.start();
    run2.start();
}

void loop() {
    run1.tick();
    run2.tick();
}
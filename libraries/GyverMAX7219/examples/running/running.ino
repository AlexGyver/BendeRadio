#include <Arduino.h>
// полное описание библиотеки бегущей строки смотри здесь
// https://github.com/GyverLibs/GyverGFX

#include <GyverMAX7219.h>
#include <RunningGFX.h>

MAX7219<4, 1, 5> mtrx;
RunningGFX run(&mtrx);
const char pstr_g[] PROGMEM = "Global pgm string";

void setup() {
    mtrx.begin();       // запускаем
    mtrx.setBright(5);  // яркость 0..15

    run.setSpeed(15);
    // run.setText("hello");
    run.setText_P(pstr_g);
    // run.setWindow(5, 15, 1);
    run.start();
}

void loop() {
  run.tick();
}

#include <GyverMAX7219.h>
MAX7219<2, 1, 5> mtrx;  // матрица (2х1), пин CS на D5

void setup() {
    mtrx.begin();       // запускаем
    mtrx.setBright(5);  // яркость 0..15

    // встроенные иконки
    mtrx.drawBytes_P(GFX_icons::alarm, 8);
    mtrx.drawBytes_P(GFX_icons::email, 8);
    mtrx.update();
}

void loop() {
}

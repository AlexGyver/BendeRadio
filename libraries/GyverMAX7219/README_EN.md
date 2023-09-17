This is an automatic translation, may be incorrect in some places. See sources and examples!

# GyverMAX7219
The sharpest library for MAX7219 matrices in the wild west
- Inherits graphics from GyverGFX
- Connecting matrices in a zigzag pattern
- Hardware and software SPI
- Incredible optimization

### Compatibility
Compatible with all Arduino platforms (using Arduino functions)

## Content
- [Install](#install)
- [Initialization](#init)
- [Usage](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- Requires [GyverGFX](https://github.com/GyverLibs/GyverGFX) to work
- The library can be found under the name **GyverMAX7219** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download library](https://github.com/GyverLibs/GyverMAX7219/archive/refs/heads/main.zip) .zip archive for manual installation:
    - Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE% D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Initialization
```cpp
MAX7219 < W, H, CS > mtrx; // connect to hardware SPI
// example: UNO / Nano (CLK - D13, DI - D11, CS - any pin)

MAX7219 < W, H, CS, DATA, CLK > mtrx; // connect to any pins
// W and H - the number of MATRIXes horizontally and vertically
// CS, DATA, CLK - pin numbers
```

<a id="usage"></a>
## Usage
```cpp
voidbegin(); // run
void setRotation(uint8_t rotation); // rotate matrices (0, 1, 2, 3 by 90 degrees clockwise)
void setBright(byte value); // set brightness [0-15]
void setPower(bool value); // switch power
void clear(); // clear
voidfill(); // fill
void fillByte(byte data); // fill with byte
void dot(int x, int y, byte fill = 1); // set point
bool get(int x, int y); // get point
void update(); // update

// and also inherits everything from GyverGFX:
void dot(int x, int y, uint8_t fill = 1); // point, fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void fastLineH(int y, int x0, int x1, uint8_t fill = 1); // vertical line, fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void fastLineV(int x, int y0, int y1, uint8_t fill = 1); // horizontal line, fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void line(int x0, int y0, int x1, int y1, uint8_t fill = 1); // line, fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void rect(int x0, int y0, int x1, int y1, uint8_t fill = 1); // rectangle, fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void roundRect(int x0, int y0, int x1, int y1, uint8_t fill = 1); // rounded rectangle, fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void circle(int x, int y, int radius, uint8_t fill = 1); // circle, fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void bezier(uint8_t* arr, uint8_t size, uint8_t dense, uint8_t fill = 1); // bezier curve
void bezier16(int* arr, uint8_t size, uint8_t dense, uint8_t fill = 1);// Bezier curve 16 bit. fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void drawBitmap(int x, int y, const uint8_t *frame, int width, int height, uint8_t invert = 0, byte mode = 0); // bitmap
void setCursor(int x, int y); // set cursor
void setScale(uint8_tscale); // text scale
void invertText(bool inv); // invert text
void autoPrintln(bool mode); // automatic line break
void textDisplayMode(bool mode); // text output mode GFX_ADD/GFX_REPLACE

// and from Print.h
// print/println any data type
```

<a id="example"></a>
## Example
See **examples** for other examples!
```cpp
#include <GyverMAX7219.h>
MAX7219 < 1, 1, 5 > mtrx; // one matrix (1x1), pin CS on D5

void setup() {
  mtrx.begin(); // run
  mtrx.setBright(5); // brightness 0..15
  //mtrx.rotate(1); // you can rotate 0..3, 90 degrees clockwise

  mtrx.dot(0, 0); // pixel at coordinates 0,0
  mtrx update(); // show
  delay(1000);
  mtrx.clear();

  // criss-cross lines
  mtrx.line(0, 0, 7, 7); // (x0, y0, x1, y1)
  mtrx.line(7, 0, 0, 7);
  mtrx update();
  delay(1000);
  mtrx.clear();

  // a circle
  mtrx.circle(3, 3, 3, GFX_FILL); // x, y, radius, fill
  mtrx update();
  delay(1000);
  mtrx.clear();

  // circle
  mtrx.circle(3, 3, 3, GFX_STROKE);
  mtrx update();
  delay(1000);
  mtrx.clear();

  // see the documentation for the rest of the geometry
}

void loop() {
}
```

<a id="versions"></a>
## Versions
- v1.0 - release
- v1.1 - SPI optimized
- v1.2 - redesigned FastIO
- v1.2.1 - fixed bug in SPI (since 1.2)
- v1.2.2 - removed FastIO
- v1.3 - minor improvements and optimization, added matrix rotation

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!
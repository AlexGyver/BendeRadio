This is an automatic translation, may be incorrect in some places. See sources and examples!

# GyverGFX
Lightweight 2D Graphics Library for Displays and Matrices
- Points
- Lines
- Rectangles
- Rounded rectangles
- Circles
- Bezier curve
- Bitmap
- Text output (Russian, English) of several sizes

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
- The library can be found by the name **GyverGFX** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download library](https://github.com/GyverLibs/GyverGFX/archive/refs/heads/main.zip) .zip archive for manual installation:
    - Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE% D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Initialization
```cppGyverGFX(int x, int y); // indicating the size of the "screen"
```

<a id="usage"></a>
## Usage
```cpp
//fill:
// GFX_CLEAR - clear
// GFX_FILL - fill the shape
// GFX_STROKE - stroke the shape

virtual void dot(int x, int y, uint8_t fill = 1); // dot
virtual void fastLineH(int y, int x0, int x1, uint8_t fill = 1); // vertical line
virtual void fastLineV(int x, int y0, int y1, uint8_t fill = 1); // horizontal line
virtual void line(int x0, int y0, int x1, int y1, uint8_t fill = 1); // line
virtual void rect(int x0, int y0, int x1, int y1, uint8_t fill = 1); // rectangle
virtual void roundRect(int x0, int y0, int x1, int y1, uint8_t fill = 1); // rounded rectangle
virtual void circle(int x, int y, int radius, uint8_t fill = 1); // circle
virtual void bezier(uint8_t* arr, uint8_t size, uint8_t dense, uint8_t fill = 1); // bezier curve
virtual void bezier16(int* arr, uint8_t size, uint8_t dense, uint8_t fill = 1); // bezier curve 16 bit. fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
virtual void drawBitmap(int x, int y, const uint8_t *frame, int width, int height, uint8_t invert = 0, byte mode = 0); // bitmap

void setCursor(int x, int y); // set cursor
void setScale(uint8_tscale); // text scale
void invertText(bool inv); // invert text
void autoPrintln(bool mode); // automatic line break
void textDisplayMode(bool mode); // text output mode GFX_ADD/GFX_REPLACE
```

<a id="example"></a>
## Example
```cpp
// example of inheritance into a class
class MAX7219 : public GyverGFX {
public:
    MAX7219() : GyverGFX(width * 8, height * 8) {
        begin();
    }
```

<a id="versions"></a>
## Versions
- v1.0
- v1.1 - memory optimization

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
BeeThe Library is open for revision and your **Pull Request**'s!
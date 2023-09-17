This is an automatic translation, may be incorrect in some places. See sources and examples!

# VolAnalyzer
Library for amplitude sound analysis on Arduino
- Cunning adaptive algorithm
- Built-in filters for a smooth flow of values
- Receiving volume in the specified range regardless of the input signal range
- Receiving a signal for sudden changes in sound
- Work in the virtual analyzer mode (without reference to the ADC MK)

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
- The library can be found by the name **VolAnalyzer** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download library](https://github.com/GyverLibs/VolAnalyzer/archive/refs/heads/main.zip) .zip archive for manual installation:
    - Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%%D0%B5%D0%BA)

<a id="init"></a>
## Initialization
```cpp
VolAnalyzer analyzer(A0); // specify pin
VolAnalyzer analyzer; // "virtual" parser
```

<a id="usage"></a>
## Usage
```cpp
// analyzer ticker. Returns true when the current analysis ends. call more often
bool tick(); // polls the pin specified in setPin
bool tick(int thisRead); // takes the specified value

// analysis settings
void setPin(int pin); // specify ADC pin
void setDt(int dt); // set the time between ADC polls, µs (default 500)
void setWindow(int window); // set selection window width (default 20)
void setTrsh(int trsh); // set volume threshold in raw ADC units (default 40)

// amplitude
void setAmpliDt(int ampliDt); // set filtering period for amplitude envelopes, ms (default 150)
void setAmpliK(byte k); // set filtering coefficient of amplitude envelopes 0-31 (default 30)

// volume
void setVolDt(int volDt); // set volume filtering period (default 20)
void setVolK(byte k); // set volume filter factor 0-31 (default 25)
void setVolMin(int scale); // set minimum volume value (default 0)
void setVolMax(int ​​scale); // set the maximum volume value (default 100)

// pulse
void setPulseMax(int ​​maxV); // upper pulse threshold (according to the volume scale)
void setPulseMin(int minV); // lower threshold for reloading the pulse (according to the volume scale)
void setPulseTimeout(int tout); // pulse timeout, ms

// get values
int getVol(); // volume within setVolMin.. setVolMax
boolpulse(); // loud volume jump
int getMin(); // current value of the lows envelope
int getMax(); // current value of the maximum envelope
int getRaw();// raw signal value
```

<a id="example"></a>
## Example
```cpp
// amplitude analysis of sound

#include "VolAnalyzer.h"
VolAnalyzer analyzer(A0);


void setup() {
  Serial.begin(115200);
  analyzer.setVolK(20);
  analyzer.setTrsh(10);
  analyzer.setVolMin(10);
  analyzer.setVolMax(100);
}

void loop() {
  if (analyzer.tick()) {
    //Serial.print(analyzer.getVol());
    //Serial.print(',');
    Serial.print(analyzer.getRaw());
    Serial print(',');
    Serial.print(analyzer.getMin());
    Serial print(',');
    Serial.println(analyzer.getMax());
  }
}
```

<a id="versions"></a>
## Versions
- v1.0
- v1.1 - sharper drop on audio cutout
- v1.2 - +compatibility. Returned the smooth fall of the sound
- v1.3 - new pulse handler
- v1.4 - algorithm improvement
- v1.5 - strong facilitation and algorithm improvement
- v1.6 - sharper reaction to sound
- v1.7 - no division by 0 in map
- v1.8 - now works with 12bit ADC

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!
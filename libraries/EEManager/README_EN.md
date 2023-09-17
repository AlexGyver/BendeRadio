This is an automatic translation, may be incorrect in some places. See sources and examples!

# EEManager
EEPROM manager - library to reduce memory wear
- Delayed recording (updating) by timer
- Works on standard EEPROM.h
- Built-in first entry key mechanism

### Compatibility
Compatible with all Arduino platforms (using Arduino functions)
- For esp8266 don't forget to call EEPROM.begin(size)!

## Content
- [Install](#install)
- [Initialization](#init)
- [Usage](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found by the name **EEManager** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download library](https://github.com/GyverLibs/EEManager/archive/refs/heads/main.zip) .zip archive for manual installation:
    - Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE% D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Initialization
```cpp
// pass our variable of any type (actually its address) - structure, array, whatever
EEManager memory(data);

// you can set the update timeout, by default. 5 seconds (5000ms)
EEManager memory(data, 1000);
```

<a id="usage"></a>
## Usage
```cpp
// Start work, read data into a variable.
// Accepts the date storage start address and key
// Returns:
// 0 - key matched, data read from eprom
// 1 - the key did not match (first run), data is written to eprom
// 2 - error, there is not enough space in eprom
uint8_t begin(uint8_t addr, uint8_t key);

void setTimeout(uint16_t tout = 5000); // change timeout
void updateNow(); // update data in herprom now
void update(); // delay update and reset timer
bool tick(); // update ticker
void reset(); // reset startup key. When reloading (or calling begin), standard data will be written
uint16_t dataSize(); // get data size
uint16_t blockSize(); // get the size of the entire block (data + key)
uint16_t startAddr(); // get the address of the first byte in the block
uint16_tendAddr(); // get the address of the last byte in the block (including the key)
```

<a id="example"></a>
## Example
```cpp
// data storage structure
struct Data {
  charval = 'H';
  char str[15] = "hello kitty!";
};
data data; // variable with which we work in the program

#include <EEManager.h> // include lib
EEManager memory(data); // pass our variable (actually its address)

// you can set the update timeout, by default. 5 seconds (5000ms)
//EEManager memory(data, 1000);

void setup() {
  Serial.begin(9600);
  // for esp8266 don't forget to call EEPROM.begin(size)!
  /*
    We start the manager, we transfer:
    - Starting address in memory for writing the date
    - Storage key (0..255) or symbol
  */
  byte stat = memory.begin(0, 'b');

  /*
    Return codes:
    0 - key matched, data read from eprom
    1 - the key did not match (the firststart), data recorded in eprom
    2 - error, there is not enough space in eprom
  */
  Serial println(stat);

  // output the entire contents of the memory block to the port, including the key
  for (uint16_t i = memory.startAddr(); i < memory.endAddr() + 1; i++)
    Serial.write(EEPROM.read(i));
  Serial.println();
  
  data.val = 'X'; // change the data in the variable
  memory update(); // defer update

  // note: if you want to create another block next in memory -
  // run it at memory.endAddr() + 1, this is the first
  // free byte after the first block
}

void loop() {
  // we call tick() in the loop, it will update according to the timer
  // function will return true when this happens
  if (memory.tick()) Serial.println("Updated!");

  // output the entire contents of the memory block to the port, including the key
  for (uint16_t i = memory.startAddr(); i < memory.endAddr() + 1; i++)
    Serial.write(EEPROM.read(i));
  Serial.println();

  // reset timer for 10 seconds: after 10 seconds
  // the data key will be reset, and on the next run
  // default data will be written to eprom (what is in the structure label)
  static uint32_t tmr;
  if (millis() - tmr >= 10000) {
    memory reset();
    Serial.println("reset");
    for(;;);
  }
  
  delay(1000); // chilim
}
```

<a id="versions"></a>
## Versions
- v1.0
- v1.1 - begin return codes changed
- v1.2 - added nextAddr()
- v1.2.1 - esp32 support

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!
# TinyIR
Samsung IR Protocol decoder written for ATTiny13 and MicroCore.

### [Download](https://github.com/Ivan-Alone/TinyIR/raw/main/TinyIR.zip)

## Information and requirements:
Examples of resulting sizes:
- Empty MicroCore project: 142 bytes memory, 4 bytes RAM
- MicroCore project with just included TinyIR: 220 bytes memory, 20 bytes RAM
- MicroCore project with TinyIR and checking 1 IR code: 798 bytes memory, 20 bytes RAM
- MicroCore project with TinyIR and checking 4 IR codes: 862 bytes memory, 20 bytes RAM

You **must** enable **micros()** in your project. Clocking frequency is not important, but must be more than 38 kHz (carrier frequency of IR Sensor).

This library using hardware interrupt **INT0** on **PB1** pin on **ATTiny13**, you CAN'T connect IR Receiver to other pin.

## Sensor Connection
![Connection Diagram](https://raw.githubusercontent.com/Ivan-Alone/TinyIR/main/tinyir_connection.png)

## Functions: 
```C++
void IR_Init();
```

Must be called in setup() function. Initialize **INT0** on **PB1**, thats doing all data receiving and processing functions.

```C++
bool IR_CheckCode(uint32_t code);
```

Checking receivced code. Returns **TRUE** if code was received. Can be used in loop, in constuction like:

```C++
if (IR_CheckCode(0x7070FF00) {
    // Doing some stuff when 0x7070FF00 code received
}
```

## Configuration
You can reduce memory usage used by this library! We have 2 configuration constants. All those constants **MUST** be defined **BEFORE** including TinyIR. Take care - you CAN'T edit this settigs in your code logic - it's *hard* settigns, that applies on compiling stage.

```C++
#define TinyIR_NO_ACCURATE_DECODING
```

Disables accurate decoding. Samsung remote control sends impulse ~1640 μS long for "1" bit value, and ~500 μS long for "0". Using disabled accurate decoding we don't check lenght of "0" impulse (checking just lenght of "1" impulse), so we can reduce memory usage for 20-22 bytes in ROM.

```C++
#define TinyIR_DISABLE_REPEAT
```

Some remote control that uses Samsung protocol doesn't resend code if button is hold. Those RC sends some "repeat code", that can be decoded in 0x0 or 0x1. So, if we don't need hold tracking, or our RC sends real codes if button is hold, we can disable decoding of repeat codes, that can reduce memory usage for 112-114 bytes in ROM.

Remember - some RC sends "repeat codes", some sends real code (RC from Samsung TV does that, f.e.)

## Example

```C++
// Disable accurate decoding
#define TinyIR_NO_ACCURATE_DECODING 
// Disable repeat decoding
#define TinyIR_DISABLE_REPEAT

#include <TinyIR.h>


int8_t horisontalOffset = 0;
int8_t verticalOffset = 0;

#define KEY_UP    0x7070FF00
#define KEY_DOWN  0x707055AA
#define KEY_LEFT  0x70700000
#define KEY_RIGHT 0x70700FF0


void setup() {
  IR_Init();
}

void loop() {
  if (IR_CheckCode(KEY_UP)) { // If KEY_UP pressed, do things: 
    verticalOffset++;
  }
  if (IR_CheckCode(KEY_DOWN)) {
    verticalOffset--;
  }
  if (IR_CheckCode(KEY_LEFT)) {
    horisontalOffset--;
  }
  if (IR_CheckCode(KEY_RIGHT)) {
    horisontalOffset++;
  }
}
```

This code will take: 784 bytes in ROM, 18 bytes RAM. Very *tiny*, I think.

## Resume
I hope this library will useful. I can't find good libraries/examples of IR for ATTiny before, so I wrote own.

Enjoy ;)

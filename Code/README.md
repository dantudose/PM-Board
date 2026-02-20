# PM-Board Test

PlatformIO project for validating the PM-board. The firmware brings up the board peripherals, reads sensors over I2C/ADC, and renders live values on the TFT display while driving status LEDs and the speaker.

## What the firmware does

- Runs an RGB LED self-test sequence on boot.
- Initializes the SD card interface and reports the card type over serial and the 16x2 LCD.
- Plays a short tone sequence on the speaker.
- Powers the I2C domain and brings up:
  - LSM9DS0 IMU (accel/gyro/mag)
  - MPL3115A2 barometer (altitude + temperature)
- Updates a 1.8" ST7735 TFT (Ucglib) with live readings and bar graphs.
- Reads analog channels for temperature, light, and a button ladder.
- Uses two digital buttons to toggle the RGB status LED colors.

## Board signals and pins


| Signal | Pin | Notes |
| --- | --- | --- |
| Speaker | 12 | Plays startup tune |
| GLCD backlight | 31 | TFT backlight control |
| ADC temp | A0 | Analog input |
| ADC light | A1 | Analog input |
| ADC buttons | A5 | Analog button ladder |
| I2C enable | 30 | Powers I2C domain |
| LCD RS | 16 | 16x2 LCD |
| LCD EN | 18 | 16x2 LCD |
| LCD RW | 17 | 16x2 LCD (held LOW) |
| LCD D4..D7 | 19..22 | 4-bit LCD bus |
| LCD backlight | 23 | 16x2 LCD |
| Button up | 14 | Active LOW |
| Button down | 2 | Active LOW |
| LED red | 13 | Active LOW |
| LED green | 15 | Active LOW |
| LED blue | 3 | Active LOW |
| SD chip select | 26 | Uses hardware SPI |
| TFT DC (A0) | 4 | Uses hardware SPI |
| TFT CS | 0 | Uses hardware SPI |
| TFT RESET | 1 | Uses hardware SPI |

I2C devices and addresses:

- LSM9DS0: 0x6B (gyro), 0x1D (xm), expected WHO_AM_I 0x49D4
- MPL3115A2: 0x60, expected WHO_AM_I 0xC4

## Build

```sh
platformio run -e ATmega324P
```

## Dependencies

Declared in `platformio.ini`:

- Ucglib
- SparkFun LSM9DS0 Arduino Library
- LiquidCrystal

## Configuration notes

- IMU output units: toggle `PRINT_CALCULATED` or `PRINT_RAW` in `src/main.cpp`.
- Display orientation is set to 180 degrees in `initDisplay()`.
- Barometer is configured in altimeter mode with oversample rate 128.

## Serial output

The firmware uses 115200 baud for boot logs and sensor presence checks.

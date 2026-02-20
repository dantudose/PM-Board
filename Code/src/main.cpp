#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

#include <LiquidCrystal.h>
#include <SFE_LSM9DS0.h>

#include "Ucglib.h"
#include "pitches.h"

// Board pin map and device configuration.
namespace Pins {
constexpr uint8_t kSpeaker = 12;
constexpr uint8_t kGlcdBacklight = 31;
constexpr uint8_t kAdcTemp = A0;
constexpr uint8_t kAdcLight = A1;
constexpr uint8_t kAdcButtons = A5;
constexpr uint8_t kI2cEnable = 30;
constexpr uint8_t kLcdRs = 16;
constexpr uint8_t kLcdEn = 18;
constexpr uint8_t kLcdRw = 17;
constexpr uint8_t kLcdD4 = 19;
constexpr uint8_t kLcdD5 = 20;
constexpr uint8_t kLcdD6 = 21;
constexpr uint8_t kLcdD7 = 22;
constexpr uint8_t kLcdBacklight = 23;
constexpr uint8_t kButtonUp = 14;
constexpr uint8_t kButtonDown = 2;
constexpr uint8_t kLedRed = 13;
constexpr uint8_t kLedGreen = 15;
constexpr uint8_t kLedBlue = 3;
constexpr uint8_t kSdChipSelect = 26;
constexpr uint8_t kTftDc = 4;    // A0
constexpr uint8_t kTftCs = 0;
constexpr uint8_t kTftReset = 1;
}  // namespace Pins

namespace ImuConfig {
constexpr uint8_t kXmAddr = 0x1D;
constexpr uint8_t kGAddr = 0x6B;
constexpr uint16_t kWhoAmIExpected = 0x49D4;
}  // namespace ImuConfig

namespace BaroConfig {
constexpr uint8_t kAddress = 0x60;
constexpr uint8_t kRegStatus = 0x00;
constexpr uint8_t kRegOutPMsb = 0x01;
constexpr uint8_t kRegOutTMsb = 0x04;
constexpr uint8_t kRegWhoAmI = 0x0C;
constexpr uint8_t kRegPtDataCfg = 0x13;
constexpr uint8_t kRegCtrl1 = 0x26;
constexpr uint8_t kWhoAmIExpected = 196;
constexpr float kInvalidReading = -999.0f;
}  // namespace BaroConfig

constexpr uint16_t kAdcMax = 1024;
constexpr uint8_t kDisplayWidth = 128;

// Global devices.
LiquidCrystal lcd(
    Pins::kLcdRs,
    Pins::kLcdEn,
    Pins::kLcdD4,
    Pins::kLcdD5,
    Pins::kLcdD6,
    Pins::kLcdD7);
Ucglib_ST7735_18x128x160_HWSPI ucg(Pins::kTftDc, Pins::kTftCs, Pins::kTftReset);
LSM9DS0 imu(MODE_I2C, ImuConfig::kGAddr, ImuConfig::kXmAddr);
Sd2Card sdCard;

struct SensorReadings {
  int adcTemp = 0;
  int adcLight = 0;
  int adcButtons = 0;
  float baroAltitude = BaroConfig::kInvalidReading;
  float baroTemp = BaroConfig::kInvalidReading;
  float accelX = 0.0f;
  float accelY = 0.0f;
  float accelZ = 0.0f;
  float gyroX = 0.0f;
  float gyroY = 0.0f;
  float gyroZ = 0.0f;
  float magX = 0.0f;
  float magY = 0.0f;
  float magZ = 0.0f;
};

SensorReadings readings;
bool imuOnline = false;
bool baroOnline = false;

// Notes in the melody.
const uint16_t kMelody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
// Note durations: 4 = quarter note, 8 = eighth note, etc.
const uint8_t kNoteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};
constexpr size_t kMelodyLength = sizeof(kMelody) / sizeof(kMelody[0]);

// Build-time option for IMU output units.
#define PRINT_CALCULATED
// #define PRINT_RAW

// Forward declarations.
static void initPins();
static void initPortC();
static void setI2cPower(bool on);
static void setGlcdBacklight(bool on);
static void setStatusLed(bool redOn, bool greenOn, bool blueOn);
static void runLedSelfTest();
static void playTune();
static bool initSdCard();
static bool initImu();
static bool initBarometer();
static void initDisplay();
static void drawStaticLabels();
static void readAdc(SensorReadings &out);
static void readImu(SensorReadings &out);
static void readBarometer(SensorReadings &out);
static float readAltitudeMeters();
static float readTemperatureC();
static bool waitForBaroStatus(uint8_t mask);
static void setModeAltimeter();
static void setOversampleRate(uint8_t sampleRate);
static void toggleOneShot();
static void enableEventFlags();
static uint8_t i2cRead(uint8_t regAddr);
static void i2cWrite(uint8_t regAddr, uint8_t value);
static void updateDisplay(const SensorReadings &data);
static void setTextColor(bool ok);
static uint8_t clampBarLength(float length);
static void drawBar(uint8_t y, float length);
static void drawValueRow(float value,
                         uint8_t width,
                         uint8_t precision,
                         uint8_t valueX,
                         uint8_t valueY,
                         uint8_t lineY,
                         float barLength,
                         bool okColor);
static void updateStatusLeds();

void setup() {
  Serial.begin(115200);

  initPins();
  runLedSelfTest();

  setI2cPower(false);
  initPortC();

  lcd.begin(16, 2);
  initSdCard();

  playTune();

  setI2cPower(true);
  Wire.begin();

  imuOnline = initImu();
  baroOnline = initBarometer();

  initDisplay();
  drawStaticLabels();
}

void loop() {
  readAdc(readings);
  readBarometer(readings);
  readImu(readings);

  updateDisplay(readings);
  updateStatusLeds();
}

static void initPins() {
  pinMode(Pins::kButtonUp, INPUT_PULLUP);
  pinMode(Pins::kButtonDown, INPUT_PULLUP);

  pinMode(Pins::kLedRed, OUTPUT);
  pinMode(Pins::kLedGreen, OUTPUT);
  pinMode(Pins::kLedBlue, OUTPUT);

  pinMode(Pins::kSpeaker, OUTPUT);
  pinMode(Pins::kI2cEnable, OUTPUT);
  pinMode(Pins::kGlcdBacklight, OUTPUT);

  pinMode(Pins::kLcdRw, OUTPUT);
  digitalWrite(Pins::kLcdRw, LOW);
  pinMode(Pins::kLcdBacklight, OUTPUT);
  digitalWrite(Pins::kLcdBacklight, HIGH);
}

static void initPortC() {
  // Board-specific: drive Port C lines to a known state.
  DDRC = 0xFF;
  PORTC = 0xFF;
  delay(1000);
  PORTC = 0x00;
}

static void setI2cPower(bool on) {
  digitalWrite(Pins::kI2cEnable, on ? HIGH : LOW);
}

static void setGlcdBacklight(bool on) {
  digitalWrite(Pins::kGlcdBacklight, on ? HIGH : LOW);
}

static void setStatusLed(bool redOn, bool greenOn, bool blueOn) {
  // LEDs are active-low on this board.
  digitalWrite(Pins::kLedRed, redOn ? LOW : HIGH);
  digitalWrite(Pins::kLedGreen, greenOn ? LOW : HIGH);
  digitalWrite(Pins::kLedBlue, blueOn ? LOW : HIGH);
}

static void runLedSelfTest() {
  setStatusLed(false, false, false);
  delay(300);
  setStatusLed(true, false, false);
  delay(300);
  setStatusLed(false, true, false);
  delay(300);
  setStatusLed(false, false, true);
  delay(300);
  setStatusLed(false, false, false);
  delay(300);
}

static void playTune() {
  for (size_t i = 0; i < kMelodyLength; ++i) {
    int noteDuration = 1000 / kNoteDurations[i];
    tone(Pins::kSpeaker, kMelody[i], noteDuration);

    int pauseBetweenNotes = static_cast<int>(noteDuration * 1.30f);
    delay(pauseBetweenNotes);

    noTone(Pins::kSpeaker);
  }
}

static bool initSdCard() {
  if (!sdCard.init(SPI_HALF_SPEED, Pins::kSdChipSelect)) {
    Serial.println("SD init failed");
    Serial.println("- Is a card inserted?");
    Serial.println("- Is the wiring correct?");
    Serial.println("- Is chip select correct?");
    lcd.print("SD Init Fail");
    return false;
  }

  Serial.println("SD card initialized");
  Serial.print("Card type: ");
  switch (sdCard.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
      break;
  }

  lcd.print("SD Card Found");
  lcd.setCursor(0, 1);
  lcd.print("FAT");
  delay(500);
  return true;
}

static bool initImu() {
  uint16_t status = imu.begin();
  Serial.print("LSM9DS0 WHO_AM_I returned: 0x");
  Serial.println(status, HEX);
  Serial.println("Expected: 0x49D4");
  return status == ImuConfig::kWhoAmIExpected;
}

static bool initBarometer() {
  if (i2cRead(BaroConfig::kRegWhoAmI) != BaroConfig::kWhoAmIExpected) {
    Serial.println("MPL3115A2 not detected");
    readings.baroAltitude = BaroConfig::kInvalidReading;
    readings.baroTemp = BaroConfig::kInvalidReading;
    return false;
  }

  Serial.println("MPL3115A2 online");

  setModeAltimeter();
  setOversampleRate(7);  // 128 samples
  enableEventFlags();
  return true;
}

static void initDisplay() {
  setGlcdBacklight(true);
  ucg.begin(UCG_FONT_MODE_SOLID);
  ucg.clearScreen();
  ucg.setRotate180();
  ucg.setFont(ucg_font_courR08_mr);
  ucg.setColor(0, 255, 255, 255);  // White text for static labels.
  ucg.setColor(1, 0, 0, 0);        // Black background.
}

static void drawStaticLabels() {
  ucg.setPrintPos(0, 8);
  ucg.print("Temp:");
  ucg.setPrintPos(0, 18);
  ucg.print("Light:");
  ucg.setPrintPos(0, 28);
  ucg.print("Buttons:");
  ucg.setPrintPos(0, 38);
  ucg.print("Baro Altitude:");
  ucg.setPrintPos(0, 48);
  ucg.print("Baro Temp:");
  ucg.setPrintPos(0, 58);
  ucg.print("Accel. X:");
  ucg.setPrintPos(0, 68);
  ucg.print("Accel. Y:");
  ucg.setPrintPos(0, 78);
  ucg.print("Accel. Z:");
  ucg.setPrintPos(0, 88);
  ucg.print("Gyro X:");
  ucg.setPrintPos(0, 98);
  ucg.print("Gyro Y:");
  ucg.setPrintPos(0, 108);
  ucg.print("Gyro Z:");
  ucg.setPrintPos(0, 118);
  ucg.print("Mag X:");
  ucg.setPrintPos(0, 128);
  ucg.print("Mag Y:");
  ucg.setPrintPos(0, 138);
  ucg.print("Mag Z:");
}

static void readAdc(SensorReadings &out) {
  out.adcLight = kAdcMax - analogRead(Pins::kAdcLight);
  out.adcTemp = analogRead(Pins::kAdcTemp);
  out.adcButtons = analogRead(Pins::kAdcButtons);
}

static void readImu(SensorReadings &out) {
  out.accelX = 0.0f;
  out.accelY = 0.0f;
  out.accelZ = 0.0f;
  out.gyroX = 0.0f;
  out.gyroY = 0.0f;
  out.gyroZ = 0.0f;
  out.magX = 0.0f;
  out.magY = 0.0f;
  out.magZ = 0.0f;

  if (!imuOnline) {
    return;
  }

  imu.readAccel();
#ifdef PRINT_CALCULATED
  out.accelX = imu.calcAccel(imu.ax);
  out.accelY = imu.calcAccel(imu.ay);
  out.accelZ = imu.calcAccel(imu.az);
#elif defined(PRINT_RAW)
  out.accelX = imu.ax;
  out.accelY = imu.ay;
  out.accelZ = imu.az;
#endif

  imu.readGyro();
#ifdef PRINT_CALCULATED
  out.gyroX = imu.calcGyro(imu.gx);
  out.gyroY = imu.calcGyro(imu.gy);
  out.gyroZ = imu.calcGyro(imu.gz);
#elif defined(PRINT_RAW)
  out.gyroX = imu.gx;
  out.gyroY = imu.gy;
  out.gyroZ = imu.gz;
#endif

  imu.readMag();
#ifdef PRINT_CALCULATED
  out.magX = imu.calcMag(imu.mx);
  out.magY = imu.calcMag(imu.my);
  out.magZ = imu.calcMag(imu.mz);
#elif defined(PRINT_RAW)
  out.magX = imu.mx;
  out.magY = imu.my;
  out.magZ = imu.mz;
#endif
}

static void readBarometer(SensorReadings &out) {
  if (!baroOnline) {
    out.baroAltitude = BaroConfig::kInvalidReading;
    out.baroTemp = BaroConfig::kInvalidReading;
    return;
  }

  out.baroAltitude = readAltitudeMeters();
  out.baroTemp = readTemperatureC();
}

static float readAltitudeMeters() {
  toggleOneShot();

  if (!waitForBaroStatus(1 << 1)) {
    return BaroConfig::kInvalidReading;
  }

  Wire.beginTransmission(BaroConfig::kAddress);
  Wire.write(BaroConfig::kRegOutPMsb);
  Wire.endTransmission(false);
  Wire.requestFrom(BaroConfig::kAddress, static_cast<uint8_t>(3));

  int counter = 0;
  while (Wire.available() < 3) {
    if (++counter > 100) {
      return BaroConfig::kInvalidReading;
    }
    delay(1);
  }

  uint8_t msb = Wire.read();
  uint8_t csb = Wire.read();
  uint8_t lsb = Wire.read();

  toggleOneShot();

  float fraction = (lsb >> 4) / 16.0f;
  return static_cast<float>((msb << 8) | csb) + fraction;
}

static float readTemperatureC() {
  toggleOneShot();

  if (!waitForBaroStatus(1 << 1)) {
    return BaroConfig::kInvalidReading;
  }

  Wire.beginTransmission(BaroConfig::kAddress);
  Wire.write(BaroConfig::kRegOutTMsb);
  Wire.endTransmission(false);
  Wire.requestFrom(BaroConfig::kAddress, static_cast<uint8_t>(2));

  int counter = 0;
  while (Wire.available() < 2) {
    if (++counter > 100) {
      return BaroConfig::kInvalidReading;
    }
    delay(1);
  }

  uint8_t msb = Wire.read();
  uint8_t lsb = Wire.read();

  float fraction = (lsb >> 4) / 16.0f;
  return static_cast<float>(msb) + fraction;
}

static bool waitForBaroStatus(uint8_t mask) {
  for (int i = 0; i < 100; ++i) {
    if (i2cRead(BaroConfig::kRegStatus) & mask) {
      return true;
    }
    delay(1);
  }
  return false;
}

static void setModeAltimeter() {
  uint8_t tempSetting = i2cRead(BaroConfig::kRegCtrl1);
  tempSetting |= (1 << 7);
  i2cWrite(BaroConfig::kRegCtrl1, tempSetting);
}

static void setOversampleRate(uint8_t sampleRate) {
  if (sampleRate > 7) {
    sampleRate = 7;
  }

  sampleRate <<= 3;
  uint8_t tempSetting = i2cRead(BaroConfig::kRegCtrl1);
  tempSetting &= 0b11000111;
  tempSetting |= sampleRate;
  i2cWrite(BaroConfig::kRegCtrl1, tempSetting);
}

static void toggleOneShot() {
  uint8_t tempSetting = i2cRead(BaroConfig::kRegCtrl1);
  tempSetting &= ~(1 << 1);
  i2cWrite(BaroConfig::kRegCtrl1, tempSetting);

  tempSetting = i2cRead(BaroConfig::kRegCtrl1);
  tempSetting |= (1 << 1);
  i2cWrite(BaroConfig::kRegCtrl1, tempSetting);
}

static void enableEventFlags() {
  i2cWrite(BaroConfig::kRegPtDataCfg, 0x07);
}

static uint8_t i2cRead(uint8_t regAddr) {
  Wire.beginTransmission(BaroConfig::kAddress);
  Wire.write(regAddr);
  Wire.endTransmission(false);
  Wire.requestFrom(BaroConfig::kAddress, static_cast<uint8_t>(1));
  return Wire.read();
}

static void i2cWrite(uint8_t regAddr, uint8_t value) {
  Wire.beginTransmission(BaroConfig::kAddress);
  Wire.write(regAddr);
  Wire.write(value);
  Wire.endTransmission(true);
}

static void updateDisplay(const SensorReadings &data) {
  ucg.setColor(1, 0, 0, 0);

  drawValueRow(data.adcTemp, 3, 0, 108, 8, 10, data.adcTemp / 8.0f, true);
  drawValueRow(data.adcLight, 3, 0, 108, 18, 20, data.adcLight / 8.0f, true);
  drawValueRow(data.adcButtons, 4, 0, 102, 28, 30, data.adcButtons / 8.0f, true);

  drawValueRow(data.baroAltitude, 4, 1, 96, 38, 40, data.baroAltitude / 8.0f, baroOnline);
  drawValueRow(data.baroTemp, 4, 1, 102, 48, 50, data.baroTemp / 8.0f, baroOnline);

  drawValueRow(data.accelX, 4, 1, 102, 58, 60, data.accelX * 100.0f, imuOnline);
  drawValueRow(data.accelY, 4, 1, 102, 68, 70, data.accelY * 100.0f, imuOnline);
  drawValueRow(data.accelZ, 4, 1, 102, 78, 80, data.accelZ * 100.0f, imuOnline);

  drawValueRow(data.gyroX, 4, 1, 90, 88, 90, data.gyroX, imuOnline);
  drawValueRow(data.gyroY, 4, 1, 90, 98, 100, data.gyroY, imuOnline);
  drawValueRow(data.gyroZ, 4, 1, 90, 108, 110, data.gyroZ, imuOnline);

  drawValueRow(data.magX, 4, 1, 90, 118, 120, data.magX, imuOnline);
  drawValueRow(data.magY, 4, 1, 90, 128, 130, data.magY, imuOnline);
  drawValueRow(data.magZ, 4, 1, 90, 138, 140, data.magZ, imuOnline);
}

static void setTextColor(bool ok) {
  if (ok) {
    ucg.setColor(0, 0, 255, 0);
  } else {
    ucg.setColor(0, 255, 0, 0);
  }
}

static uint8_t clampBarLength(float length) {
  if (length <= 0.0f) {
    return 0;
  }
  if (length >= kDisplayWidth) {
    return kDisplayWidth;
  }
  return static_cast<uint8_t>(length);
}

static void drawBar(uint8_t y, float length) {
  ucg.setColor(0, 0, 0);
  ucg.drawHLine(0, y, kDisplayWidth);
  ucg.setColor(255, 0, 255);
  ucg.drawHLine(0, y, clampBarLength(length));
}

static void drawValueRow(float value,
                         uint8_t width,
                         uint8_t precision,
                         uint8_t valueX,
                         uint8_t valueY,
                         uint8_t lineY,
                         float barLength,
                         bool okColor) {
  char buffer[12];
  dtostrf(value, width, precision, buffer);

  setTextColor(okColor);
  ucg.setPrintPos(valueX, valueY);
  ucg.print(buffer);

  drawBar(lineY, barLength);
}

static void updateStatusLeds() {
  if (digitalRead(Pins::kButtonUp) == LOW) {
    setStatusLed(true, false, false);
  } else if (digitalRead(Pins::kButtonDown) == LOW) {
    setStatusLed(false, false, true);
  } else {
    setStatusLed(false, false, false);
  }
}

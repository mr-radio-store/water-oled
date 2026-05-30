/*
Water level on OLED

Hardware Requirement: uno, water level, OLED, breadboard, and jumper wire

1. OLED Display (SSD1306, I2C 128x64):
OLED Pin	Arduino Pin	Notes
VCC	3.3V or 5V	Depends on OLED module spec
GND	GND	Ground
SCL	A5 (Uno), D21 (Mega), or dedicated SCL pin	I2C Clock line
SDA	A4 (Uno), D20 (Mega), or dedicated SDA pin	I2C Data line
2. Water Sensor (Analog output):
Water Sensor Pin	Arduino Pin	Notes
VCC	5V	Power supply for sensor
GND	GND	Ground
Analog Out	A0	Connect to Arduino analog input pin A0

*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// === Pin Definitions ===
const int waterSensorPin = A0;

// === Sensor Calibration ===
const int minRaw = 200;   // Adjust this (dry value)
const int maxRaw = 600;   // Adjust this (soaked value)

// === OLED Configuration ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDR     0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  delay(100);
  
  Wire.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("❌ OLED not found. Check wiring!"));
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);      // Smaller size for Starting...
  display.setCursor(10, 20);
  display.println(F("Starting..."));
  display.display();
  delay(1000);
}

void loop() {
  int raw = analogRead(waterSensorPin);
  raw = constrain(raw, minRaw, maxRaw);

  int percent = map(raw, minRaw, maxRaw, 0, 100);
  percent = constrain(percent, 0, 100);

  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print("  Water %: ");
  Serial.print(percent);
  Serial.println("%");

  display.clearDisplay();

  // Smaller text for raw value on top
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Raw:");
  display.print(raw);

  // Bigger text for percentage in the middle
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print(percent);
  display.print("%");

  // Bigger detection message below, centered horizontally
  if (percent > 20) {
    display.setTextSize(2);
    // Calculate approximate width of text "Water" at size 2 (~6 pixels per char * 5 chars = 30 pixels)
    int16_t x = (SCREEN_WIDTH - 48) / 2; // 48 is approx width: 6*8 (size 2) chars width
    display.setCursor(x, 50);
    display.print("Water");
  }

  display.display();

  delay(2000);
}

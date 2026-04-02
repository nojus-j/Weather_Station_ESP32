#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BMP280_ADDRESS 0x76
Adafruit_BMP280 bmp;

#define BUTTON1 17
#define BUTTON2 16

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for(;;);
  }
  
  startUp();

  bmp.begin(BMP280_ADDRESS);

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, // Operating mode
                  Adafruit_BMP280::SAMPLING_X2, // Temperature oversampling
                  Adafruit_BMP280::SAMPLING_X16, // Pressure oversampling
                  Adafruit_BMP280::FILTER_X16, // Filtering
                  Adafruit_BMP280::STANDBY_MS_500); // Standby time
}

void loop() {

  bool b1 = digitalRead(BUTTON1) == LOW;
  bool b2 = digitalRead(BUTTON2) == LOW;

  if (b1) {
    Serial.println("Button 1 pressed");
  }

  if (b2) {
    Serial.println("Button 2 pressed");
  }

  displayInfoLocal(bmp.readTemperature(),
            bmp.readPressure() / 100,
            bmp.readAltitude(1009.9));

  delay(100);
}

void startUp() {
  display.display();
  delay(1);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.print("NOJUS :)");
  display.println();
  display.print("WEATHER");
  display.println();
  display.print("STATION");
  display.display();
  delay(2000);
}

void displayInfoLocal(float temp, float pressure, float alt) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("LOCAL INFO");

  display.setCursor(0, 16);
  display.print("Temp: ");
  display.print(temp, 1);

  display.setCursor(0, 32);
  display.print("Press: ");
  display.print(pressure, 1);
  display.println(" hPa");

  display.setCursor(0, 48);
  display.print("Alt: ");
  display.print(alt, 1);
  display.println(" m");

  display.display();
}
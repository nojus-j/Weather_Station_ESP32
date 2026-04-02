#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#define BMP280_ADDRESS 0x76
Adafruit_BMP280 bmp;

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(100);
  Serial.println("BMP280 test");
  unsigned status;
  status = bmp.begin(BMP280_ADDRESS);

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, // Operating mode
                  Adafruit_BMP280::SAMPLING_X2, // Temperature oversampling
                  Adafruit_BMP280::SAMPLING_X16, // Pressure oversampling
                  Adafruit_BMP280::FILTER_X16, // Filtering
                  Adafruit_BMP280::STANDBY_MS_500); // Standby time


}

void loop() {
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");
  Serial.print("Altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" m");
  delay(1000);
}
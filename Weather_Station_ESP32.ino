#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi
const char* ssid = "s";
const char* password = "s";
String apiKey = "s";
int WiFiStatus;

// screen
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int screenMode = 0;

// bmp280
#define BMP280_ADDRESS 0x76
Adafruit_BMP280 bmp;

// buttons
#define BUTTON1 16
#define BUTTON2 17

// api
unsigned long lastCityUpdate = 0;
const unsigned long updateInterval = 60000;

float cachedKaunas = -1000;
float cachedTaurage = -1000;
float cachedVilnius = -1000;


void setup() {
  Serial.begin(9600);

  // button setup
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  // display setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for(;;);
  }
  // start up animation of the screen
  startUp();

  // bmp280 setup
  bmp.begin(BMP280_ADDRESS);

  if (!bmp.begin(BMP280_ADDRESS)) {
    Serial.println("BMP280 not found");
    while(1);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, // Operating mode
                  Adafruit_BMP280::SAMPLING_X2, // Temperature oversampling
                  Adafruit_BMP280::SAMPLING_X16, // Pressure oversampling
                  Adafruit_BMP280::FILTER_X16, // Filtering
                  Adafruit_BMP280::STANDBY_MS_500); // Standby time

  initWiFi();
}
void loop() {
  updateCityWeather();

  bool b1 = digitalRead(BUTTON1) == LOW;
  bool b2 = digitalRead(BUTTON2) == LOW;

  // local
  if (b1) {
    screenMode++;
    if (screenMode > 2) screenMode = 0;
    delay(200);
  }

  // city
  if (b2) {
    screenMode--;
    if (screenMode < 0) screenMode = 2;
    delay(200);
  }

  if (screenMode == 0) {
    displayInfoLocal(bmp.readTemperature(),
            bmp.readPressure() / 100,
            bmp.readAltitude());
  }

  if (screenMode == 1) {
    displayInfoCities();
  }

  if (screenMode == 2) {
    displayInfoWiFi();
  }
}

void startUp() {
  display.display();
  delay(1);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.print("NOJUS");
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

  display.setCursor(122, 56);
  display.print(screenMode);

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

void displayInfoCities() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("CITY WEATHER");

  if (WiFiStatus == -1) {
    display.setCursor(0, 16);
    display.println("OFFLINE MODE");
    display.display();
    return;
  }

  display.setCursor(122, 56);
  display.print(screenMode);

  display.setCursor(0, 16);
  display.print("Kaunas: ");
  display.print(cachedKaunas, 1);
  display.println("C");

  display.setCursor(0, 28);
  display.print("Taurage: ");
  display.print(cachedTaurage, 1);
  display.println("C");

  display.setCursor(0, 40);
  display.print("Vilnius: ");
  display.print(cachedVilnius, 1);
  display.println("C");

  display.display();
}


void displayInfoWiFi() {
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0,0);
  display.println("WiFi INFO:");

  if (WiFiStatus == -1) {
    display.setCursor(0, 16);
    display.println("OFFLINE MODE");
    display.display();
    return;
  }

  display.setCursor(122, 56);
  display.print(screenMode);

  display.setCursor(0, 16);
  display.print("SSID: ");
  display.println(WiFi.SSID());

  display.setCursor(0, 28);
  display.print("IP: ");
  display.println(WiFi.localIP());

  display.setCursor(0, 40);
  display.print("RSSI: ");
  display.print(WiFi.RSSI());
  display.println(" dBm");
  
  display.display();
}

float getCityTemp(String city) {
  if (WiFi.status() != WL_CONNECTED) return -1000;

  WiFiClient client;
  HTTPClient http;

  String url = "http://api.openweathermap.org/data/2.5/weather?q=" 
               + city + "&units=metric&appid=" + apiKey;

  http.begin(client, url);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();

    int tIndex = payload.indexOf("\"temp\":");
    if (tIndex > 0) {
      float temp = payload.substring(tIndex + 7).toFloat();
      return temp;
    }
  }
  return -1000;
}

void updateCityWeather() {
  unsigned long now = millis();

  if (WiFiStatus == 1 && now - lastCityUpdate >= updateInterval) {
    lastCityUpdate = now;

    cachedKaunas = getCityTemp("Kaunas");
    cachedTaurage = getCityTemp("Taurage");
    cachedVilnius = getCityTemp("Vilnius");

    Serial.println("City weather updated!");
  }
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  unsigned long startAttempt = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 8000) {
    Serial.print('.');
    delay(1000);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected!");
    Serial.println(WiFi.localIP());
    WiFiStatus = 1;
  } else {
    Serial.println("\nFailed to connect");
    WiFiStatus = -1;
  }
}

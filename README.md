# Weather_Station_ESP32
A weather station that gathers information inside and outside a greenhouse. The collected data, like temperature, humidity, pressure, moisture in soil and lux, will all be shown in the local device display or on a mobile device.

To power the whole project, I needed to choose the best suitable microcontroller. There are many options, but the main ones I was considering are:
  1. Arduino Nano. It's a compact microcontroller, great with energy efficiency, cheap and easy to use, but it has the drawback of having not enough GPIO pins, and it does not have WiFi or Bluetooth.
  2. Raspberry Pi Pico. More powerful than an Arduino nano, but still does not have WiFi and Bluetooth unless using a Pico W, but it comes with a price increase.
  3. ESP32. And the final microcontroller, which I chose to use because it offers the perfect balance between the previous two, has Wi-Fi and Bluetooth capabilities, a fast processor, and deep-sleep technology, which provides longer battery life if needed.

The next step was to choose the sensors I'll need:
  1. To monitor the change of temperature, pressure and humidity, I chose to use the combination of two sensors, BMP280 and AHT20. The reason for this is that it's cheap, and to get more accurate results, both BMP280 and AHT20 can measure temperature changes, so the results of both will improve accuracy. BMP280 also supports pressure readings, and the AHT20 supports humidity measurements.
  2. To measure the soil's moisture, I'll use 5 capacitive soil moisture sensors. Because the ESP32 does not have the required number of analog inputs, a multiplexer is needed; for this, I'm using the CD74HC4067 analog multiplexer. It allows reading multiple sensors using a single ADC pin.
  3. To display the info locally, I used an ST7789V LCD 3.4-inch display because it's cheap and easy to implement, perfect for a weather station.
  4. To power the ESP32, I first had the idea to have it be powered by solar power, but I fortunately have the capabilities to provide the ESP32 with a constant power source from an outlet in the greenhouse. I'll use a simple power brick, which will provide the 5V needed for the microcontroller.

Then comes the coding part. Because this was my first encounter with ESP32, I used the most widely used Arduino IDE, which is simple and very intuitive to use. It has a wide library support and easy debugging via the Serial Monitor.
The main program logic:

    Read temperature, humidity, and pressure
    Read soil moisture from all 6 channels via multiplexer
    Read light intensity
    Display data on LCD
    Send data over Wi-Fi

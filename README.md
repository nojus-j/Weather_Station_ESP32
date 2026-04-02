# Weather_Station_ESP32
A weather station that collects data about the air temperature, humidity, pressure and lux. With the collected data, the user can see it in two ways: locally on the device itself or using their smart device on a website.

To power the whole project, I needed to choose the best suitable microcontroller. There are many options, but the main ones I was considering are:
  1. Arduino Nano. It's a compact microcontroller, great with energy efficiency, cheap and easy to use, but it has the drawback of having not enough pins for data, and it does not have wifi and Bluetooth. It would be a great choice if I only needed to display the information locally.
  2. Raspberry Pi Pico.
  3. ESP32. And the final microcontroller, which I chose to use because it offers the perfect balance between the previous two, has Wi-Fi and Bluetooth capabilities, a fast processor, and deep-sleep technology, which provides longer battery life.

The next step was to choose the sensors I'll need:
  1. To monitor the change of temperature and pressure, I chose to use the BMP280 sensor. The main reason to use this sensor is that it can measure air pressure accurately.
       Pressure range 300 ... 1100 hPa.
       +- 0.12 hPa or +- 1 meter.
       Temperature range -40 ... +85 °C.
     The sensor has 6 pins:
         VCC - used for power (3.3v)
         GND - used for ground
         SCL - Serial Clock, master device that pulses
         SDA -
         CSB
         SDC
  3. To display the info locally, I used an SSD1306 OLED 0.96 screen because it's cheap and low power, perfect for a weather station.
       it has 4 pins:
         GND - used for ground
         VCC - used for power (3.3v)
         SCL -
         SDA -

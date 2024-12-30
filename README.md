## Word Clock for ESP32S2-Mini

This is the arduino code for a Word Clock using the ESP32S2-Mini.  It uses WifiManager and NTP to connect to the internet and grab the current time.  You can set the time zone in the code.

I wrote this after building [This Clock](https://www.printables.com/model/350568-wordclock-16x8-led-matrix-2023-v1) and since I used a different microcontroller and the source was only available in binary form I couldn't update the output pin used for the 16x16 LED matrix, so I just wrote my own.  

TODO: Add a Web Interface to set color and timezone

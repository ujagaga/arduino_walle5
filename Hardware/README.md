# ESP 32 Eye #

I made this for my robot to be able to stream video, measure distance, drive servos to move the head and run an OLED display for facial expressions.
A RaspberryPi 4 is intended to drive the whole robot, run facial recognition, speach recognition,...

## How to start ##

1. Open Arduino IDE and install support for ESP32. Go to "Arduino > Preferences" and enter: 

        https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json 

into the “Additional Board Manager URLs” field

2. Go to "Tools > Board > Boards Manager" and search for "esp32" Install "esp32 by Espressif Systems" and "Arduino ESP32 Boards"

3. Go to "Sketch > Include Library > manage Libraries" and install "ArduinoOTA"

4. Connect your ESP32-cam board, select it in the boards manager and program it.

5. Open the Serial Monitor to view debugging messages at startup. The baud rate is 1 000 000.

6. Use HTerm or similar Serial therminal emulator to send hex commands to sample video, drive motors, sensors,...

NOTE: This is work in progress. More features to come, so will continue this...

## Contact ##

* web: http://www.radinaradionica.com
* email: ujagaga@gmail.com


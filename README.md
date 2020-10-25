# Helper component for ESP32

[![Build Status](https://travis-ci.com/CalinRadoni/ESP32HAL.svg?branch=main)](https://travis-ci.com/CalinRadoni/ESP32HAL)
![Build with ESP-IDF v4.1](https://github.com/CalinRadoni/ESP32HAL/workflows/Build%20with%20ESP-IDF%20v4.1/badge.svg)

Helper object for ESP32 generic peripherals.

This is a *work-in-progress* and should be tested before use.

## Implemented classes

- GPIO
- ADC
- I2C
- SPI
- CPU (*incomplete, more functions should be added*)
- Timers

### Timers

The `Timers` class uses FreeRTOS Task Notifications as light weight counting semaphores.
If you want to read more about those see the [RTOS Task Notifications](https://www.freertos.org/RTOS-task-notifications.html) document.

## Tests

Tested with:

- GPIO: WS2812* controlled with ESP32-DevKitC board and a level convertor made from two 74HCT1G04 inverters
- GPIO: WS2818B in pax-LampD1 (ESP32-WROOM-32 module + 2 x SN74LV1T34 logic level shifters)
- ADC: *not yet*
- I2C: *not yet*
- SPI: *not yet*
- Timers: in pax-LampD1

Read more about [pax-LampD1](https://github.com/CalinRadoni/pax-LampD1) if intereseted.

## Development Environment

Currently uses the latest stable version of [Espressif IoT Development Framework](https://github.com/espressif/esp-idf) - v4.1 as of October 2020.

Editing was done in [Visual Studio Code](https://code.visualstudio.com).

Version control with [Git](https://git-scm.com).

## License

This software and its documentation are released under the [GNU GPLv3](http://www.gnu.org/licenses/gpl-3.0.html) License. See the `LICENSE-GPLv3.txt` file.

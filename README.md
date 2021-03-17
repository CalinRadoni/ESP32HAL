# Helper component for ESP32

![Build with ESP-IDF v4.1](https://github.com/CalinRadoni/ESP32HAL/workflows/Build%20with%20ESP-IDF%20v4.1/badge.svg)
![Build with ESP-IDF v4.2](https://github.com/CalinRadoni/ESP32HAL/workflows/Build%20with%20ESP-IDF%20v4.2/badge.svg)
![Build with ESP-IDF latest](https://github.com/CalinRadoni/ESP32HAL/workflows/Build%20with%20ESP-IDF%20latest/badge.svg)

Helper object for ESP32 generic peripherals.

This is a *work-in-progress* and should be tested before use.

## Classes

Implemented classes are:

- CPU information, tasks, heap
- GPIO simple usage
- ADC for ADC1, 8 channels, 12 bits, selectable attenuation
- I2C master and SPI master
- Timers, for ESP32's hardware timers, with FreeRTOS Task Notifications

See [ESP32HAL documentation](https://calinradoni.github.io/ESP32HAL/) for more information.

## Usage and tests

- used in [pax-LampD1](https://github.com/CalinRadoni/pax-LampD1)
- tested with ESP32-DevKitC

## Development Environment

- the stable version of [Espressif IoT Development Framework](https://github.com/espressif/esp-idf) - v4.1 as of October 2020
- editing was done in [Visual Studio Code](https://code.visualstudio.com)
- version control with [Git](https://git-scm.com)

## License

This software and its documentation are released under the [GNU GPLv3](http://www.gnu.org/licenses/gpl-3.0.html) License. See the `LICENSE-GPLv3.txt` file.

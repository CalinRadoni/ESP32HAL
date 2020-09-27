# Helper component for ESP32

[![Build Status](https://travis-ci.com/CalinRadoni/ESP32HAL.svg?branch=main)](https://travis-ci.com/CalinRadoni/ESP32HAL)

Helper object for ESP32 generic peripherals.

This is a *work-in-progress* and should be tested before use.

Implemented classes:

- GPIO
- ADC
- I2C
- SPI

## Tests

Tested with:

- GPIO: WS2812* controlled with ESP32-DevKitC board and a level convertor made from two 74HCT1G04 inverters
- GPIO: WS2818B in pax-LampD1 (based on pax-DLED: ESP32-WROOM-32 module and two SN74LV1T34 logic level shifters)
- ADC: *not yet*
- I2C: *not yet*
- SPI: *not yet*

## Development Environment

Currently uses the latest stable version of [Espressif IoT Development Framework](https://github.com/espressif/esp-idf) - v4.0.1 as of June 2020.

Editing was done in [Visual Studio Code](https://code.visualstudio.com).

Version control with [Git](https://git-scm.com).

## License

This software and its documentation are released under the [GNU GPLv3](http://www.gnu.org/licenses/gpl-3.0.html) License. See the `LICENSE-GPLv3.txt` file.

/**
This file is part of ESP32HAL esp-idf component (https://github.com/CalinRadoni/ESP32HAL)
Copyright (C) 2020 by Calin Radoni

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef esp32_hal_gpio_H
#define esp32_hal_gpio_H

#include "freertos/FreeRTOS.h"

class esp32_hal_gpio {
public:
    esp32_hal_gpio(void);
    virtual ~esp32_hal_gpio();

    /**
     * Reset a gpio to default state
     *
     * selects gpio function in IOMUX
     * enables pullup and disable input and output
     * disconnects any other peripheral output configured via GPIO Matrix
     */
    bool ModeDefault(gpio_num_t pinNumber);

    /**
     * Set a pin as input
     *
     * @param pinNumber
     * @param pullUp
     * @param pullDown
     */
    bool ModeInput(gpio_num_t pinNumber, bool pullUp, bool pullDown);

    bool ModeOutput(gpio_num_t pinNumber);
};

#endif

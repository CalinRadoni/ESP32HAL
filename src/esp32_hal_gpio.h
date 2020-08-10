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

#include "esp32_hal_common.h"
#include "driver/gpio.h"

namespace esp32hal {

class GPIO
{
public:
    GPIO(void);
    virtual ~GPIO();

    /**
     * Reset a gpio to default state.
     *
     * This function:
     * - selects GPIO function in IOMUX
     * - enables pull-up and disable input and output
     * - disconnects any other peripheral output configured via GPIO Matrix
     *
     * @param pinNumber use [GPIO_NUM_0 - 19, 21 - 23, 25 - 27, 32 ... GPIO_NUM_39]
     */
    bool ModeDefault(gpio_num_t pinNumber);

    /**
     * Set a pin as input.
     *
     * At least one of pull-up and pull-down must be disabled otherwise
     * the function will return false.
     *
     * @param pinNumber use [GPIO_NUM_0 - 19, 21 - 23, 25 - 27, 32 ... GPIO_NUM_39]
     * @param pullUp use GPIO_PULLUP_ENABLE or GPIO_PULLUP_DISABLE
     * @param pullDown use GPIO_PULLDOWN_ENABLE or GPIO_PULLDOWN_DISABLE
     */
    bool ModeInput(gpio_num_t pinNumber, gpio_pullup_t pullUp = GPIO_PULLUP_DISABLE, gpio_pulldown_t pullDown = GPIO_PULLDOWN_DISABLE);

    /**
     * Put a pin in output mode and set its level.
     *
     * Pull-up and pull-down are disabled.
     *
     * @param pinNumber use [GPIO_NUM_0 - 19, 21 - 23, 25 - 27, 32 ... GPIO_NUM_33], (34 - 39 are input only)
     * @param level sets the output to LOW if 0, HIGH otherwise
     */
    bool ModeOutput(gpio_num_t pinNumber, uint32_t level);
};

} // namespace

#endif

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

#include "esp32_hal_gpio.h"

namespace esp32hal {

GPIO::GPIO(void)
{
    //
}
GPIO::~GPIO()
{
    //
}

// -----------------------------------------------------------------------------

bool GPIO::ModeDefault(gpio_num_t pinNumber)
{
    if ((pinNumber < GPIO_NUM_0) || (pinNumber > GPIO_NUM_MAX)) return false;

    gpio_reset_pin(pinNumber);
    return true;

}

bool GPIO::ModeInput(gpio_num_t pinNumber, bool pullUp, bool pullDown)
{
    if ((pinNumber < GPIO_NUM_0) || (pinNumber > GPIO_NUM_MAX)) return false;

    gpio_config_t gc;
    esp_err_t err;

    gc.pin_bit_mask = 1ULL << pinNumber;
    gc.mode         = GPIO_MODE_INPUT;
    gc.pull_up_en   = pullUp ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
    gc.pull_down_en = pullDown ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;
    gc.intr_type    = GPIO_INTR_DISABLE;

    err = gpio_config(&gc);
    if (err != ESP_OK) return false;

    gpio_pad_select_gpio(pinNumber);

    return true;
}

bool GPIO::ModeOutput(gpio_num_t pinNumber, bool high)
{
    if ((pinNumber < GPIO_NUM_0) || (pinNumber > GPIO_NUM_MAX)) return false;

    gpio_config_t gc;
    esp_err_t err;

    gc.pin_bit_mask = 1ULL << pinNumber;
    gc.mode         = GPIO_MODE_OUTPUT;
    gc.pull_up_en   = GPIO_PULLUP_DISABLE;
    gc.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gc.intr_type    = GPIO_INTR_DISABLE;

    err = gpio_config(&gc);
    if (err != ESP_OK) return false;

    err = gpio_set_level(pinNumber, high ? 1 : 0);
    if (err != ESP_OK) return false;

    gpio_pad_select_gpio(pinNumber);

    return true;
}

} // namespace

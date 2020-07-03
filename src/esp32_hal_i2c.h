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

#ifndef esp32_hal_i2c_H
#define esp32_hal_i2c_H

#include "esp32_hal_common.h"
#include "driver/i2c.h"

class esp32_hal_i2c {
public:
    esp32_hal_i2c(void);
    virtual ~esp32_hal_i2c();

    /**
     * Initializes the I2C in master mode
     *
     * @param port is the I2C port, 0 or 1
     * @param clock_speed is the clock speed in Hz, max 1 MHz, most common value is 100 kHz
     * @param enable_pull_up enable internal pull-up resistors for SDA and SCL lines
     */
    bool Initialize(uint8_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio, uint32_t clock_speed, bool enable_pull_up);

    /**
     * Unloads the I2C driver
     */
    void CleanUp(void);

private:
    i2c_port_t i2c_port;
    bool initialized;
};

#endif

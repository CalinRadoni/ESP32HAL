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

namespace esp32hal {

struct I2COptions
{
    i2c_port_t port = i2c_port_t::I2C_NUM_0;    // use i2c_port_t::I2C_NUM_0 or i2c_port_t::I2C_NUM_1
    gpio_num_t sda_gpio;
    gpio_num_t scl_gpio;
    bool enable_pull_up = false;
    uint32_t clock_speed = 100000;  // clock speed in Hz, max 1 MHz, most sensors are working at 100 kHz.
    uint32_t timeout = 1000; // timeout in ms. Minimum value will be set to 1 ms. Resolution is one tick period.
};

// This is a class for using the I2C peripherals (port 0 and port 1) of an ESP32 in Master mode
class I2C
{
public:
    I2C(void);
    virtual ~I2C();

    /// Initializes the I2C in master mode.
    bool Initialize(const I2COptions&);

    void CleanUp(void);

    /// @brief Writes data.
    /// @details If 'data' == nullptr or 'length' == 0 it writes only 'regAddr' byte.
    /// @param devAddr is the address of the slave device
    /// @param regAddr is the destination register
    /// @param data is data to be written
    /// @param length is the number of bytes to be written
    /// @return true on success
    bool Write(uint8_t devAddr, uint8_t regAddr, uint8_t* data, size_t length);

    /// Fails if 'data' == nullptr or 'length' == 0
    bool Read(uint8_t devAddr, uint8_t regAddr, uint8_t* data, size_t length, bool repeatedStart);

    /// @brief Tests if a device is present at the specified address
    ///
    /// @param devAddr is the address to test
    /// @param testTimeout is a custom timeout to speed up a scan
    ///
    /// Scan example:
    /// @code{.cpp}
    /// for (uint8_t addr = 0x03; addr < 0x78; ++addr) {
    ///     if (TestAddress(addr)) {
    ///         // a device was found at this I2C address
    ///     }
    /// }
    /// @endcode
    bool TestAddress(uint8_t devAddr, uint32_t testTimeout = 100);

private:
    bool initialized;
    i2c_port_t i2c_port;
    TickType_t timeout;
};

} // namespace

#endif

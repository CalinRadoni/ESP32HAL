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

#ifndef esp32_hal_spi_H
#define esp32_hal_spi_H

#include "esp32_hal_common.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

namespace esp32hal {

// default HSPI pins:
//    MOSI = 13, MISO = 12, SCLK = 14, CS0 = 15
// default VSPI pins:
//    MOSI = 23, MISO = 19, SCLK = 18, CS0 = 5
// only the first device attached to the bus cau use CS0
struct SPIOptions
{
    spi_host_device_t host_device = HSPI_HOST; // use HSPI_HOST or VSPI_HOST
    gpio_num_t mosi_gpio;
    gpio_num_t miso_gpio;
    gpio_num_t sclk_gpio;
    uint32_t max_transfer_size = 32; // defaults to 4094 if 0.
};

// This is a class for using HSPI or VSPI in master mode
class SPI
{
public:
    SPI(void);
    virtual ~SPI();

    /**
     * Initialize the HSPI or VSPI in master mode.
     *
     * This function disables DMA for transfers.
     */
    bool Initialize(const SPIOptions&);

    void CleanUp(void);

    /**
     * Adds a SPI slave device.
     *
     * The device is added with an 8-bit address !
     *
     * @param frequency Frequency can be set between SPI_MASTER_FREQ_8M and SPI_MASTER_FREQ_80M.
     *                  Any value you enter will be rounded to one of SPI_MASTER_FREQ_*.
     *                  Read https://docs.espressif.com/projects/esp-idf/en/stable/api-reference/peripherals/spi_master.html
     *                  for speed and timing considerations.
     *                  The frequency depends on the slave too.
     *                  SPI_MASTER_FREQ_10M may be a safe bet in most cases.
     * @param cs_gpio   if not used, pass GPIO_NUM_NC for cs_gpio
     */
    bool AddDevice(uint32_t frequency, gpio_num_t cs_gpio, spi_device_handle_t* handle);

    bool AddDevice(spi_device_interface_config_t* options, spi_device_handle_t* handle);

    bool RemoveDevice(spi_device_handle_t handle);

    /**
     * Writes data to a slave device.
     *
     * regAddr is 8-bit and the length is set in the AddDevice function.
     *
     * If 'data' == nullptr or 'length' == 0 writes only 'regAddr'
     */
    bool Write(spi_device_handle_t handle, uint8_t regAddr, uint8_t* data, size_t length);

    /**
     * Reads data from a slave device.
     *
     * regAddr is 8-bit and the length is set in the AddDevice function.
     *
     * Fails if 'data' == nullptr or 'length' == 0
     */
    bool Read(spi_device_handle_t handle, uint8_t regAddr, uint8_t* data, size_t length);

private:
    bool initialized;
    spi_host_device_t host;
};

} // namespace

#endif


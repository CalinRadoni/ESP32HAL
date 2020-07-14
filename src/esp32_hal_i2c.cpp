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

#include "esp32_hal_i2c.h"

namespace esp32hal {

const bool checkACK = true;
const uint32_t default_timeout = 1000;

I2C::I2C(void)
{
    initialized = false;
    i2c_port = i2c_port_t::I2C_NUM_MAX;
    timeout = default_timeout / portTICK_PERIOD_MS;
}

I2C::~I2C()
{
    CleanUp();
}

// -----------------------------------------------------------------------------

bool I2C::Initialize(const I2COptions& options)
{
    i2c_config_t config;

    if (initialized) CleanUp();

    if ((options.port != i2c_port_t::I2C_NUM_0) && (options.port != i2c_port_t::I2C_NUM_1)) return false;

    i2c_port = options.port;
    timeout = options.timeout / portTICK_PERIOD_MS;
    if (timeout == 0)
        ++timeout;

    config.mode = i2c_mode_t::I2C_MODE_MASTER;
    config.sda_io_num = options.sda_gpio;
    config.scl_io_num = options.scl_gpio;
    config.sda_pullup_en = options.enable_pull_up ? gpio_pullup_t::GPIO_PULLUP_ENABLE : gpio_pullup_t::GPIO_PULLUP_DISABLE;
    config.scl_pullup_en = options.enable_pull_up ? gpio_pullup_t::GPIO_PULLUP_ENABLE : gpio_pullup_t::GPIO_PULLUP_DISABLE;
    config.master.clk_speed = options.clock_speed;

    esp_err_t err = i2c_param_config(i2c_port, &config);
    if (err != ESP_OK) return false;

    err = i2c_driver_install(i2c_port, config.mode, 0, 0, 0);
    if (err != ESP_OK) return false;

    initialized = true;
    return true;
}

void I2C::CleanUp(void)
{
    if (initialized) {
        i2c_driver_delete(i2c_port);
        initialized = false;
    }
}

// -----------------------------------------------------------------------------

bool I2C::Write(uint8_t devAddr, uint8_t regAddr, uint8_t* data, size_t length)
{
    if (!initialized) return false;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_WRITE, checkACK);
    i2c_master_write_byte(cmd, regAddr, checkACK);
    if (data != nullptr && length > 0) {
        i2c_master_write(cmd, data, length, checkACK);
    }
    i2c_master_stop(cmd);
    esp_err_t res = i2c_master_cmd_begin(i2c_port, cmd, timeout);
    i2c_cmd_link_delete(cmd);

    return (res == ESP_OK) ? true : false;
}

bool I2C::Read(uint8_t devAddr, uint8_t regAddr, uint8_t* data, size_t length, bool repeatedStart)
{
    if (!initialized) return false;
    if (data == nullptr) return false;
    if (length == 0) return false;

    esp_err_t res;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_WRITE, checkACK);
    i2c_master_write_byte(cmd, regAddr, checkACK);

    if (!repeatedStart) {
        i2c_master_stop(cmd);
        res = i2c_master_cmd_begin(i2c_port, cmd, timeout);
        i2c_cmd_link_delete(cmd);
        if (res != ESP_OK) return false;

        cmd = i2c_cmd_link_create();
    }

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_READ, checkACK);
    i2c_master_read(cmd, data, length, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    res = i2c_master_cmd_begin(i2c_port, cmd, timeout);
    i2c_cmd_link_delete(cmd);

    return (res == ESP_OK) ? true : false;
}

bool I2C::TestAddress(uint8_t devAddr, uint32_t testTimeout)
{
    if (!initialized) return false;

    TickType_t ticks_timeout = testTimeout / portTICK_PERIOD_MS;
    if (ticks_timeout == 0)
        ++ticks_timeout;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_WRITE, checkACK);
    i2c_master_stop(cmd);
    esp_err_t res = i2c_master_cmd_begin(i2c_port, cmd, ticks_timeout);
    i2c_cmd_link_delete(cmd);

    return (res == ESP_OK) ? true : false;
}

} // namespace

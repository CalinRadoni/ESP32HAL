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

#include "esp32_hal_spi.h"

namespace esp32hal {

SPI::SPI(void)
{
    initialized = false;
    host = HSPI_HOST;
}

SPI::~SPI()
{
    CleanUp();
}

// -----------------------------------------------------------------------------

bool SPI::Initialize(const SPIOptions& options)
{
    if (initialized) return false;

    if ((options.host_device != HSPI_HOST) && (options.host_device != VSPI_HOST)) return false;
    host = options.host_device;

    spi_bus_config_t config;
    config.miso_io_num = options.miso_gpio;
    config.mosi_io_num = options.mosi_gpio;
    config.sclk_io_num = options.sclk_gpio;
    config.quadhd_io_num = -1;
    config.quadwp_io_num = -1;
    config.max_transfer_sz = options.max_transfer_size;
    esp_err_t res = spi_bus_initialize(host, &config, 0);
    if (res != ESP_OK) return false;

    initialized = true;
    return true;
}

void SPI::CleanUp(void)
{
    if (initialized) {
        spi_bus_free(host);
        initialized = false;
    }
}

bool SPI::AddDevice(uint32_t frequency, gpio_num_t cs_gpio, spi_device_handle_t* handle)
{
    if (!initialized) return false;
    if (handle == nullptr) return false;
    if (frequency > SPI_MASTER_FREQ_80M) return false;

    spi_device_interface_config_t config;
    config.command_bits = 0;
    config.address_bits = 8;
    config.dummy_bits = 0;
    config.mode = 0;
    config.duty_cycle_pos = 128;
    config.cs_ena_pretrans = 0;
    config.cs_ena_posttrans = 0;
    config.clock_speed_hz = frequency;
    config.input_delay_ns = 0;
    config.spics_io_num = cs_gpio;
    config.flags = 0;
    config.queue_size = 1;
    config.pre_cb = nullptr;
    config.post_cb = nullptr;

    esp_err_t res = spi_bus_add_device(host, &config, handle);
    return (res == ESP_OK) ? true : false;
}

bool SPI::AddDevice(spi_device_interface_config_t* options, spi_device_handle_t* handle)
{
    if (!initialized) return false;
    if (options == nullptr) return false;
    if (handle == nullptr) return false;

    esp_err_t res = spi_bus_add_device(host, options, handle);
    return (res == ESP_OK) ? true : false;
}

bool SPI::RemoveDevice(spi_device_handle_t handle)
{
    esp_err_t res = spi_bus_remove_device(handle);
    return (res == ESP_OK) ? true : false;
}

bool SPI::Write(spi_device_handle_t handle, uint8_t regAddr, uint8_t* data, size_t length)
{
    if (!initialized) return false;

    size_t number_of_bits = length * 8;

    spi_transaction_t transaction;
    transaction.flags = 0;
    transaction.cmd = 0;
    transaction.addr = regAddr;
    if ((number_of_bits == 0) || (data == nullptr)) {
        transaction.length = 0;
        transaction.tx_buffer = nullptr;
    }
    else {
        transaction.length = number_of_bits;
        transaction.tx_buffer = data;
    }
    transaction.rxlength = 0;
    transaction.rx_buffer = nullptr;
    transaction.user = nullptr;

    esp_err_t res = spi_device_polling_transmit(handle, &transaction);
    return (res == ESP_OK) ? true : false;
}

bool SPI::Read(spi_device_handle_t handle, uint8_t regAddr, uint8_t* data, size_t length)
{
    if (!initialized) return false;
    if (data == nullptr) return false;
    if (length == 0) return false;

    size_t number_of_bits = length * 8;

    spi_transaction_t transaction;
    transaction.flags = 0;
    transaction.cmd = 0;
    transaction.addr = regAddr;
    transaction.length = number_of_bits;
    transaction.tx_buffer = nullptr;
    transaction.rxlength = number_of_bits;
    transaction.rx_buffer = data;
    transaction.user = nullptr;

    esp_err_t res = spi_device_polling_transmit(handle, &transaction);
    return (res == ESP_OK) ? true : false;
}

} // namespace

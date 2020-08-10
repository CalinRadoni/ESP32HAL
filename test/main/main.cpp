/**
This file is part of ESP32HAL esp-idf component (https://github.com/CalinRadoni/ESP32HAL)
Copyright (C) 2019+ by Calin Radoni

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

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include <string.h>

#include "esp32_hal_gpio.h"
#include "esp32_hal_adc.h"
#include "esp32_hal_i2c.h"
#include "esp32_hal_spi.h"

#include "sdkconfig.h"

const char* TAG = "Test";

const gpio_num_t GPIO_BOOT = (gpio_num_t)0;  // input, pull-up

extern "C" {

    void app_main()
    {
        esp32hal::GPIO gpio;
        esp32hal::ADC adc1;
        esp32hal::I2C i2c0;
        esp32hal::SPI hspi;

        if (!gpio.ModeInput(GPIO_BOOT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE)) {
            ESP_LOGE(TAG, "gpio.ModeInput failed !");
        }

        if (!adc1.InitializeADC1()) {
            ESP_LOGE(TAG, "adc1.InitializeADC1 failed !");
        }

        // Print chip information
        esp_chip_info_t chip_info;
        esp_chip_info(&chip_info);
        printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

        printf("silicon revision %d, ", chip_info.revision);

        printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

        for (int i = 10; i >= 0; i--) {
            printf("Restarting in %d seconds...\n", i);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        printf("Restarting now.\n");
        fflush(stdout);
        esp_restart();
    }
}

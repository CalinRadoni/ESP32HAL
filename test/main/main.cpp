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

#include "esp32_hal_cpu.h"
#include "esp32_hal_gpio.h"
#include "esp32_hal_adc.h"
#include "esp32_hal_i2c.h"
#include "esp32_hal_spi.h"
#include "esp32_hal_timers.h"

#include "sdkconfig.h"

const char* TAG = "Test";

const gpio_num_t GPIO_BOOT = (gpio_num_t)0;  // input, pull-up

esp32hal::CPU esp32;
esp32hal::Timers timers;

extern "C" {

    void bye(void) {
        for (uint8_t i = 10; i > 0; --i) {
            printf("Restarting in %d seconds...\n", i);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        fflush(stdout);
        esp_restart();
    }

    static void LoopTask(void *taskParameter) {
        for(;;) {
            uint32_t notifiedValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            if (notifiedValue != 0) {
                if (esp32.RefreshSystemState())
                    esp32.PrintTaskStatus();
            }
        }

        // the next lines are here only for "completion"
        timers.DisableTimer(timer_group_t::TIMER_GROUP_0, timer_idx_t::TIMER_0);
        vTaskDelete(NULL);
    }

    void app_main() {
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

        printf("This is %s revision %d with %d CPU cores, WiFi%s%s ",
            esp32.chipModel.c_str(),
            esp32.revision,
            esp32.numberOfCores,
            esp32.feat_BT ? "/BT" : "",
            esp32.feat_BLE ? "/BLE" : "");

        printf("and %dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            esp32.feat_embeddedFlash ? "embedded" : "external");

        TaskHandle_t xLoopTask = NULL;
        xTaskCreate(LoopTask, "Loop task", 4096, NULL, uxTaskPriorityGet(NULL) + 2, &xLoopTask);
        if (xLoopTask == NULL) {
            printf("ERROR: Failed to create the loop task !");
            bye();
        }
        if (!timers.EnableTimer(xLoopTask, 0, 0, 10000, true, false)) {
            printf("ERROR: Failed to enable the timer 0:0 !");
            bye();
        }
    }
}

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

#include "esp32_hal_cpu.h"
#include "esp_log.h"
#include "esp_flash.h"
#include "esp_spi_flash.h"

namespace esp32hal {

static const char* TAG = "ESP32";

// -----------------------------------------------------------------------------

CPU::CPU(void) {
    runTime = 0;
    taskCount = 0;
    tasks = nullptr;

    revision = 0;
    numberOfCores = 0;
    feat_embeddedFlash = false;
    feat_WiFi_BGN = false;
    feat_BT = false;
    feat_BLE = false;
    espFlashID = 0;
    espFlashSize = 0;
    spiFlashSize = 0;
}

CPU::~CPU() {
    ClearTaskStatus();
}

// -----------------------------------------------------------------------------

uint32_t CPU::GetRandomNumber(void)
{
    return esp_random();
}

void CPU::RefreshHeapSize(void)
{
    heap_caps_get_info(&heapInfo, MALLOC_CAP_DEFAULT);
}

void CPU::ReadChipInfo(void)
{
    esp_chip_info_t chipInfo;
    esp_chip_info(&chipInfo);

    switch (chipInfo.model) {
        case esp_chip_model_t::CHIP_ESP32: chipModel = "ESP32"; break;
        case esp_chip_model_t::CHIP_ESP32S2BETA: chipModel = "ESP32-S2 Beta"; break;
        default: chipModel = "unknown"; break;
    }
    feat_embeddedFlash = chipInfo.features & CHIP_FEATURE_EMB_FLASH;
    feat_WiFi_BGN      = chipInfo.features & CHIP_FEATURE_WIFI_BGN;
    feat_BT            = chipInfo.features & CHIP_FEATURE_BT;
    feat_BLE           = chipInfo.features & CHIP_FEATURE_BLE;
    numberOfCores = chipInfo.cores;
    revision = chipInfo.revision;

    esp_flash_read_id(NULL, &espFlashID);
    esp_flash_get_size(NULL, &espFlashSize);
    spiFlashSize = spi_flash_get_chip_size();
}

bool CPU::RefreshSystemState(void)
{
    ClearTaskStatus();

    RefreshHeapSize();

#ifdef configUSE_TRACE_FACILITY
    taskCount = uxTaskGetNumberOfTasks();

    tasks = new (std::nothrow) TaskStatus_t[taskCount];
    if (tasks == nullptr) return false;

    taskCount = uxTaskGetSystemState(tasks, taskCount, &runTime);

    return taskCount > 0 ? true : false;
#else
    return false;
#endif
}

void CPU::PrintTaskStatus(void)
{
    if (tasks == nullptr) return;
    if (taskCount == 0) return;

    uint32_t taskRunPercentage;
    uint32_t totalRunTime = runTime / 100UL; // for percentage calculations;

    ESP_LOGI(TAG, "Heap bytes allocated %d, free/largest/lifetime_minimum: %d %d %d",
        heapInfo.total_allocated_bytes,
        heapInfo.total_free_bytes,
        heapInfo.largest_free_block,
        heapInfo.minimum_free_bytes);
    ESP_LOGI(TAG, "Heap blocks total/allocated/free: %d %d %d",
        heapInfo.total_blocks,
        heapInfo.allocated_blocks,
        heapInfo.free_blocks);

    for (uint32_t i = 0; i < taskCount; i++) {
        if (totalRunTime > 0) {
            taskRunPercentage = tasks[i].ulRunTimeCounter / totalRunTime;
        }
        else taskRunPercentage = 0;

        std::string taskState;
        switch (tasks[i].eCurrentState) {
            case eRunning:   taskState = "running"; break;
            case eReady:     taskState = "ready"; break;
            case eBlocked:   taskState = "blocked"; break;
            case eSuspended: taskState = "suspended"; break;
            case eDeleted:   taskState = "deleted"; break;
            default:         taskState = "unknown"; break;
        }

#if configTASKLIST_INCLUDE_COREID
        char core;
        switch(tasks[i].xCoreID) {
            case 0:  core = '0'; break;
            case 1:  core = '1'; break;
            default: core = '*'; break;
        }

        ESP_LOGI(TAG, "%3d %-16s %9s, core %c, priority %2d:%-2d, run time %2d%%, min.stack %d",
            tasks[i].xTaskNumber, tasks[i].pcTaskName, taskState.c_str(), core,
            tasks[i].uxCurrentPriority, tasks[i].uxBasePriority,
            taskRunPercentage,
            tasks[i].usStackHighWaterMark);
#else
        ESP_LOGI(TAG, "%3d %-16s %9s, priority %2d:%-2d, run time %2d%%, min.stack %d",
            tasks[i].xTaskNumber, tasks[i].pcTaskName, taskState.c_str(),
            tasks[i].uxCurrentPriority, tasks[i].uxBasePriority,
            taskRunPercentage,
            tasks[i].usStackHighWaterMark);
#endif
    }
}

void CPU::ClearTaskStatus(void)
{
    if (tasks != nullptr) {
        delete[] tasks;
        tasks = nullptr;
    }
    taskCount = 0;
}

} // namespace

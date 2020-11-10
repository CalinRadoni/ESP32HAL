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

#ifndef esp32_hal_cpu_H
#define esp32_hal_cpu_H

#include "esp32_hal_common.h"
#include "freertos/task.h"
#include <string>

namespace esp32hal {

/**
 * For detailed TaskStatus set:
 * - CONFIG_FREERTOS_USE_TRACE_FACILITY=y
 * - CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS=y
 * - CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID=y
 */

class CPU
{
public:
    CPU(void);
    virtual ~CPU();

    /**
     * These are set by the `ReadChipInfo` function (called from constructor).
     */
    std::string chipModel;
    uint8_t revision;
    uint8_t numberOfCores;
    bool feat_embeddedFlash;
    bool feat_WiFi_BGN;
    bool feat_BT;
    bool feat_BLE;

    uint32_t GetRandomNumber(void);

    /** heapInfo is set by `RefreshHeapSize` function. */
    multi_heap_info_t heapInfo;

    /**
     * Sets the `heapInfo` structure.
     *
     * Is called from `RefreshSystemState` function.
     */
    void RefreshHeapSize(void);

    /**
     * Status of tasks.
     *
     * The `RefreshSystemState` function creates and sets the content of this variable.
     * Each call to `RefreshSystemState` destroys and recreates this variable.
     *
     * `tasks` is destroyed by calling the `ClearTaskStatus` function.
     * `ClearTaskStatus` is called from destructor.
     */
    TaskStatus_t* tasks;

    bool RefreshSystemState(void);
    void PrintTaskStatus(void);
    void ClearTaskStatus(void);

protected:
    uint32_t runTime; /** set from the `RefreshSystemState` function */
    uint32_t taskCount; /** set from the `RefreshSystemState` function */

	void ReadChipInfo(void);
};

} // namespace

#endif

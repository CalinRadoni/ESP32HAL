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

#ifndef esp32_hal_timers_H
#define esp32_hal_timers_H

#include "esp32_hal_common.h"
#include "freertos/task.h"
#include "driver/timer.h"

namespace esp32hal {

/**
 * Usage in a dedicated task:
 *
 * <pre>{@code
 * const TickType_t xBlockTime = pdMS_TO_TICS(1000);
 * for (;;) {
 *     uint32_t ulNotifiedValue = ulTaskNotifyTake(pdTRUE, xBlockTime);
 *     if( ulNotifiedValue == 0 ) {
 *         // timeout
 *     }
 *     else {
 *         // ulNotifiedValue contains the number of times the timer fired since last
 *         // `ulTaskNotifyTake` call
 *     }
 * }</pre>
 */

class Timers
{
public:
    Timers(void);
    virtual ~Timers();

    /**
     * @brief Enable and start a timer
     *
     * ESP32 has two groups of two timers:
     * - timer_group_t::TIMER_GROUP_0, timer_idx_t::TIMER_0
     * - timer_group_t::TIMER_GROUP_0, timer_idx_t::TIMER_1
     * - timer_group_t::TIMER_GROUP_1, timer_idx_t::TIMER_0
     * - timer_group_t::TIMER_GROUP_1, timer_idx_t::TIMER_1;
     *
     * @param task       The task to be notified
     * @param group      Timer group
     * @param index      Timer index
     * @param periodMS   Sets the timer's periond in miliseconds
     * @param autoreload Autoreload the timer
     * @param singleShot Single shot or repeated triggering
     */
    bool EnableTimer(TaskHandle_t task, uint8_t group, uint8_t index, uint32_t periodMS, bool autoreload, bool singleShot);

    bool RestartTimer(uint8_t group, uint8_t index, uint32_t periodMS);

    void DisableTimer(uint8_t group, uint8_t index);
};

} // namespace

#endif

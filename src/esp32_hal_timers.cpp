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

#include "esp32_hal_timers.h"

namespace esp32hal {

/**
 * The standard clock is the 80 MHz APB clock.
 * Using a divider of 80 the timers will be clocked with 1 MHz.
 */
const uint32_t clockTimerDivider = 80;

TaskHandle_t task00 = NULL;
TaskHandle_t task01 = NULL;
TaskHandle_t task10 = NULL;
TaskHandle_t task11 = NULL;

// -----------------------------------------------------------------------------

void IRAM_ATTR timer_ISR(void *param) {
    // retrieve the arguments
    uint32_t arg = (uint32_t)param;

    timer_group_t group = ((arg & 0x01) == 0) ? timer_group_t::TIMER_GROUP_0 : timer_group_t::TIMER_GROUP_1;
    timer_idx_t   index = ((arg & 0x02) == 0) ? timer_idx_t::TIMER_0 : timer_idx_t::TIMER_1;
    bool singleShoot    = ((arg & 0x04) == 0) ? false : true;

    // find the physical timer group and the task then clear the interrupt status bit
    timg_dev_t *timerGroup = nullptr;
    TaskHandle_t task = NULL;
    if (group == timer_group_t::TIMER_GROUP_0) {
        timerGroup = &TIMERG0;
        if (index == timer_idx_t::TIMER_0) {
            timerGroup->int_clr_timers.t0 = 1;
            task = task00;
        }
        else {
            timerGroup->int_clr_timers.t1 = 1;
            task = task01;
        }
    }
    else {
        timerGroup = &TIMERG1;
        if (index == timer_idx_t::TIMER_0) {
            timerGroup->int_clr_timers.t0 = 1;
            task = task10;
        }
        else {
            timerGroup->int_clr_timers.t1 = 1;
            task = task11;
        }
    }

    // enable alarm if needed
    if (!singleShoot) {
        if (index == timer_idx_t::TIMER_0) {
            timerGroup->hw_timer[0].config.alarm_en = TIMER_ALARM_EN;
        }
        else {
            timerGroup->hw_timer[1].config.alarm_en = TIMER_ALARM_EN;
        }
    }

    // notify the task
    if (task != NULL) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(task, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken == pdTRUE) {
            portYIELD_FROM_ISR();
        }
    }
}

// -----------------------------------------------------------------------------

Timers::Timers(void)
{
    task00 = NULL;
    task01 = NULL;
    task10 = NULL;
    task11 = NULL;
}

Timers::~Timers()
{
    DisableTimer(0, 0);
    DisableTimer(0, 1);
    DisableTimer(1, 0);
    DisableTimer(1, 1);
}

bool Timers::EnableTimer(TaskHandle_t task, uint8_t groupIn, uint8_t indexIn, uint32_t periodMS, bool autoreload, bool singleShot)
{
    DisableTimer(groupIn, indexIn);

    timer_group_t group = (timer_group_t) groupIn;
    timer_idx_t   index = (timer_idx_t) indexIn;
    if (group >= timer_group_t::TIMER_GROUP_MAX) return false;
    if (index >= timer_idx_t::TIMER_MAX) return false;

    if (group == timer_group_t::TIMER_GROUP_0) {
        if (index == timer_idx_t::TIMER_0) task00 = task;
        else                               task01 = task;
    }
    else {
        if (index == timer_idx_t::TIMER_0) task10 = task;
        else                               task11 = task;
    }

    uint32_t arg = 0;
    if (group == timer_group_t::TIMER_GROUP_1) arg |= 0x01;
    if (index == timer_idx_t::TIMER_1)         arg |= 0x02;
    if (singleShot)                            arg |= 0x04;

    timer_config_t config;
    config.alarm_en    = timer_alarm_t::TIMER_ALARM_DIS;
    config.counter_en  = timer_start_t::TIMER_PAUSE;
    config.intr_type   = timer_intr_mode_t::TIMER_INTR_LEVEL;
    config.counter_dir = timer_count_dir_t::TIMER_COUNT_UP;
    config.auto_reload = autoreload ? timer_autoreload_t::TIMER_AUTORELOAD_EN : timer_autoreload_t::TIMER_AUTORELOAD_DIS;
    config.divider     = clockTimerDivider;

    esp_err_t err = timer_init(group, index, &config);
    if (err != ESP_OK) return false;

    err = timer_set_counter_value(group, index, 0x0ULL);
    if (err != ESP_OK) return false;

    uint64_t alarmValue = TIMER_BASE_CLK;
    alarmValue /= clockTimerDivider;
    alarmValue /= 1000;
    alarmValue *= periodMS;

    err = timer_set_alarm_value(group, index, alarmValue);
    if (err != ESP_OK) return false;

    err = timer_set_alarm(group, index, timer_alarm_t::TIMER_ALARM_EN);
    if (err != ESP_OK) return false;

    err = timer_enable_intr(group, index);
    if (err != ESP_OK) return false;

    err = timer_isr_register(group, index, timer_ISR, (void*)arg, ESP_INTR_FLAG_IRAM, NULL);
    if (err != ESP_OK) return false;

    err = timer_start(group, index);
    if (err != ESP_OK) return false;

    return true;
}

bool Timers::RestartTimer(uint8_t groupIn, uint8_t indexIn, uint32_t periodMS)
{
    timer_group_t group = (timer_group_t) groupIn;
    timer_idx_t   index = (timer_idx_t) indexIn;

    // do not check these again because the checks are allready performed
    // in the `timer_set_alarm_value` and `timer_set_alarm` functions
    // I am favouring the speed a little
    // if (group >= timer_group_t::TIMER_GROUP_MAX) return false;
    // if (index >= timer_idx_t::TIMER_MAX) return false;

    uint64_t alarmValue = TIMER_BASE_CLK;
    alarmValue /= clockTimerDivider;
    alarmValue /= 1000;
    alarmValue *= periodMS;

    esp_err_t err = timer_set_counter_value(group, index, 0x0ULL);
    if (err != ESP_OK) return false;

    err = timer_set_alarm_value(group, index, alarmValue);
    if (err != ESP_OK) return false;

    err = timer_set_alarm(group, index, timer_alarm_t::TIMER_ALARM_EN);
    if (err != ESP_OK) return false;

    return true;
}

void Timers::DisableTimer(uint8_t groupIn, uint8_t indexIn)
{
    timer_group_t group = (timer_group_t) groupIn;
    timer_idx_t   index = (timer_idx_t) indexIn;
    if (group >= timer_group_t::TIMER_GROUP_MAX) return;
    if (index >= timer_idx_t::TIMER_MAX) return;

    if (group == timer_group_t::TIMER_GROUP_0) {
        if (index == timer_idx_t::TIMER_0) task00 = NULL;
        else                               task01 = NULL;
    }
    else {
        if (index == timer_idx_t::TIMER_0) task10 = NULL;
        else                               task11 = NULL;
    }

    timer_pause(group, index);
    timer_disable_intr(group, index);
    timer_set_alarm(group, index, timer_alarm_t::TIMER_ALARM_DIS);
    timer_deinit(group, index);
}

} // namespace

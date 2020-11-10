---
title: Documentation for ESP32HAL ESP-IDF component
#description: 
date: 2020-10-30
#date-modified: 
---

## CPU

This class provides information about the ESP32 CPU, and allows you to check the **tasks** and the **heap**.

With these settings in `sdkconfig`:

- `CONFIG_FREERTOS_USE_TRACE_FACILITY=y`
- `CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS=y`
- `CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID=y`

you can get information like these:

<pre class="highlight" style="font-size: 0.7rem; background-color: #222;color: lime; width: max-content; border-radius: 8px;">
I (70370) ESP32: Heap bytes allocated 20648, free/largest/lifetime_minimum: 299992 165692 294532
I (70370) ESP32: Heap blocks total/allocated/free: 56 49 7

I (70370) ESP32:  12 Loop task            ready, core *, priority  3:3 , run time  0%, min.stack 2340
I (70380) ESP32:   7 IDLE1                ready, core 1, priority  0:0 , run time 99%, min.stack 1116
I (70390) ESP32:   6 IDLE0                ready, core 0, priority  0:0 , run time 99%, min.stack 1108
I (70400) ESP32:   8 Tmr Svc            blocked, core 0, priority  1:1 , run time  0%, min.stack 1592
I (70410) ESP32:   1 esp_timer          blocked, core 0, priority 22:22, run time  0%, min.stack 3640
I (70420) ESP32:   3 ipc1               blocked, core 1, priority 24:24, run time  0%, min.stack 600
I (70430) ESP32:   2 ipc0               blocked, core 0, priority 24:24, run time  0%, min.stack 564</pre>

just by calling `RefreshSystemState` to get the data and `PrintTaskStatus` to display formatted data in terminal.
Use `ClearTaskStatus` to free allocated data if not needed any more.

## GPIO

Simple to use functions:

- **ModeDefault** resets a pin to default state
- **ModeInput** sets a pin as input
- **ModeOutput** puts a pin in output mode and set its level

For `pinNumber` use:

- `GPIO_NUM_0` . . . `GPIO_NUM_19`
- `GPIO_NUM_21` . . . `GPIO_NUM_23`
- `GPIO_NUM_25` . . . `GPIO_NUM_27`
- `GPIO_NUM_32`, `GPIO_NUM_33`
- `GPIO_NUM_34` . . . `GPIO_NUM_39` but **NOT** for output (these are input only)

## ADC

A class for using the 8 channels of ADC1 în 12 bits mode.

Gets raw values, median values and values converted to mV.

The values are converted to mV based on the attenuation value and ADC's characteristics for that attenuation.

## I2C

A class for using the I2C peripherals (port 0 and port 1) of an ESP32 in master mode.

## SPI

A class for using HSPI or VSPI in master mode.

## Timers

A class for using the 4 hardware timers of ESP32. It uses FreeRTOS Task Notifications as light weight counting semaphores.
If you want to read more about those see the [RTOS Task Notifications](https://www.freertos.org/RTOS-task-notifications.html) document.

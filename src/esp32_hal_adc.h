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

#ifndef esp32_hal_adc_H
#define esp32_hal_adc_H

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

/**
 * For adc1_channel_t:
 *  ADC1_CHANNEL_0 is GPIO36
 *  ADC1_CHANNEL_1 is GPIO37
 *  ADC1_CHANNEL_2 is GPIO38
 *  ADC1_CHANNEL_3 is GPIO39
 *  ADC1_CHANNEL_4 is GPIO32
 *  ADC1_CHANNEL_5 is GPIO33
 *  ADC1_CHANNEL_6 is GPIO34
 *  ADC1_CHANNEL_7 is GPIO35
 *
 * Channels can also be specified using GPIO lookup macros like ADC1_GPIO32_CHANNEL
 *
 * For adc_atten_t, the input voltage of ADC will be reduced:
 *  ADC_ATTEN_DB_0   to about 1/1     (range is 0 to 1.1V)
 *  ADC_ATTEN_DB_2_5 to about 1/1.34  (range is 0 to 1.5V)
 *  ADC_ATTEN_DB_6   to about 1/2     (range is 0 to 2.2V)
 *  ADC_ATTEN_DB_11  to about 1/3.6   (range is 0 to 3.9V)
 * The most accurate results are obtained within the following approximate voltage ranges:
 *  ADC_ATTEN_DB_0   100mV to 950mV
 *  ADC_ATTEN_DB_2_5 100mV to 1250mV
 *  ADC_ATTEN_DB_6   150mV to 1750mV
 *  ADC_ATTEN_DB_11  150mV to 2450mV
 */

class esp32_hal_adc {
public:
    esp32_hal_adc(void);
    ~esp32_hal_adc();

    bool InitializeADC1(void); ADC1_GPIO32_CHANNEL;
    bool InitializeADC1_channel(adc1_channel_t channel, adc_atten_t attenuation);

    uint32_t ReadADC1(adc1_channel_t channel);
    uint32_t ReadADC1(adc1_channel_t channel, uint8_t samples);

    /**
     * Read and convert the result to mV
     */
    bool ReadADC1_mV(adc1_channel_t channel, adc_atten_t attenuation, uint32_t* result);

    /**
     * Converts a value readed by ADC1 in mV
     */
    uint32_t ConvertToVoltage(uint32_t value, adc_atten_t attenuation);

protected:
    bool CalibrateADC1(void);

private:
    esp_adc_cal_characteristics_t *adc1_characteristics;
};

#endif

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

#include "esp32_hal_adc.h"

// -----------------------------------------------------------------------------

esp32_hal_adc::esp32_hal_adc(void)
{
    adc1_characteristics = nullptr;
}

esp32_hal_adc::~esp32_hal_adc()
{
    if (adc1_characteristics != nullptr) {
        free(adc1_characteristics);
        adc1_characteristics = nullptr;
    }
}

bool esp32_hal_adc::CalibrateADC1(void)
{
}

bool esp32_hal_adc::InitializeADC1(void)
{
    if (ESP_OK != adc1_config_width(ADC_WIDTH_BIT_12)) return false;

    if (adc1_characteristics == nullptr) {
        adc1_characteristics = (esp_adc_cal_characteristics_t*) calloc(4, sizeof(esp_adc_cal_characteristics_t));
        if (adc1_characteristics == nullptr) {
            return false;
        }

        const uint32_t default_vref = 1100;
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_0,   ADC_WIDTH_BIT_12, default_vref, &adc1_characteristics[0]);
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_2_5, ADC_WIDTH_BIT_12, default_vref, &adc1_characteristics[1]);
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_6,   ADC_WIDTH_BIT_12, default_vref, &adc1_characteristics[2]);
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11,  ADC_WIDTH_BIT_12, default_vref, &adc1_characteristics[3]);
    }

    return true;
}

bool esp32_hal_adc::InitializeADC1_channel(adc1_channel_t channel, adc_atten_t attenuation)
{
    if (channel < ADC1_CHANNEL_0 || channel > ADC1_CHANNEL_7) return false;
    if (attenuation < ADC_ATTEN_DB_0 || attenuation > ADC_ATTEN_DB_11) return false;

    if (ESP_OK != adc1_config_channel_atten(channel, attenuation)) {
        return false;
    }

    return true;
}

uint32_t esp32_hal_adc::ReadADC1(adc1_channel_t channel)
{
    return adc1_get_raw(channel);
}

uint32_t esp32_hal_adc::ReadADC1(adc1_channel_t channel, uint8_t samples)
{
    uint32_t val = 0;

    if (samples == 0) return adc1_get_raw(channel);

    for (uint8_t i = 0; i < samples; i++) {
        val += adc1_get_raw(channel);
    }

    return val / samples;
}

bool ReadADC1_mV(adc1_channel_t channel, adc_atten_t attenuation, uint32_t* result)
{
    if (result == nullptr) return false;

    esp_adc_cal_characteristics_t *characteristics = nullptr;

    switch (attenuation) {
    case ADC_ATTEN_DB_0:
        characteristics = &adc1_characteristics[0];
        break;
    case ADC_ATTEN_DB_2_5:
        characteristics = &adc1_characteristics[1];
        break;
    case ADC_ATTEN_DB_6:
        characteristics = &adc1_characteristics[2];
        break;
    case ADC_ATTEN_DB_11:
        characteristics = &adc1_characteristics[3];
        break;
    default:
        break;
    }
    if (characteristics == nullptr) return 0;

    esp_err_t res = esp_adc_cal_get_voltage(channel, characteristics, result);

    return (res == ESP_OK) ? true : false;
}

uint32_t esp32_hal_adc::ConvertToVoltage(uint32_t value, adc_atten_t attenuation)
{
    esp_adc_cal_characteristics_t *characteristics = nullptr;

    switch (attenuation) {
    case ADC_ATTEN_DB_0:
        characteristics = &adc1_characteristics[0];
        break;
    case ADC_ATTEN_DB_2_5:
        characteristics = &adc1_characteristics[1];
        break;
    case ADC_ATTEN_DB_6:
        characteristics = &adc1_characteristics[2];
        break;
    case ADC_ATTEN_DB_11:
        characteristics = &adc1_characteristics[3];
        break;
    default:
        break;
    }
    if (characteristics == nullptr) return 0;

    return esp_adc_cal_raw_to_voltage(value, characteristics);
}

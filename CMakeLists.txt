set(HAL_CPU
    "src/esp32_hal_cpu.cpp"
)

set(HAL_CORE
    "src/esp32_hal_adc.cpp"
    "src/esp32_hal_gpio.cpp"
    "src/esp32_hal_spi.cpp"
    "src/esp32_hal_i2c.cpp"
    "src/esp32_hal_timers.cpp"
)

set(HAL_REQUIRES
    "esp_adc_cal"
    "spi_flash"
)

idf_component_register(
    SRCS ${HAL_CPU} ${HAL_CORE}
    INCLUDE_DIRS "src"
    REQUIRES ${HAL_REQUIRES}
)

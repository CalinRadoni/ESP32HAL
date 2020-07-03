set(HAL_CPU
    src/cpu_cpu.cpp
)

set(HAL_CORE
    src/ESP32HAL.cpp
    src/esp32_hal_adc.cpp
    src/esp32_hal_gpio.cpp
    src/cpu_spi.cpp
    src/cpu_i2c.cpp
)

set(COMPONENT_SRCS ${HAL_CPU} ${HAL_CORE})
set(COMPONENT_ADD_INCLUDEDIRS "src")

register_component()

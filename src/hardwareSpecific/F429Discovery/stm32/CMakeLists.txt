cmake_minimum_required(VERSION 3.22)

set(STM32_F4_DRIVERS_DIR        ${PROJ_PATH}/hardwareSpecific/F429Discovery/stm32/Drivers)
set(STM32_F4_HAL_DRIVERS_DIR    ${STM32_F4_DRIVERS_DIR}/STM32F4xx_HAL_Driver)
set(STM32_F4_CMSIS_DIR          ${STM32_F4_DRIVERS_DIR}/CMSIS)

set(STM32F4_DRIVER_SOURCES
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_adc.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_adc_ex.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_can.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_cec.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_cortex.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_crc.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_dma_ex.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_dma.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_dma2d.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_dsi.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_eth.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_exti.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_flash.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_flash_ex.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_flash_ramfunc.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_gpio.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_hcd.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_i2c_ex.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_i2c.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_i2s_ex.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_i2s.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_iwdg.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_lptim.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_nand.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_nor.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_pcd.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_pcd_ex.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_pwr_ex.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_rcc_ex.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_rcc.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_rtc.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_spi.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_tim.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_tim_ex.c
    # ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_timebase_tim_template.c
    # ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_timebase_rtc_alarm_template.c
    # ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_timebase_rtc_wakeup_template.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_uart.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_usart.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal_wwdg.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_hal.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_adc.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_crc.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_dac.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_dma.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_dma2d.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_exti.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_gpio.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_i2c.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_lptim.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_pwr.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_rcc.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_rng.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_rtc.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_spi.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_tim.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_usart.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_usb.c
    ${STM32_F4_HAL_DRIVERS_DIR}/Src/stm32f4xx_ll_utils.c
)

set(STM32F4_DRIVER_INCLUDES
    ${STM32_F4_HAL_DRIVERS_DIR}/Inc
    ${STM32_F4_HAL_DRIVERS_DIR}/Inc/Legacy
    ${STM32_F4_CMSIS_DIR}/Device/ST/STM32F4xx/Include
    ${STM32_F4_CMSIS_DIR}/Include
    ${STM32_F4_CMSIS_DIR}/Core/Include
    ${STM32_F4_CMSIS_DIR}/DSP/Include
)
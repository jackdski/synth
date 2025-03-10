cmake_minimum_required(VERSION 3.22)

set(STM32_G4_DRIVERS_DIR        ${PROJ_PATH}/hardwareSpecific/G4_RevA/stm32/Drivers)
set(STM32_G4_HAL_DRIVERS_DIR    ${STM32_G4_DRIVERS_DIR}/STM32G4xx_HAL_Driver)
set(STM32_G4_CMSIS_DIR          ${STM32_G4_DRIVERS_DIR}/CMSIS)

set(STM32G4_DRIVER_SOURCES
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_pcd.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_pcd_ex.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_ll_usb.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_rcc.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_rcc_ex.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_flash.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_flash_ex.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_flash_ramfunc.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_gpio.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_exti.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_dma.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_dma_ex.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_pwr.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_pwr_ex.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_cortex.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_adc.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_adc_ex.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_ll_adc.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_cordic.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_fmac.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_i2c.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_i2c_ex.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_i2s.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_spi.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_spi_ex.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_tim.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_tim_ex.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_wwdg.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_rng.c
   ${STM32_G4_HAL_DRIVERS_DIR}/Src/stm32g4xx_hal_iwdg.c
)

set(STM32G4_DRIVER_INCLUDES
    ${STM32_G4_HAL_DRIVERS_DIR}/Inc
    ${STM32_G4_HAL_DRIVERS_DIR}/Inc/Legacy
    ${STM32_G4_CMSIS_DIR}/Device/ST/STM32G4xx/Include
    ${STM32_G4_CMSIS_DIR}/Include
    ${STM32_G4_CMSIS_DIR}/Core/Include
    ${STM32_G4_CMSIS_DIR}/DSP/Include
)
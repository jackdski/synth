set(G4_REV_A_STM32_PATH ${CMAKE_CURRENT_LIST_DIR}/stm32)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/
    ${CMAKE_CURRENT_LIST_DIR}/audio
    ${CMAKE_CURRENT_LIST_DIR}/devices
    ${CMAKE_CURRENT_LIST_DIR}/drv
    ${CMAKE_CURRENT_LIST_DIR}/stm32
    ${CMAKE_CURRENT_LIST_DIR}/stm32/Core/Inc
    # ${CMAKE_CURRENT_LIST_DIR}/stm32/FATFS/Target
    # ${CMAKE_CURRENT_LIST_DIR}/stm32/FATFS/App
    # ${CMAKE_CURRENT_LIST_DIR}/stm32/FATFS/
    ${CMAKE_CURRENT_LIST_DIR}/stm32/USB_Device/App
    ${CMAKE_CURRENT_LIST_DIR}/stm32/USB_Device/Target
    ${CMAKE_CURRENT_LIST_DIR}/stm32/Drivers/STM32G4xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_LIST_DIR}/stm32/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy
    ${CMAKE_CURRENT_LIST_DIR}/stm32/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    ${CMAKE_CURRENT_LIST_DIR}/stm32/Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Inc
    # ${CMAKE_CURRENT_LIST_DIR}/stm32/Middlewares/Third_Party/FatFs/
    # ${CMAKE_CURRENT_LIST_DIR}/stm32/Middlewares/Third_Party/FatFs/src/
    ${CMAKE_CURRENT_LIST_DIR}/stm32/Drivers/CMSIS/Device/ST/STM32G4xx/Include
    ${CMAKE_CURRENT_LIST_DIR}/stm32/Drivers/CMSIS/Include
    ${CMAKE_CURRENT_LIST_DIR}/stm32/Drivers/CMSIS/Core/Include
    ${CMAKE_CURRENT_LIST_DIR}/stm32/Drivers/CMSIS/DSP/Include
    ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/themes
    ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/themes/default
)

target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/startup_stm32g491xx.s
    ${CMAKE_CURRENT_LIST_DIR}/hardwareSpecific.c
    # ${CMAKE_CURRENT_LIST_DIR}/audio/audio_hardwareSpecific.c
    ${CMAKE_CURRENT_LIST_DIR}/audio/audio_hardwareSpecific.cpp
    ${CMAKE_CURRENT_LIST_DIR}/drv/drv_GPIO_hardwareSpecific.c
    ${CMAKE_CURRENT_LIST_DIR}/drv/drv_I2C_hardwareSpecific.c
    ${CMAKE_CURRENT_LIST_DIR}/drv/drv_SPI_hardwareSpecific.c
    ${CMAKE_CURRENT_LIST_DIR}/drv/drv_encoder_hardwareSpecific.c
    ${CMAKE_CURRENT_LIST_DIR}/devices/LEDs_hardwareSpecific.c
    ${CMAKE_CURRENT_LIST_DIR}/devices/button_hardwareSpecific.c
    ${CMAKE_CURRENT_LIST_DIR}/devices/knob_hardwareSpecific.c
    ${CMAKE_CURRENT_LIST_DIR}/devices/PCA9555_hardwareSpecific.c
    ${CMAKE_CURRENT_LIST_DIR}/devices/PCA9685_hardwareSpecific.c
    ${CMAKE_CURRENT_LIST_DIR}//lvgl/src/extra/themes/default/lv_theme_default.c
)


set(G4_REV_A_STM32_PATH  ${CMAKE_CURRENT_LIST_DIR}/stm32)

target_sources(${TARGET} PRIVATE
    ${G4_REV_A_STM32_PATH}/Core/Src/adc.c
    ${G4_REV_A_STM32_PATH}/Core/Src/cordic.c
    ${G4_REV_A_STM32_PATH}/Core/Src/dma.c
    ${G4_REV_A_STM32_PATH}/Core/Src/fmac.c
    ${G4_REV_A_STM32_PATH}/Core/Src/gpio.c
    ${G4_REV_A_STM32_PATH}/Core/Src/i2c.c
    ${G4_REV_A_STM32_PATH}/Core/Src/i2s.c
    ${G4_REV_A_STM32_PATH}/Core/Src/iwdg.c
    ${G4_REV_A_STM32_PATH}/Core/Src/rng.c
    ${G4_REV_A_STM32_PATH}/Core/Src/spi.c
    ${G4_REV_A_STM32_PATH}/Core/Src/tim.c
    ${G4_REV_A_STM32_PATH}/Core/Src/wwdg.c
    ${G4_REV_A_STM32_PATH}/Core/Src/stm32g4xx_it.c
    ${G4_REV_A_STM32_PATH}/Core/Src/stm32g4xx_hal_msp.c
    ${G4_REV_A_STM32_PATH}/Core/Src/stm32g4xx_hal_timebase_tim.c
    # ${G4_REV_A_STM32_PATH}/FATFS/App/app_fatfs.c
    # ${G4_REV_A_STM32_PATH}/FATFS/Target/user_diskio.c
    # ${G4_REV_A_STM32_PATH}/USB_Device/App/usb_device.c
    # ${G4_REV_A_STM32_PATH}/USB_Device/App/usbd_desc.c
    # ${G4_REV_A_STM32_PATH}/USB_Device/App/usbd_custom_hid_if.c
    # ${G4_REV_A_STM32_PATH}/USB_Device/Target/usbd_conf.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd_ex.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usb.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc_ex.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_adc.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cordic.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fmac.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c_ex.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2s.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi_ex.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_wwdg.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rng.c
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_iwdg.c
    ${G4_REV_A_STM32_PATH}/Core/Src/system_stm32g4xx.c
    # ${G4_REV_A_STM32_PATH}/Middlewares/Third_Party/FatFs/src/diskio.c
    # ${G4_REV_A_STM32_PATH}/Middlewares/Third_Party/FatFs/src/ff.c
    # ${G4_REV_A_STM32_PATH}/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c
    # ${G4_REV_A_STM32_PATH}/Middlewares/Third_Party/FatFs/src/option/syscall.c
    ${G4_REV_A_STM32_PATH}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
    ${G4_REV_A_STM32_PATH}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
    ${G4_REV_A_STM32_PATH}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
    ${G4_REV_A_STM32_PATH}/Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Src/usbd_customhid.c
)

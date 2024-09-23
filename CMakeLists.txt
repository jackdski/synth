cmake_minimum_required(VERSION 3.22)

set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

# Some default GCC settings
# arm-none-eabi- must be part of path environment
set(TOOLCHAIN_PREFIX                /opt/homebrew/bin/arm-none-eabi-)
set(triple arm-linux-gnueabihf)
set(FLAGS                           "-fdata-sections -ffunction-sections -Wl,--gc-sections")
set(CPP_FLAGS                       "${FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_C_FLAGS                   ${FLAGS})
set(CMAKE_CXX_FLAGS                 ${CPP_FLAGS})
set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size)

set(CMAKE_C_COMPILER_TARGET         ${triple})
set(CMAKE_ASM_COMPILER_TARGET       ${triple})
set(CMAKE_CXX_COMPILER_TARGET       ${triple})

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Setup compiler settings
set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)
set(CMAKE_CXX_STANDARD              20)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)

# Paths
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(HARDWARE_SPECIFIC_PATH          ${CMAKE_CURRENT_SOURCE_DIR}/src/hardwareSpecific)
set(FREERTOS_PATH                   ${CMAKE_CURRENT_SOURCE_DIR}/src/FreeRTOS)

set(BUILD_DIRECTORY                 "build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${BUILD_DIRECTORY})
set(CMAKE_CROSSCOMPILING            1)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
message("Build type: " ${CMAKE_BUILD_TYPE})

#
# Core project settings
#
project(synth)
enable_language(C CXX ASM)

set(sources_shared_SRCS
    ${PROJ_PATH}/main.c
    ${PROJ_PATH}/drv/drv_I2C.c
    ${PROJ_PATH}/drv/drv_SPI.c
    ${PROJ_PATH}/drv/drv_GPIO.c
    ${PROJ_PATH}/drv/drv_encoder.c
    ${PROJ_PATH}/devices/LEDs.c
    ${PROJ_PATH}/devices/PCA9555.c
    ${PROJ_PATH}/devices/PCA9685.c
    ${PROJ_PATH}/devices/button.c
    ${PROJ_PATH}/devices/knob.c
    ${PROJ_PATH}/devices/sgtl5000.c
    ${PROJ_PATH}/devices/ST7789.c
    ${PROJ_PATH}/display/display.c
    ${PROJ_PATH}/display/screens/homeScreen.c
    ${PROJ_PATH}/display/screens/waveformScreen.c
    ${PROJ_PATH}/display/screens/settingsSelection.c
    ${PROJ_PATH}/misc/misc.c
    ${PROJ_PATH}/misc/knobControls.c
    ${PROJ_PATH}/wavetables/wavetables.c
    ${PROJ_PATH}/audio/effects/lpf.c
    ${PROJ_PATH}/audio/oscillator.c
    ${PROJ_PATH}/audio/note_info.c
    ${PROJ_PATH}/audio/keyboard.cpp
    ${PROJ_PATH}/audio/adsr.c
    # ${PROJ_PATH}/audio/note.c
    ${PROJ_PATH}/audio/audio.cpp
    ${PROJ_PATH}/audio/AudioManager.cpp
    # ${PROJ_PATH}/audio/sequencer.cpp
    # ${PROJ_PATH}/audio/mixer.c
    ${PROJ_PATH}/utils/cpu_utils.c
    ${FREERTOS_PATH}/croutine.c
    ${FREERTOS_PATH}/event_groups.c
    ${FREERTOS_PATH}/list.c
    ${FREERTOS_PATH}/queue.c
    ${FREERTOS_PATH}/stream_buffer.c
    ${FREERTOS_PATH}/tasks.c
    ${FREERTOS_PATH}/timers.c
    ${FREERTOS_PATH}/portable/GCC/ARM_CM4F/port.c
    ${FREERTOS_PATH}/portable/Common/mpu_wrappers.c
    ${FREERTOS_PATH}/portable/MemMang/heap_4.c
    # ${PROJ_PATH}/lib/SystemView/SEGGER/SEGGER_SYSVIEW.c
    # ${PROJ_PATH}/lib/SystemView/SEGGER/SEGGER_RTT.c
    # ${PROJ_PATH}/lib/SystemView/SEGGER/SEGGER_RTT_printf.c
    # ${PROJ_PATH}/lib/SystemView/Sample/FreeRTOSV10.4/SEGGER_SYSVIEW_FreeRTOS.c
    # ${PROJ_PATH}/lib/SystemView/Sample/FreeRTOSV10.4/Config/Cortex-M/SEGGER_SYSVIEW_Config_FreeRTOS.c
    # ${PROJ_PATH}/lib/SystemView/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c
    # ${PROJ_PATH}/lib/SystemView/SEGGER/SEGGER_RTT_ASM_ARMv7M.S
)

set(include_path_shared_DIRS
    ${PROJ_PATH}/
    ${PROJ_PATH}/misc/
    ${PROJ_PATH}/display/
    ${PROJ_PATH}/wavetables/
    ${PROJ_PATH}/audio/
    ${PROJ_PATH}/audio/effects/
    ${PROJ_PATH}/devices/
    ${PROJ_PATH}/drv/
    ${PROJ_PATH}/utils/
    ${PROJ_PATH}/lib/
    # ${PROJ_PATH}/lib/SystemView
    # ${PROJ_PATH}/lib/SystemView/SEGGER
    # ${PROJ_PATH}/lib/SystemView/CONFIG
    # ${PROJ_PATH}/lib/SystemView/Sample/FreeRTOSV10.4
    ${FREERTOS_PATH}/portable/MemMag
    ${FREERTOS_PATH}/portable/GCC/ARM_CM4F
    ${FREERTOS_PATH}/include
)

# Options
option(BUILD_F429DISCOVERY "Enable building F429 Discovery Revision" ON)
option(BUILD_G4_REV_A "Enable building the STM32G4-based revision A" ON)


if(BUILD_F429DISCOVERY)

    set(synth_F429Discovery synth_F429Discovery)

    set(F429_DISCOVERY_STM32_PATH ${HARDWARE_SPECIFIC_PATH}/F429Discovery/stm32)

    add_executable(synth_F429Discovery
        ${HARDWARE_SPECIFIC_PATH}/F429Discovery/startup_stm32f429xx.s
        ${HARDWARE_SPECIFIC_PATH}/F429Discovery/main.cpp
        ${F429_DISCOVERY_STM32_PATH}/Core/Src/gpio.c
        ${F429_DISCOVERY_STM32_PATH}/Core/Src/i2c.c
        ${F429_DISCOVERY_STM32_PATH}/Core/Src/i2s.c
        ${F429_DISCOVERY_STM32_PATH}/Core/Src/sdio.c
        ${F429_DISCOVERY_STM32_PATH}/Core/Src/spi.c
        ${F429_DISCOVERY_STM32_PATH}/Core/Src/tim.c
        ${F429_DISCOVERY_STM32_PATH}/Core/Src/usart.c
        ${F429_DISCOVERY_STM32_PATH}/Core/Src/stm32f4xx_it.c
        ${F429_DISCOVERY_STM32_PATH}/Core/Src/stm32f4xx_hal_timebase_tim.c
        ${F429_DISCOVERY_STM32_PATH}/Core/Src/stm32f4xx_hal_msp.c
        ${F429_DISCOVERY_STM32_PATH}/Core/Src/system_stm32f4xx.c
        ${F429_DISCOVERY_STM32_PATH}/Core/Src/adc.c
        ${F429_DISCOVERY_STM32_PATH}/Core/Src/dma.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s_ex.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
        ${sources_shared_SRCS}
    )

    set(include_path_F429Discovery_DIRS
        ${include_path_shared_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hardwareSpecific/F429Discovery/
        ${F429_DISCOVERY_STM32_PATH}/
        ${F429_DISCOVERY_STM32_PATH}/Core/Inc
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Inc
        ${F429_DISCOVERY_STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
        ${F429_DISCOVERY_STM32_PATH}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
        ${F429_DISCOVERY_STM32_PATH}/Drivers/CMSIS/Include
        ${F429_DISCOVERY_STM32_PATH}/Drivers/CMSIS/Core/Include
        ${F429_DISCOVERY_STM32_PATH}/Drivers/CMSIS/Core_A/Include
        ${F429_DISCOVERY_STM32_PATH}/Drivers/CMSIS/DSP/Include
    )

    set(LV_CONF_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lv_conf.h
        CACHE STRING "" FORCE
    )
    set(LV_CONF_INCLUDE_SIMPLE 1)
    # set(BUILD_SHARED_LIBS ON)

    set_target_properties(synth_F429Discovery
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIRECTORY}/F429Discovery"
    )

    include(${HARDWARE_SPECIFIC_PATH}/F429Discovery/lvgl/CMakeLists.txt)
    target_link_libraries(synth_F429Discovery PRIVATE lvgl::lvgl)
    target_include_directories(synth_F429Discovery PRIVATE ${include_path_F429Discovery_DIRS})

    target_compile_options(lvgl PRIVATE
        -MMD
        -MP
        -MF
        -mthumb
        -mcpu=cortex-m4
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
        # Full debug configuration
        # -Og -g3 -ggdb
        # Optimization
        -O2 -g3 -ggdb
    )

    target_compile_options(synth_F429Discovery PRIVATE
        -mthumb
        -mcpu=cortex-m4
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
        # Full debug configuration
        # -Og -g3 -ggdb
        # Optimization
        -O2 -g3 -ggdb
    )

    set(symbols_SYMB_F429_DISCOVERY
        "DEBUG"
        "STM32F429xx"
        "USE_HAL_DRIVER"
    )
    target_compile_definitions(synth_F429Discovery PUBLIC ${symbols_SYMB_F429_DISCOVERY})

    # Linker options
    set(F429_DISCOVERY_linker_script_SRC    ${HARDWARE_SPECIFIC_PATH}/F429Discovery/stm32/STM32F429ZITx_FLASH.ld)
    target_link_options(synth_F429Discovery PRIVATE
        -T${F429_DISCOVERY_linker_script_SRC}
        -mthumb
        -mcpu=cortex-m4
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -Wl,-Map=${BUILD_DIRECTORY}/F429Discovery/synth_F429Discovery.map
        -specs=nosys.specs
        -u _printf_float                # STDIO float formatting support
        -Wl,--start-group
        -lc
        -lm
        -lnosys
        -lstdc++
        -lsupc++
        -Wl,--end-group
        -Wl,--print-memory-usage
    )

    # Execute post-build to print size
    add_custom_command(TARGET synth_F429Discovery POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:synth_F429Discovery>
    )

    # Convert output to hex and binary
    add_custom_command(TARGET synth_F429Discovery POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:synth_F429Discovery> ${BUILD_DIRECTORY}/G4_RevA/synth_F429Discovery.hex
    )

    # Convert to bin file -> add conditional check?
    add_custom_command(TARGET synth_F429Discovery POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:synth_F429Discovery> ${BUILD_DIRECTORY}/G4_RevA/synth_F429Discovery.bin
    )

endif(BUILD_F429DISCOVERY)

if(BUILD_G4_REV_A)

set(G4_REV_A_STM32_PATH ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32)

add_executable(synth_G4RevA
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/startup_stm32g491xx.s
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/hardwareSpecific.c
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/audio/audio_hardwareSpecific.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/audio/audio_hardwareSpecific.cpp
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/drv/drv_GPIO_hardwareSpecific.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/drv/drv_I2C_hardwareSpecific.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/drv/drv_SPI_hardwareSpecific.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/drv/drv_encoder_hardwareSpecific.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/devices/LEDs_hardwareSpecific.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/devices/button_hardwareSpecific.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/devices/knob_hardwareSpecific.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/devices/PCA9555_hardwareSpecific.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/devices/PCA9685_hardwareSpecific.c
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
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/lvgl/src/extra/themes/default/lv_theme_default.c
    ${sources_shared_SRCS}
)

set(include_path_G4_RevA_DIRS
    ${include_path_shared_DIRS}
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/audio/
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/drv
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/devices
    ${G4_REV_A_STM32_PATH}/
    ${G4_REV_A_STM32_PATH}/Core/Inc
    # ${G4_REV_A_STM32_PATH}/FATFS/Target
    # ${G4_REV_A_STM32_PATH}/FATFS/App
    # ${G4_REV_A_STM32_PATH}/FATFS/
    ${G4_REV_A_STM32_PATH}/USB_Device/App
    ${G4_REV_A_STM32_PATH}/USB_Device/Target
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Inc
    ${G4_REV_A_STM32_PATH}/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy
    ${G4_REV_A_STM32_PATH}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    ${G4_REV_A_STM32_PATH}/Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Inc
    # ${G4_REV_A_STM32_PATH}/Middlewares/Third_Party/FatFs/
    # ${G4_REV_A_STM32_PATH}/Middlewares/Third_Party/FatFs/src/
    ${G4_REV_A_STM32_PATH}/Drivers/CMSIS/Device/ST/STM32G4xx/Include
    ${G4_REV_A_STM32_PATH}/Drivers/CMSIS/Include
    ${G4_REV_A_STM32_PATH}/Drivers/CMSIS/Core/Include
    ${G4_REV_A_STM32_PATH}/Drivers/CMSIS/DSP/Include
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/lvgl/src/extra/themes
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/lvgl/src/extra/themes/default
)

set(LV_CONF_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lv_conf.h
    CACHE STRING "" FORCE
)
set(LV_CONF_INCLUDE_SIMPLE 1)
# set(BUILD_SHARED_LIBS ON)

set_target_properties(synth_G4RevA
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIRECTORY}/G4_RevA"
)

include(${HARDWARE_SPECIFIC_PATH}/G4_RevA/lvgl/CMakeLists.txt)
target_link_libraries(synth_G4RevA PRIVATE lvgl_G4::lvgl_G4)
target_include_directories(synth_G4RevA PRIVATE ${include_path_G4_RevA_DIRS})

target_compile_options(lvgl_G4 PRIVATE
    -mthumb
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    # Full debug configuration
    # -Og -g3 -ggdb
    # Optimization
    -O2 -g3 -ggdb
)

target_compile_options(synth_G4RevA PRIVATE
    -mthumb
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    # Full debug configuration
    # -Og -g3 -ggdb
    # Optimization
    -O2 -g3 -ggdb
)

set(symbols_SYMB_G4_RevA
    "DEBUG"
    "USE_HAL_DRIVER"
    "STM32G491xx"
)
target_compile_definitions(synth_G4RevA PUBLIC ${symbols_SYMB_G4_RevA})

# Linker options
set(G4_RevA_linker_script_SRC    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/STM32G491CETx_FLASH.ld)
target_link_options(synth_G4RevA PRIVATE
    -T${G4_RevA_linker_script_SRC}
    -mthumb
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -Wl,-Map=${BUILD_DIRECTORY}/G4_RevA/synth_G4RevA.map
    -specs=nosys.specs
    -u _printf_float                # STDIO float formatting support
    -Wl,--start-group
    -lc
    -lm
    -lnosys
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,--print-memory-usage
)

# Execute post-build to print size
add_custom_command(TARGET synth_G4RevA POST_BUILD
COMMAND ${CMAKE_SIZE} $<TARGET_FILE:synth_G4RevA>
)

# Convert output to hex and binary
add_custom_command(TARGET synth_G4RevA POST_BUILD
COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:synth_G4RevA> ${BUILD_DIRECTORY}/G4_RevA/synth_G4RevA.hex
)

# Convert to bin file -> add conditional check?
add_custom_command(TARGET synth_G4RevA POST_BUILD
COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:synth_G4RevA> ${BUILD_DIRECTORY}/G4_RevA/synth_G4RevA.bin
)

endif(BUILD_G4_REV_A)
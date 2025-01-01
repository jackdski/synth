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

set(SRCS
    ${PROJ_PATH}/main.cpp
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
    ${PROJ_PATH}/devices/ST7789.cpp
    ${PROJ_PATH}/display/DisplayManager.cpp
    ${PROJ_PATH}/display/sub_screens/volume_bar.cpp
    ${PROJ_PATH}/display/screens/homeScreen.cpp
    ${PROJ_PATH}/display/screens/waveformScreen.cpp
    ${PROJ_PATH}/display/screens/settingsSelection.cpp
    ${PROJ_PATH}/display/screens/sequencerScreen.cpp
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
    ${PROJ_PATH}/audio/sequencer.cpp
    # ${PROJ_PATH}/audio/mixer.c
    ${PROJ_PATH}/utils/cpu_utils.c
    ${PROJ_PATH}/lvgl/src/themes/mono/lv_theme_mono.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/startup_stm32g491xx.s
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/hardwareSpecific.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/interrupts.cpp
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
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/adc.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/cordic.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/dma.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/fmac.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/gpio.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/i2c.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/i2s.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/iwdg.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/rng.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/spi.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/tim.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/wwdg.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/stm32g4xx_it.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/stm32g4xx_hal_msp.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/stm32g4xx_hal_timebase_tim.c
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/FATFS/App/app_fatfs.c
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/FATFS/Target/user_diskio.c
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/USB_Device/App/usb_device.c
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/USB_Device/App/usbd_desc.c
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/USB_Device/App/usbd_custom_hid_if.c
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/USB_Device/Target/usbd_conf.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd_ex.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usb.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc_ex.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_adc.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cordic.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fmac.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c_ex.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2s.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi_ex.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_wwdg.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rng.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_iwdg.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Src/system_stm32g4xx.c
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Middlewares/Third_Party/FatFs/src/diskio.c
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Middlewares/Third_Party/FatFs/src/ff.c
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Middlewares/Third_Party/FatFs/src/option/syscall.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Src/usbd_customhid.c
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
)

set(INCLUDE_DIRS
    ${PROJ_PATH}/
    ${PROJ_PATH}/misc/
    ${PROJ_PATH}/display/sub_screens/
    ${PROJ_PATH}/display/
    ${PROJ_PATH}/wavetables/
    ${PROJ_PATH}/audio/
    ${PROJ_PATH}/audio/effects/
    ${PROJ_PATH}/audio/samples/
    ${PROJ_PATH}/devices/
    ${PROJ_PATH}/drv/
    ${PROJ_PATH}/utils/
    ${PROJ_PATH}/lib/
    ${FREERTOS_PATH}/portable/MemMag
    ${FREERTOS_PATH}/portable/GCC/ARM_CM4F
    ${FREERTOS_PATH}/include
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/audio
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/devices
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/drv
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Core/Inc
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/FATFS/Target
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/FATFS/App
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/FATFS/
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/USB_Device/App
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/USB_Device/Target
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Inc
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Inc
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Middlewares/Third_Party/FatFs/
    # ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Middlewares/Third_Party/FatFs/src/
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/CMSIS/Device/ST/STM32G4xx/Include
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/CMSIS/Include
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/CMSIS/Core/Include
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/Drivers/CMSIS/DSP/Include
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/lvgl/src/themes
    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/lvgl/src/themes/mono
)

set(TARGET synth_G4RevA)
set(G4_REV_A_STM32_PATH ${HARDWARE_SPECIFIC_PATH}/G4_RevA/G4_RevA/stm32)
set(DEVICE STM32G491CE)

add_executable(synth_G4RevA ${SRCS})

set(include_path_G4_RevA_DIRS ${INCLUDE_DIRS})

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


option(SEGGER_SYSTEMVIEW "Segger SystemView" OFF)
if(SEGGER_SYSTEMVIEW)
    include(${PROJ_PATH}/lib/SystemView/SystemView.cmake)
endif(SEGGER_SYSTEMVIEW)

include(${PROJ_PATH}/lvgl/CMakeLists.txt)
target_link_libraries(synth_G4RevA PRIVATE lvgl::lvgl)
target_include_directories(synth_G4RevA PRIVATE ${include_path_G4_RevA_DIRS})

target_compile_options(lvgl PRIVATE
    -mthumb
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    # Full debug configuration
    -Og -g3 -ggdb
    # Optimization
    # -O2 -g3 -ggdb
)

target_compile_options(${TARGET} PRIVATE
    -mthumb
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    # Full debug configuration
    -Og -g3 -ggdb
    # Optimization
    # -O2 -g3 -ggdb
)

set(symbols_SYMB_G4_RevA
    "DEBUG"
    "USE_HAL_DRIVER"
    "STM32G491xx"
)
target_compile_definitions(${TARGET} PUBLIC ${symbols_SYMB_G4_RevA})

# Linker options
set(G4_RevA_linker_script_SRC    ${HARDWARE_SPECIFIC_PATH}/G4_RevA/stm32/STM32G491CETx_FLASH.ld)
target_link_options(${TARGET} PRIVATE
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
add_custom_command(TARGET ${TARGET} POST_BUILD
COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET}>
)

# Convert output to hex and binary
add_custom_command(TARGET ${TARGET} POST_BUILD
COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${TARGET}> ${BUILD_DIRECTORY}/G4_RevA/${TARGET}.hex
)

# Convert to bin file -> add conditional check?
add_custom_command(TARGET ${TARGET} POST_BUILD
COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET}> ${BUILD_DIRECTORY}/G4_RevA/${TARGET}.bin
)

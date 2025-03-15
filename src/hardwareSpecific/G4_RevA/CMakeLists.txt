	cmake_minimum_required(VERSION 3.22)

set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

# Some default GCC settings
# arm-none-eabi- must be part of path environment
# set(TOOLCHAIN_PREFIX                /opt/homebrew/bin/arm-none-eabi-)
#set(TOOLCHAIN_PREFIX                arm-none-eabi-)
set(triple arm-linux-gnueabihf)
set(FLAGS                           "-fdata-sections -ffunction-sections -Wl,--gc-sections")
set(CPP_FLAGS                       "${FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_C_FLAGS                   ${FLAGS})
set(CMAKE_CXX_FLAGS                 ${CPP_FLAGS})
# set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}gcc)
# set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER})
# set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}g++)
# set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy)
# set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size)

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
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)

# Paths
set(HARDWARE_SPECIFIC_PATH         ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CROSSCOMPILING            1)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
message("Build type: " ${CMAKE_BUILD_TYPE})

#
# Core project settings
#
project(miniSynth)
enable_language(C CXX ASM)

set(HARDWARE_SPECIFIC_SOURCES
   ${HARDWARE_SPECIFIC_PATH}/startup_stm32g491xx.s
   ${HARDWARE_SPECIFIC_PATH}/hardwareSpecific.c
   ${HARDWARE_SPECIFIC_PATH}/interrupts.cpp
   ${HARDWARE_SPECIFIC_PATH}/audio/AudioI2SInterface_hardwareSpecific.cpp
   ${HARDWARE_SPECIFIC_PATH}/drv/drv_GPIO_hardwareSpecific.c
   ${HARDWARE_SPECIFIC_PATH}/drv/drv_I2C_hardwareSpecific.c
   ${HARDWARE_SPECIFIC_PATH}/drv/drv_SPI_hardwareSpecific.c
   ${HARDWARE_SPECIFIC_PATH}/drv/drv_encoder_hardwareSpecific.c
   ${HARDWARE_SPECIFIC_PATH}/devices/LEDs_hardwareSpecific.c
   ${HARDWARE_SPECIFIC_PATH}/devices/button_hardwareSpecific.c
   ${HARDWARE_SPECIFIC_PATH}/devices/knob_hardwareSpecific.c
   ${HARDWARE_SPECIFIC_PATH}/devices/PCA9555_hardwareSpecific.c
   ${HARDWARE_SPECIFIC_PATH}/devices/PCA9685_hardwareSpecific.c
)

set(STM32_HARDWARE_SPECIFIC_SOURCES
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/system_stm32g4xx.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/adc.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/cordic.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/dma.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/fmac.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/gpio.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/i2c.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/i2s.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/iwdg.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/rng.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/spi.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/tim.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/wwdg.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/stm32g4xx_it.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/stm32g4xx_hal_msp.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/stm32g4xx_hal_timebase_tim.c
    # ${HARDWARE_SPECIFIC_PATH}/stm32/FATFS/App/app_fatfs.c
    # ${HARDWARE_SPECIFIC_PATH}/stm32/FATFS/Target/user_diskio.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/USB_Device/App/usb_device.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/USB_Device/App/usbd_desc.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/USB_Device/App/usbd_dfu_flash.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/USB_Device/Target/usbd_conf.c
)

set(STM32G4_DRIVER_SOURCES_X
   # ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/Third_Party/FatFs/src/diskio.c
   # ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/Third_Party/FatFs/src/ff.c
   # ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c
   # ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/Third_Party/FatFs/src/option/syscall.c
   ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
   ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
   ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
   # ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Src/usbd_customhid.c
   ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/ST/STM32_USB_Device_Library/Class/DFU/Src/usbd_dfu.c
)

set(G4_REV_A_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${HARDWARE_SPECIFIC_PATH}/
    ${HARDWARE_SPECIFIC_PATH}/audio
    ${HARDWARE_SPECIFIC_PATH}/devices
    ${HARDWARE_SPECIFIC_PATH}/drv
    ${HARDWARE_SPECIFIC_PATH}/stm32
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Inc
    # ${HARDWARE_SPECIFIC_PATH}/stm32/FATFS/Target
    # ${HARDWARE_SPECIFIC_PATH}/stm32/FATFS/App
    # ${HARDWARE_SPECIFIC_PATH}/stm32/FATFS/
    ${HARDWARE_SPECIFIC_PATH}/stm32/USB_Device/App
    ${HARDWARE_SPECIFIC_PATH}/stm32/USB_Device/Target
    ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    # ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Inc
    ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/ST/STM32_USB_Device_Library/Class/DFU/Inc/
    # ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/Third_Party/FatFs/
    # ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/Third_Party/FatFs/src/
)

set(TARGET miniSynthG4)
set(DEVICE STM32G491CE)

include(${CMAKE_CURRENT_SOURCE_DIR}/stm32/CMakeLists.txt)

add_executable(
    ${TARGET}
    ${COMMON_SOURCES}
    ${HARDWARE_SPECIFIC_SOURCES}
    ${STM32_HARDWARE_SPECIFIC_SOURCES}
    ${STM32G4_DRIVER_SOURCES_X}
    ${STM32G4_DRIVER_SOURCES}
)

target_include_directories(${TARGET} PRIVATE
    ${COMMON_INCLUDE_DIRS}
    ${G4_REV_A_INCLUDE_DIRS}
    ${STM32G4_DRIVER_INCLUDES}
)

# LVGL lib
target_link_libraries(${TARGET} PRIVATE lvgl::lvgl)

# ARM DSP lib
target_link_libraries(${TARGET} PRIVATE "${HARDWARE_SPECIFIC_PATH}/stm32/Drivers/CMSIS/DSP/Lib/GCC/libarm_cortexM4lf_math.a")

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
    "LV_CONF_INCLUDE_SIMPLE"
)
target_compile_definitions(${TARGET} PUBLIC ${symbols_SYMB_G4_RevA})

# Linker options
set(G4_RevA_linker_script_SRC    ${HARDWARE_SPECIFIC_PATH}/stm32/STM32G491CETx_FLASH.ld)
target_link_options(${TARGET} PRIVATE
    -T${G4_RevA_linker_script_SRC}
    -mthumb
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -Wl,-Map=${TARGET}.map
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
COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${TARGET}> ${TARGET}.hex
)

# Convert to bin file -> add conditional check?
add_custom_command(TARGET ${TARGET} POST_BUILD
COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET}> ${TARGET}.bin
)

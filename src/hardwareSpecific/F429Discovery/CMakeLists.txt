	cmake_minimum_required(VERSION 3.22)

set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

# Some default GCC settings
set(triple arm-linux-gnueabihf)
set(FLAGS                           "-fdata-sections -ffunction-sections -Wl,--gc-sections")
set(CPP_FLAGS                       "${FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_C_FLAGS                   ${FLAGS})
set(CMAKE_CXX_FLAGS                 ${CPP_FLAGS})

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
   ${HARDWARE_SPECIFIC_PATH}/startup_stm32f429xx.s
   ${HARDWARE_SPECIFIC_PATH}/hardwareSpecific.c
)

set(STM32_HARDWARE_SPECIFIC_SOURCES
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/system_stm32f4xx.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/gpio.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/crc.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/iwdg.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/stm32f4xx_it.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/stm32f4xx_hal_msp.c
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Src/stm32f4xx_hal_timebase_tim.c
    ${HARDWARE_SPECIFIC_PATH}/drv/drv_GPIO_hardwareSpecific.c
    ${HARDWARE_SPECIFIC_PATH}/devices/LEDs_hardwareSpecific.c
    )

set(STM32F4_DRIVER_SOURCES_X
#    ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
#    ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
#    ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
#    ${HARDWARE_SPECIFIC_PATH}/stm32/Middlewares/ST/STM32_USB_Device_Library/Class/DFU/Src/usbd_dfu.c
)

set(F4_REV_A_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${HARDWARE_SPECIFIC_PATH}/
    ${HARDWARE_SPECIFIC_PATH}/devices
    ${HARDWARE_SPECIFIC_PATH}/drv
    ${HARDWARE_SPECIFIC_PATH}/stm32
    ${HARDWARE_SPECIFIC_PATH}/stm32/Core/Inc
)

set(TARGET F429Discovery)
set(DEVICE STM32F429xx)

include(${CMAKE_CURRENT_SOURCE_DIR}/stm32/CMakeLists.txt)

add_executable(
    ${TARGET}
    ${COMMON_SOURCES}
    ${HARDWARE_SPECIFIC_SOURCES}
    ${STM32_HARDWARE_SPECIFIC_SOURCES}
    ${STM32F4_DRIVER_SOURCES_X}
    ${STM32F4_DRIVER_SOURCES}
)

target_include_directories(${TARGET} PRIVATE
    ${COMMON_INCLUDE_DIRS}
    ${F4_REV_A_INCLUDE_DIRS}
    ${STM32F4_DRIVER_INCLUDES}
)

# LVGL lib
target_link_libraries(${TARGET} PRIVATE lvgl::lvgl)

# ARM DSP lib
# target_link_libraries(${TARGET} PRIVATE "${HARDWARE_SPECIFIC_PATH}/stm32/Drivers/CMSIS/DSP/Lib/GCC/libarm_cortexM4lf_math.a")

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

set(symbols_SYMB_F4_RevA
    "DEBUG"
    "USE_HAL_DRIVER"
    "STM32F429xx"
    "LV_CONF_INCLUDE_SIMPLE"
)
target_compile_definitions(${TARGET} PUBLIC ${symbols_SYMB_F4_RevA})

# Linker options
set(F4_RevA_linker_script_SRC    ${HARDWARE_SPECIFIC_PATH}/stm32/STM32F429ZITx_FLASH.ld)
target_link_options(${TARGET} PRIVATE
    -T${F4_RevA_linker_script_SRC}
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

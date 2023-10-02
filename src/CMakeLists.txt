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
set(CMAKE_BUILD_TYPE                "Debug")
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})
set(STM32_PATH                      ${CMAKE_CURRENT_SOURCE_DIR}/stm32)
set(FREERTOS_PATH                   ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS)
set(BUILD_DIRECTORY                 "../build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${BUILD_DIRECTORY})
message("Build type: "              ${CMAKE_BUILD_TYPE})

#
# Core project settings
#
project(synth)
enable_language(C CXX ASM)

#
# Core MCU flags, CPU, instruction set and FPU setup
# Needs to be set properly for your MCU
#
set(CPU_PARAMETERS
    -mthumb
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

# Set linker script
set(linker_script_SRC               ${STM32_PATH}/STM32F429ZITx_FLASH.ld)
set(EXECUTABLE                      ${CMAKE_PROJECT_NAME})
set(FORMATTER                       ${CMAKE_PROJECT_NAME}_FORMATTER)

#
# List of source files to compile
#
set(sources_SRCS
    ${PROJ_PATH}/main.cpp
    ${PROJ_PATH}/devices/LEDs.cpp
    ${PROJ_PATH}/devices/sgtl5000.cpp
    ${PROJ_PATH}/audio/mixer.cpp
    ${PROJ_PATH}/utils/util_rollingBuffer.cpp
    ${PROJ_PATH}/startup_stm32f429xx.s
    ${STM32_PATH}/Core/Src/gpio.c
    ${STM32_PATH}/Core/Src/dma2d.c
    ${STM32_PATH}/Core/Src/i2c.c
    ${STM32_PATH}/Core/Src/i2s.c
    ${STM32_PATH}/Core/Src/sdio.c
    ${STM32_PATH}/Core/Src/spi.c
    ${STM32_PATH}/Core/Src/tim.c
    ${STM32_PATH}/Core/Src/usart.c
    ${STM32_PATH}/Core/Src/stm32f4xx_it.c
    ${STM32_PATH}/Core/Src/stm32f4xx_hal_timebase_tim.c
    ${STM32_PATH}/Core/Src/stm32f4xx_hal_msp.c
    ${STM32_PATH}/Core/Src/system_stm32f4xx.c
    ${STM32_PATH}/USB_DEVICE/App/usb_device.c
    ${STM32_PATH}/USB_DEVICE/App/usbd_desc.c
    ${STM32_PATH}/USB_DEVICE/App/usbd_audio_if.c
    ${STM32_PATH}/USB_DEVICE/Target/usbd_conf.c
    ${STM32_PATH}/Core/Src/adc.c
    ${STM32_PATH}/Core/Src/dma.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s_ex.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma2d.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
    ${STM32_PATH}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
    ${STM32_PATH}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
    ${STM32_PATH}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
    ${STM32_PATH}/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Src/usbd_audio.c
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

#
# Include directories
#
set(include_path_DIRS
    ${PROJ_PATH}/
    ${PROJ_PATH}/audio
    ${PROJ_PATH}/wavetables
    ${PROJ_PATH}/devices/
    ${PROJ_PATH}/utils/
    ${STM32_PATH}/
    ${STM32_PATH}/Core/Inc
    ${STM32_PATH}/USB_DEVICE/App
    ${STM32_PATH}/USB_DEVICE/Target
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Inc
    ${STM32_PATH}/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    ${STM32_PATH}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    ${STM32_PATH}/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc
    ${STM32_PATH}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${STM32_PATH}/Drivers/CMSIS/Include
    ${STM32_PATH}/Drivers/CMSIS/Core/Include
    ${STM32_PATH}/Drivers/CMSIS/Core_A/Include
    ${STM32_PATH}/Drivers/CMSIS/DSP/Include
    ${FREERTOS_PATH}/include
    ${FREERTOS_PATH}/portable/GCC/ARM_CM4F
    ${FREERTOS_PATH}/portable/MemMag
)

#
# Symbols definition
#
set(symbols_SYMB
    "DEBUG"
    "STM32F429xx"
    "USE_HAL_DRIVER"
)

# Executable files
add_executable(${EXECUTABLE} ${sources_SRCS})

# Include paths
target_include_directories(${EXECUTABLE} PRIVATE ${include_path_DIRS})

# Project symbols
target_compile_definitions(${EXECUTABLE} PRIVATE ${symbols_SYMB})

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    # Full debug configuration
    -Og -g3 -ggdb
)

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
    -T${linker_script_SRC}
    ${CPU_PARAMETERS}
    -Wl,-Map=${BUILD_DIRECTORY}/${CMAKE_PROJECT_NAME}.map
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

# Execute pre-build formatting
message(STATUS "Formatting [*.cpp, *.hpp, *.h]...")
execute_process(
    COMMAND "clang-format -i src/devices/*.cpp"
)
message(STATUS "Formatting complete.")

# Execute post-build to print size
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

# Convert output to hex and binary
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${BUILD_DIRECTORY}/${EXECUTABLE}.hex
)

# Convert to bin file -> add conditional check?
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${BUILD_DIRECTORY}/${EXECUTABLE}.bin
)

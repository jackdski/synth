cmake_minimum_required(VERSION 3.22)

set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

# Some default GCC settings
# arm-none-eabi- must be part of path environment
# set(TOOLCHAIN_PREFIX                /opt/homebrew/bin/arm-none-eabi-)
set(TOOLCHAIN_PREFIX                arm-none-eabi-)
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
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)

# Paths
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(HARDWARE_SPECIFIC_PATH          ${CMAKE_CURRENT_SOURCE_DIR}/src/hardwareSpecific)
set(FREERTOS_PATH                   ${CMAKE_CURRENT_SOURCE_DIR}/src/FreeRTOS)
set(BOOTLOADER_PATH                 ${CMAKE_CURRENT_SOURCE_DIR}/src/bootloader)

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

set(COMMON_SOURCES
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
    ${PROJ_PATH}/display/screens/keyboardScreen.cpp
    ${PROJ_PATH}/misc/misc.cpp
    ${PROJ_PATH}/misc/knobControls.c
    ${PROJ_PATH}/LEDManager/LEDManager.cpp
    ${PROJ_PATH}/audio/samples/hi_hat_0.cpp
    ${PROJ_PATH}/audio/samples/hi_hat_1.cpp
    ${PROJ_PATH}/audio/samples/hi_hat_2.cpp
    ${PROJ_PATH}/audio/samples/kick_0.cpp
    ${PROJ_PATH}/audio/samples/GlobalSamples.cpp
    ${PROJ_PATH}/wavetables/wavetables.cpp
    ${PROJ_PATH}/audio/effects/lpf.c
    ${PROJ_PATH}/audio/keyboard.cpp
    ${PROJ_PATH}/audio/adsr.c
    ${PROJ_PATH}/audio/audio.cpp
    ${PROJ_PATH}/audio/AudioManager.cpp
    ${PROJ_PATH}/audio/sequencer.cpp
    ${PROJ_PATH}/audio/MIDI.cpp
    ${PROJ_PATH}/utils/cpu_utils.c
    ${PROJ_PATH}/lvgl/src/themes/mono/lv_theme_mono.c
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

set(COMMON_INCLUDE_DIRS
    ${PROJ_PATH}/
    ${PROJ_PATH}/misc/
    ${PROJ_PATH}/display/screens/
    ${PROJ_PATH}/display/sub_screens/
    ${PROJ_PATH}/display/
    ${PROJ_PATH}/wavetables/
    ${PROJ_PATH}/LEDManager/
    ${PROJ_PATH}/audio/
    ${PROJ_PATH}/audio/effects/
    ${PROJ_PATH}/audio/samples/
    ${PROJ_PATH}/devices/
    ${PROJ_PATH}/drv/
    ${PROJ_PATH}/utils/
    ${PROJ_PATH}/lib/
    ${PROJ_PATH}/lvgl/src/themes
    ${PROJ_PATH}/lvgl/src/themes/mono
    ${FREERTOS_PATH}/portable/MemMag
    ${FREERTOS_PATH}/portable/GCC/ARM_CM4F
    ${FREERTOS_PATH}/include
)

set(LV_CONF_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lv_conf.h
    CACHE STRING "" FORCE
)
set(LV_CONF_INCLUDE_SIMPLE 1)
set(BUILD_SHARED_LIBS OFF)

# LVGL lib
include(${PROJ_PATH}/lvgl/CMakeLists.txt)

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
    # -Og -g3 -ggdb
    # Optimization
    -O2 -g3 -ggdb
)

# stm32g4 - miniSynth + bootloader
add_subdirectory(${HARDWARE_SPECIFIC_PATH}/G4_RevA)
add_subdirectory(${BOOTLOADER_PATH}/stm32g4)


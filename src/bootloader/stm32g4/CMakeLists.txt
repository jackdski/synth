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
set(HARDWARE_SPECIFIC_PATH_G4      ${PROJ_PATH}/hardwareSpecific/G4_RevA)
set(STM32_HARDWARE_SPECIFIC_PATH   ${HARDWARE_SPECIFIC_PATH_G4}/stm32)
set(BOOT_STM32_PATH                ${CMAKE_CURRENT_SOURCE_DIR}/stm32)

set(CMAKE_CROSSCOMPILING            1)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
message("Build type: " ${CMAKE_BUILD_TYPE})

#
# Core project settings
#
project(synth_bootloader)
enable_language(C CXX ASM)

set(BOOT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/startup_stm32g491xx.s
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${BOOT_STM32_PATH}/Core/Src/system_stm32g4xx.c
    ${BOOT_STM32_PATH}/Core/Src/gpio.c
    ${BOOT_STM32_PATH}/Core/Src/iwdg.c
    ${BOOT_STM32_PATH}/Core/Src/stm32g4xx_it.c
    ${BOOT_STM32_PATH}/Core/Src/stm32g4xx_hal_msp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_Device/App/usb_device.c
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_Device/App/usbd_desc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_Device/App/usbd_dfu_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_Device/Target/usbd_conf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/DFU/Src/usbd_dfu.c
)

set(BOOT_G4_REV_A_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${BOOT_STM32_PATH}/
    ${BOOT_STM32_PATH}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_Device/App
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_Device/Target
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/DFU/Inc/
)

set(TARGET synth_G4Bootloader)
set(DEVICE STM32G491CE)

include(${STM32_HARDWARE_SPECIFIC_PATH}//CMakeLists.txt)

add_executable(
    ${TARGET}
    ${BOOT_SOURCES}
    ${STM32G4_DRIVER_SOURCES}
)

target_include_directories(${TARGET} PRIVATE
    ${BOOT_G4_REV_A_INCLUDE_DIRS}
    ${STM32G4_DRIVER_INCLUDES}
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
    # -Og -g3 -ggdb
    # Optimization
    -O2 -g3 -ggdb
)

set(symbols_SYMB_G4_RevA
    "DEBUG"
    "USE_HAL_DRIVER"
    "STM32G491xx"
)
target_compile_definitions(${TARGET} PUBLIC ${symbols_SYMB_G4_RevA})

# Linker options
set(G4_REVA_LINKER_SCRIPT_SRC    ${CMAKE_CURRENT_SOURCE_DIR}/STM32G491CETx_FLASH.ld)
target_link_options(${TARGET} PRIVATE
    -T${G4_REVA_LINKER_SCRIPT_SRC}
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

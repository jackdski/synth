#!/bin/bash

CORES=16

cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --no-warn-unused-cli -S/synth -B/synth/build

if [ -z "$1" ]; then
    # if no target specified, build everything
    declare -a targets=("miniSynthG4"
                        "synth_G4Bootloader"
                       )
else
    targets="$@"
fi

# build all specified targets
for target in $targets
do
    echo "Building ${target}..."
    cmake --build /synth/build --target ${target} --config Debug -j${CORES}
    if [ ! -d /synth/build/${target} ]; then
        mkdir /synth/build/${target}
    fi

    # copy all .hex, .bin, .elf, .map files into target directories
    case ${target} in
    "miniSynthG4")
        if [ -f "/synth/build/src/hardwareSpecific/G4_RevA/miniSynthG4.hex" ]; then
            cp /synth/build/src/hardwareSpecific/G4_RevA/miniSynthG4.* /synth/build/miniSynthG4/
        fi
        ;;
    "synth_G4Bootloader")
        if [ -f "/synth/build/src/bootloader/stm32g4/synth_G4Bootloader.hex" ]; then
            cp /synth/build/src/bootloader/stm32g4/synth_G4Bootloader.* /synth/build/synth_G4Bootloader/
        fi
        ;;
    *)
        echo "Error: No target specified"
        ;;
    esac
done


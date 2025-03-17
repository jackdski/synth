#!/bin/bash

CORES=16

declare -a build_targets=("miniSynthG4"
                          "synth_G4Bootloader"
                         )

quiet_output=0

usage() {
    echo "Usage: $0 [OPTIONS] [TARGET]"
    echo "Options:"
    echo " -h,       Display this help message"
}

while test $# != 0
do
    case "$1" in
    -h) echo "Possible targets:"
        printf '\t%s\n' "${build_targets[@]}"
        exit
    ;;

    *)  usage
        exit
    ;;
    esac
    shift
done

if [ -z "$1" ]; then
    # if no target specified, build everything
    declare -a targets=${build_targets[@]}
else
    targets="$@"
fi

# build all specified targets
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --no-warn-unused-cli -S/synth -B/synth/build

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


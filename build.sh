#!/bin/bash

CORES=16
build_type=Debug
declare -a targets=()

declare -a possible_targets=("miniSynthG4"
                          "synth_G4Bootloader"
                          "F429Discovery"
                          "synth_F4Bootloader"
                         )


usage() {
    echo "Usage: $0 [OPTIONS] [TARGET]"
    echo "Targets:"
    printf '\t%s\n' "${possible_targets[@]}"
    echo "Options:"
    echo " -h,       Display this help message"
}

if (($# > 0)); then
    for arg in "$@";
    do
        case $arg in
        -h) usage
            exit
            ;;
        -r) build_type=Release
            ;;
        *) targets+=($arg)
            ;;
        esac
        shift
    done
fi

if ((${#targets[@]} == 0)); then
    targets=${possible_targets[@]}
fi

printf "Targets: %s\n\n" "${targets}"

# build all specified targets
cmake -DCMAKE_BUILD_TYPE:STRING=${build_type} -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --no-warn-unused-cli -S/synth -B/synth/build

for target in $targets
do
    echo "Building ${target}..."
    cmake --build /synth/build --target ${target} --config ${build_type} -j${CORES}
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
    "F429Discovery")
        if [ -f "/synth/build/src/hardwareSpecific/F429Discovery/F429Discovery.hex" ]; then
            cp /synth/build/src/hardwareSpecific/F429Discovery/F429Discovery.* /synth/build/F429Discovery/
        fi
        ;;
    "synth_F4Bootloader")
        if [ -f "/synth/build/src/bootloader/stm32g4/synth_F4Bootloader.hex" ]; then
            cp /synth/build/src/bootloader/stm32g4/synth_F4Bootloader.* /synth/build/synth_F4Bootloader/
        fi
        ;;
    *)
        echo "Error: No target specified"
        ;;
    esac
done

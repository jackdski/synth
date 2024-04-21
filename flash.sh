#!/bin/sh
case "$1" in
    "G4")
        echo "Flashing G4_RevA"
        st-flash write ./build/G4_RevA/synth.bin 0x08000000
        st-flash --format ihex write ./build/G4_RevA/synth.hex
    ;;
    "F4")
        echo "Flashing F429Discovery"
        st-flash write ./build/F429Discovery/synth.bin 0x08000000
        st-flash --format ihex write ./build/F429Discovery/synth.hex
    ;;
    *)
        echo "`basename ${0}`: options [G4, F4]"
        exit 1
    ;;
esac

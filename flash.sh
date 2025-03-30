#!/bin/sh
case "$1" in
    "G4")
        echo "Flashing G4_RevA"
        # st-flash write ./build/build/G4_RevA/synth_G4RevA.bin 0x08000000
        # st-flash --format ihex write ./build/build/G4_RevA/synth_G4RevA.bin
        # Work with a device in DFU mode with vendor ID 0x0951 and product ID 0x0026
        dfu-util -a 0 -s 0x0800e800:leave -D ./build/src/hardwareSpecific/G4_RevA/miniSynthG4.bin # --device ,951:26
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

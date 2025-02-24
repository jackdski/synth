#ifndef AUDIO_I2S_INTERFACE_HARDWARE_SPECIFIC_H_
#define AUDIO_I2S_INTERFACE_HARDWARE_SPECIFIC_H_

#include <stdint.h>
#include "AudioI2SInterface.hpp"

namespace Audio
{

class Audio_I2SInterface : public Audio_I2SInterfaceBase
{
public:
    using Audio_I2SInterfaceBase::Audio_I2SInterfaceBase;

    void i2sTransmit(uint16_t *sampleBlock, uint32_t numSamples);
    void i2sStop(uint16_t *sampleBlock, uint32_t numSamples);
};

}

#endif // AUDIO_I2S_INTERFACE_HARDWARE_SPECIFIC_H_
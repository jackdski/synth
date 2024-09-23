#ifndef AUDIO_HARDWARE_SPECIFIC_HPP_
#define AUDIO_HARDWARE_SPECIFIC_HPP_

#include "AudioI2SInterface.hpp"

#include <stdint.h>

namespace Audio
{

class Audio_I2SInterface : public Audio_I2SInterfaceBase
{
public:
    void i2sTransmit (uint16_t *sampleBlock, uint32_t numSamples) override;
    void i2sStop(uint16_t *sampleBlock, uint32_t numSamples) override;
};

}

#endif

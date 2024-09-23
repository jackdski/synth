#ifndef AUDIO_I2S_INTERFACE_H_
#define AUDIO_I2S_INTERFACE_H_

#include <stdint.h>

namespace Audio
{

class Audio_I2SInterfaceBase
{
public:
    virtual void i2sTransmit(uint16_t *sampleBlock, uint32_t numSamples)
    {
    }

    virtual void i2sStop(uint16_t *sampleBlock, uint32_t numSamples)
    {
    }
};

}

#endif // AUDIO_I2S_INTERFACE_H_
#ifndef AUDIO_I2S_INTERFACE_HPP_
#define AUDIO_I2S_INTERFACE_HPP_

#include <stdint.h>

namespace Audio
{

class Audio_I2SInterfaceBase
{
public:
    void (* txHalfCompleteCallback)(void);
    void (* txCompleteCallback)(void);

    void i2sTransmit(uint16_t *sampleBlock, uint32_t numSamples);
    void i2sStop(uint16_t *sampleBlock, uint32_t numSamples);

    Audio_I2SInterfaceBase(void (*txHalfCompleteCallback)(void), void (*txCompleteCallback)(void)) : txHalfCompleteCallback(txHalfCompleteCallback), txCompleteCallback(txCompleteCallback)
    {
    }
};

}

#endif // AUDIO_I2S_INTERFACE_HPP_
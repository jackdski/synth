#ifndef AUDIO_HARDWARE_SPECIFIC_H_
#define AUDIO_HARDWARE_SPECIFIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void audio_hardwareSpecific_i2sTransmit(uint16_t *sampleBlock, uint32_t numSamples);
void audio_hardwareSpecific_i2sStop(void);

#ifdef __cplusplus
}
#endif

#endif // AUDIO_HARDWARE_SPECIFIC_H_

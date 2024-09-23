// #include "audio.hpp"

#ifdef __cplusplus
extern "C" {
#endif

/* I N C L U D E S */

#include "features.h"

#if (FEATURE_MIXER || FEATURE_AUDIO)

#include "audio_hardwareSpecific.h"
#include "i2s.h"
// #include "mixer.h"

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* D A T A   D E F I N I T I O N S */

extern I2S_HandleTypeDef hi2s2;

/* P R I V A T E   F U N C T I O N S */

/* P U B L I C   F U N C T I O N S */

void audio_hardwareSpecific_i2sTransmit(uint16_t *sampleBlock, uint32_t numSamples)
{
    (void)HAL_I2S_Transmit_DMA(&hi2s2, sampleBlock, numSamples);
}

void audio_hardwareSpecific_i2sStop(void)
{
    (void)HAL_I2S_DMAStop(&hi2s2);
}

// void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
// {
//     if (hi2s == &hi2s2)
//     {
//         audio_txHalfCompleteCallback();
//     }
// }

// void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
// {
//     if (hi2s == &hi2s2)
//     {
//         audio_txCompleteCallback();
//     }
// }

#ifdef __cplusplus
}
#endif

#endif  // FEATURE_MIXER || FEATURE_AUDIO

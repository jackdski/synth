#include "mixer.h"

/* I N C L U D E S */

#include "FreeRTOS.h"
#include "task.h"

#include "constants.h"

#include "i2c.h"
#include "sgtl5000.h"

#if (FEATURE_MIXER)

/* D E F I N E S */

#define HALF_OF_MIXER_SAMPLES_PER_BLOCK (MIXER_SAMPLES_PER_BLOCK / 2U)

// #define I2S_DATA_FORMAT_MAX_VALUE       8388608.0f  // 24bit - 2^24 / 2
#define I2S_DATA_FORMAT_MAX_VALUE       32768.0f  // 16bit

#define I2S_BUFFER_HALFWAY_INDEX        (MIXER_SAMPLES_PER_BLOCK * MIXER_NUMBER_OF_CHANNELS / 2U)
#define I2S_BUFFER_SIZE                 (uint32_t)(MIXER_SAMPLES_PER_BLOCK * 2U)

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* T Y P E D E F S */

typedef struct
{
    bool enabled;
    bool initialized;

    bool middleCTestActive;

    float volume;
    float gain;

    // effects
    Oscillator_E lfoChannel;
    LPF_S lpf;
} Mixer_data_S;

/* D A T A   D E F I N I T I O N S */

static Mixer_data_S mixer;

/* P R I V A T E   F U N C T I O N S */

/* P U B L I C   F U N C T I O N S */

void Mixer_init(void)
{
    memset(&mixer, 0U, sizeof(mixer));

    mixer.lfoChannel = OSCILLATOR_LFO;
    mixer.volume     = 0.3F;
    mixer.gain       = 0.5F;
    mixer.enabled    = true;

    bool initialized = true;

    // codec
    initialized &= SGTL5000_init();
    initialized &= SGTL5000_configure();
    mixer.initialized = initialized;

    // effects
    LPF_init(&mixer.lpf, 1000.0F, (1.0f / SYNTH_SAMPLE_FREQUENCY));
}

/* P R I V A T E   F U N C T I O N S */

/* P U B L I C   F U N C T I O N S */

void Mixer_updateInputs(void)
{
    // SGTL5000_pollRegisters();
    SGTL5000_updateVolume(mixer.volume);
}

void Mixer_updateVolume(float volume)
{
    mixer.volume = volume;
}

void Mixer_updateSampleBlock(uint16_t *sampleBlock, const bool firstHalf)
{
    const uint32_t startIndex = (firstHalf) ? 0U : (I2S_BUFFER_HALFWAY_INDEX);
    const uint32_t endIndex   = (firstHalf) ? (I2S_BUFFER_HALFWAY_INDEX) : (I2S_BUFFER_SIZE);

    for (uint32_t i = startIndex; i < endIndex; i += MIXER_NUMBER_OF_CHANNELS)
    {
        // update all notes
        float sample         = 0.0f;
        float numActiveNotes = 0.0f;

        for (Note_channel_E noteChannel = 0U; noteChannel < NOTE_CHANNEL_COUNT; noteChannel++)
        {
            if (Note_isActive(noteChannel))
            {
                sample += Note_update(noteChannel);
                numActiveNotes += 1.0f;
            }
        }

        // apply affects
        if (oscillator_getFrequency(mixer.lfoChannel) > 0.0f)
        {
            oscillator_getSample(mixer.lfoChannel);
            numActiveNotes += 1.0f;
        }

        sample = LPF_update(&mixer.lpf, sample);

        // divide sample by number of active notes
        sample /= numActiveNotes;

        // sample as a 16bit value
        const uint16_t value = (uint16_t)(sample * mixer.volume) * I2S_DATA_FORMAT_MAX_VALUE;
        sampleBlock[i]       = value;  // left
        sampleBlock[i + 1U]   = value; // right
    }
}

void Mixer_setEnable(const bool enable)
{
    mixer.enabled = enable;
}

bool Mixer_isEnabled(void)
{
    return mixer.enabled;
}

bool Mixer_isInitialized(void)
{
    return mixer.initialized;
}

#endif  // FEATURE_MIXER

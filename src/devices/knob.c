
/* I N C L U D E S */

#include "knob.h"

/* D E F I N E S */

/* T Y P E D E F S */

typedef struct
{
    Knob_config_S *config;
} Knob_data_S;

/* P R I V A T E   D A T A   D E F I N I T I O N S */

static Knob_data_S Knob_data;

/* P U B L I C   F U N C T I O N S */

void Knob_init(Knob_config_S *config)
{
    if (config != NULL)
    {
        Knob_data.config = config;
    }
}

uint32_t Knob_getCount(const Knob_channel_E channel)
{
    return drv_encoder_getCount(Knob_data.config->channelConfig[channel].encoderChannel);
}

// get direction: __HAL_TIM_IS_TIM_COUNTING_DOWN

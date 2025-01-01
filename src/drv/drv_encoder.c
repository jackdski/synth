/* I N C L U D E S */

#include "drv_encoder.h"

#if (FEATURE_ENCODER)
#include "tim.h"

/* D E F I N E S */

#define MAX_UNSIGNED_INT_32_BIT         (4294967296U)
#define HALF_MAX_UNSIGNED_INT_32_BIT    (4294967296U / 2U)

/* T Y P E D E F S */

typedef struct
{
    int32_t previousCount[DRV_ENCODER_CHANNEL_COUNT];
} drv_encoder_data_S;


/* P R I V A T E   D A T A   D E F I N I T I O N S */

static drv_encoder_data_S data;

/* P U B L I C   F U N C T I O N S */

void drv_encoder_init(void)
{
    MX_TIM2_Init();
    MX_TIM3_Init();

    drv_encoder_hardwareSpecific_init();
}

void drv_encoder_update(const drv_encoder_channel_E channel)
{
    data.previousCount[channel] = (drv_encoder_getCount(channel) / 2U);
}

int32_t drv_encoder_updateAndGetDiff(const drv_encoder_channel_E channel)
{
    const uint32_t currentCount = (drv_encoder_getCount(channel) / 2U);
    const uint32_t prevCount = data.previousCount[channel];

    const int32_t diff = (prevCount + HALF_MAX_UNSIGNED_INT_32_BIT - currentCount) % MAX_UNSIGNED_INT_32_BIT - HALF_MAX_UNSIGNED_INT_32_BIT;
    data.previousCount[channel] = currentCount;
    return diff;
}

// get direction: __HAL_TIM_IS_TIM_COUNTING_DOWN
#endif  // FEATURE_ENCODER

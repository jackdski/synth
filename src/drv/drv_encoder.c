/* I N C L U D E S */

#include "drv_encoder.h"
#include "tim.h"

/* D E F I N E S */

/* T Y P E D E F S */

/* P R I V A T E   D A T A   D E F I N I T I O N S */

/* P U B L I C   F U N C T I O N S */

void drv_encoder_init(void)
{
    MX_TIM2_Init();
    MX_TIM3_Init();

    drv_encoder_hardwareSpecific_init();
}

// get direction: __HAL_TIM_IS_TIM_COUNTING_DOWN

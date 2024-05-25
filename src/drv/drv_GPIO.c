/* I N C L U D E S */

#include "drv_GPIO.h"
#include "gpio.h"

/* D E F I N E S */


/* T Y P E D E F S */

typedef struct
{
    drv_GPIO_state_E state;
} drv_GPIO_hardwareSpecific_channelData_S;

typedef struct
{
    drv_GPIO_hardwareSpecific_channelData_S channelData[DRV_GPIO_CHANNEL_COUNT];
} drv_GPIO_hardwareSpecific_data_S;

/* P R I V A T E   D A T A   D E F I N I T I O N S */

drv_GPIO_hardwareSpecific_data_S drv_GPIO_hardwareSpecific_data;


/* P U B L I C   F U N C T I O N S */

void drv_GPIO_init(void)
{
    MX_GPIO_Init();
}

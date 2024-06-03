#ifndef DRV_GPIO_BASE_HPP_
#define DRV_GPIO_BASE_HPP_

/* I N C L U D E S */

#include "features.h"

#include "Utils.h"
#include "drv_GPIO_hardwareSpecific.h"
#include "main.h"

#if FEATURE_GPIO

/* D E F I N E S */

/* T Y P E D E F S */

typedef enum
{
    DRV_GPIO_TYPE_INPUT,
    DRV_GPIO_TYPE_OUTPUT,
} drv_GPIO_type_E;

typedef enum
{
    DRV_GPIO_LOW,
    DRV_GPIO_HIGH,
} drv_GPIO_state_E;

typedef struct
{
    GPIO_TypeDef *gpio;
    uint16_t pin;
    drv_GPIO_type_E type;
    bool activeLow;
    drv_GPIO_state_E initState;
} drv_GPIO_channelConfig_S;

typedef struct
{
    drv_GPIO_channelConfig_S *channelConfig;
    uint32_t channelCount;
} drv_GPIO_config_S;

/* P U B L I C   F U N C T I O N S */

void drv_GPIO_init(void);
void drv_GPIO_update(void);
void drv_GPIO_setOutput(drv_GPIO_channel_E channel, drv_GPIO_state_E state);
void drv_GPIO_toggleOutput(drv_GPIO_channel_E channel);
drv_GPIO_state_E drv_GPIO_getInput(drv_GPIO_channel_E channel);

#endif  // FEATURE_GPIO
#endif  // DRV_GPIO_BASE_HPP_

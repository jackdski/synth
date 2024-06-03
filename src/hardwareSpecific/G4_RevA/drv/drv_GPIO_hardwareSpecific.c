#include "drv_GPIO.h"

#include <stdbool.h>

#if FEATURE_GPIO

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

static drv_GPIO_hardwareSpecific_data_S drv_GPIO_hardwareSpecific_data;

// clang-format off

static drv_GPIO_channelConfig_S drv_GPIO_channelConfig[DRV_GPIO_CHANNEL_COUNT] = {
    [DRV_GPIO_CHANNEL_SPI_SD_CARD_CS] =
    {
        .gpio       = GPIOB,
        .pin        = GPIO_PIN_0,
        .type       = DRV_GPIO_TYPE_OUTPUT,
        .activeLow  = true,
        .initState  = DRV_GPIO_HIGH,
    },
    [DRV_GPIO_CHANNEL_LCD_DC] =
    {
        .gpio       = GPIOB,
        .pin        = GPIO_PIN_1,
        .type       = DRV_GPIO_TYPE_OUTPUT,
        .activeLow  = false,
        .initState  = DRV_GPIO_HIGH,
    },
    [DRV_GPIO_CHANNEL_BUTTON_A] =
    {
        .gpio       = GPIOB,
        .pin        = GPIO_PIN_2,
        .type       = DRV_GPIO_TYPE_INPUT,
        .activeLow  = false,
        .initState  = DRV_GPIO_LOW,
    },
    [DRV_GPIO_CHANNEL_SPI_LCD_CS] =
    {
        .gpio       = GPIOB,
        .pin        = GPIO_PIN_5,
        .type       = DRV_GPIO_TYPE_OUTPUT,
        .activeLow  = true,
        .initState  = DRV_GPIO_HIGH,
    },
    [DRV_GPIO_CHANNEL_LED_DRIVER_OE] =
    {
        .gpio       = GPIOB,
        .pin        = GPIO_PIN_6,
        .type       = DRV_GPIO_TYPE_OUTPUT,
        .activeLow  = false,
        .initState  = DRV_GPIO_HIGH,
    },
    [DRV_GPIO_CHANNEL_LCD_RST] =
    {
        .gpio       = GPIOB,
        .pin        = GPIO_PIN_9,
        .type       = DRV_GPIO_TYPE_OUTPUT,
        .activeLow  = false,
        .initState  = DRV_GPIO_HIGH,
    },
    [DRV_GPIO_CHANNEL_BUTTON_B] =
    {
        .gpio       = GPIOB,
        .pin        = GPIO_PIN_11,
        .type       = DRV_GPIO_TYPE_INPUT,
        .activeLow  = false,
        .initState  = DRV_GPIO_LOW,
    },
    [DRV_GPIO_CHANNEL_LED_BLINKY] =
    {
        .gpio       = GPIOB,
        .pin        = GPIO_PIN_14,
        .type       = DRV_GPIO_TYPE_OUTPUT,
        .activeLow  = false,
        .initState  = DRV_GPIO_LOW,
    },
};

// clang-format on

drv_GPIO_config_S drv_GPIO_config = {
    .channelConfig = drv_GPIO_channelConfig,
    .channelCount  = DRV_GPIO_CHANNEL_COUNT,
};

/* P U B L I C   F U N C T I O N S */

void drv_GPIO_setOutput(drv_GPIO_channel_E channel, drv_GPIO_state_E state)
{
    const drv_GPIO_channelConfig_S *channelConfig = &drv_GPIO_config.channelConfig[channel];
    HAL_GPIO_WritePin(channelConfig->gpio, channelConfig->pin, (GPIO_PinState)state);
}

void drv_GPIO_toggleOutput(drv_GPIO_channel_E channel)
{
    const drv_GPIO_channelConfig_S *channelConfig = &drv_GPIO_config.channelConfig[channel];
    HAL_GPIO_TogglePin(channelConfig->gpio, channelConfig->pin);
}

drv_GPIO_state_E drv_GPIO_getInput(drv_GPIO_channel_E channel)
{
    drv_GPIO_hardwareSpecific_channelData_S *channelData = &drv_GPIO_hardwareSpecific_data.channelData[channel];
    drv_GPIO_channelConfig_S *channelConfig              = &drv_GPIO_config.channelConfig[channel];

    if (channelConfig->type == DRV_GPIO_TYPE_INPUT)
    {
        if (HAL_GPIO_ReadPin(channelConfig->gpio, channelConfig->pin) == GPIO_PIN_SET)
        {
            channelData->state = DRV_GPIO_HIGH;
        }
        else
        {
            channelData->state = DRV_GPIO_LOW;
        }
    }

    return channelData->state;
}

void drv_GPIO_update(void)
{
    for (drv_GPIO_channel_E channel = (drv_GPIO_channel_E)0U; channel < DRV_GPIO_CHANNEL_COUNT; channel++)
    {
        drv_GPIO_hardwareSpecific_channelData_S *channelData = &drv_GPIO_hardwareSpecific_data.channelData[channel];
        drv_GPIO_channelConfig_S *channelConfig              = &drv_GPIO_config.channelConfig[channel];

        if (channelConfig->type == DRV_GPIO_TYPE_INPUT)
        {
            if (HAL_GPIO_ReadPin(channelConfig->gpio, channelConfig->pin) == GPIO_PIN_SET)
            {
                channelData->state = DRV_GPIO_HIGH;
            }
            else
            {
                channelData->state = DRV_GPIO_LOW;
            }
        }
    }
}

#endif  // FEATURE_GPIO

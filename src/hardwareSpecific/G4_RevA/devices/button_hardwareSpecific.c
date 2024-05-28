/* I N C L U D E S */

#include "button.h"

#include "PCA9555.h"
#include "drv_GPIO.h"

#if FEATURE_BUTTON

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* T Y P E D E F S */

/* D A T A   D E F I N I T I O N S */

// clang-format off
static Button_channelConfig_S button_channelConfig[BUTTON_CHANNEL_COUNT] =
{
    [BUTTON_CHANNEL_A] = {         .inputType = BUTTON_INPUT_TYPE_GPIO, .gpioChannel = DRV_GPIO_CHANNEL_BUTTON_A},
    [BUTTON_CHANNEL_B] = {         .inputType = BUTTON_INPUT_TYPE_GPIO, .gpioChannel = DRV_GPIO_CHANNEL_BUTTON_B},

    [BUTTON_CHANNEL_1]  = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_7},
    [BUTTON_CHANNEL_2]  = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_6},
    [BUTTON_CHANNEL_3]  = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_5},
    [BUTTON_CHANNEL_4]  = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_0},
    [BUTTON_CHANNEL_5]  = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_15},
    [BUTTON_CHANNEL_6]  = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_10},
    [BUTTON_CHANNEL_7]  = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_9},
    [BUTTON_CHANNEL_8]  = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_8},
    [BUTTON_CHANNEL_9]  = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_4},
    [BUTTON_CHANNEL_10] = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_3},
    [BUTTON_CHANNEL_11] = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_2},
    [BUTTON_CHANNEL_12] = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_1},
    [BUTTON_CHANNEL_13] = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_14},
    [BUTTON_CHANNEL_14] = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_13},
    [BUTTON_CHANNEL_15] = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_12},
    [BUTTON_CHANNEL_16] = {.inputType = BUTTON_INPUT_TYPE_PORT_EXPANDER,     .pca9555Channel = PCA9555_CHANNEL_11},
};
// clang-format on

Button_config_S buttonConfig = {
    .channelConfig = button_channelConfig,
    .channelCount  = (uint32_t)(BUTTON_CHANNEL_COUNT),
};

/* P R I V A T E   F U N C T I O N S */

/* P U B L I C   F U N C T I O N S */

#endif  // FEATURE_BUTTON

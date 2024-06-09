#ifndef PCA9685_H_
#define PCA9685_H_

/* I N C L U D E S */

#include "features.h"

#include "drv_GPIO.h"
#include "drv_I2C.h"

#include <stdint.h>

#include "PCA9685_hardwareSpecific.h"

#if FEATURE_PCA9685

/* D E F I N E S */

#define PCA9685_ADDRESS(A5, A4, A3, A2, A1, A0) ((0x40U | (A5 << 5U) | (A4 << 4U) | (A3 << 3U) | (A2 << 2U) | (A1 << 1U) | A0))

#define PCA9685_PWM_MAX_VALUE                   4095U
#define PCA9685_PWM_MIN_VALUE                   0U

#define PCA9685_BRIGHTNESS_MAX_VALUE            1.0f
#define PCA9685_BRIGHTNESS_MIN_VALUE            0.0f

#define PCA9685_PWM_PERCENT_TO_COUNT(PERCENT)   ((uint16_t)((PERCENT * PCA9685_PWM_MAX_VALUE) - 1U))

#define PCA9685_BYTES_PER_LED                   4U

// REGISTERS

#define PCA9685_REGISTER_MODE1                  0x00U
#define PCA9685_REGISTER_MODE2                  0x01U
#define PCA9685_REGISTER_SUBADR1                0x02U
#define PCA9685_REGISTER_SUBADR2                0x03U
#define PCA9685_REGISTER_SUBADR3                0x04U
#define PCA9685_REGISTER_ALLCALLADR             0x05U

#define PCA9685_REGISTER_LED0_ON_L              0x06U
#define PCA9685_REGISTER_LED0_ON_H              0x07U
#define PCA9685_REGISTER_LED0_OFF_L             0x08U
#define PCA9685_REGISTER_LED0_OFF_H             0x09U
#define PCA9685_REGISTER_LED1_ON_L              0x0AU
#define PCA9685_REGISTER_LED1_ON_H              0x0BU
#define PCA9685_REGISTER_LED1_OFF_L             0x0CU
#define PCA9685_REGISTER_LED1_OFF_H             0x0DU
#define PCA9685_REGISTER_LED2_ON_L              0x0EU
#define PCA9685_REGISTER_LED2_ON_H              0x0FU
#define PCA9685_REGISTER_LED2_OFF_L             0x10U
#define PCA9685_REGISTER_LED2_OFF_H             0x11U
#define PCA9685_REGISTER_LED3_ON_L              0x12U
#define PCA9685_REGISTER_LED3_ON_H              0x13U
#define PCA9685_REGISTER_LED3_OFF_L             0x14U
#define PCA9685_REGISTER_LED3_OFF_H             0x15U
#define PCA9685_REGISTER_LED4_ON_L              0x16U
#define PCA9685_REGISTER_LED4_ON_H              0x17U
#define PCA9685_REGISTER_LED4_OFF_L             0x18U
#define PCA9685_REGISTER_LED4_OFF_H             0x19U
#define PCA9685_REGISTER_LED5_ON_L              0x1aU
#define PCA9685_REGISTER_LED5_ON_H              0x1bU
#define PCA9685_REGISTER_LED5_OFF_L             0x1cU
#define PCA9685_REGISTER_LED5_OFF_H             0x1dU
#define PCA9685_REGISTER_LED6_ON_L              0x1eU
#define PCA9685_REGISTER_LED6_ON_H              0x1fU
#define PCA9685_REGISTER_LED6_OFF_L             0x20U
#define PCA9685_REGISTER_LED6_OFF_H             0x21U
#define PCA9685_REGISTER_LED7_ON_L              0x22U
#define PCA9685_REGISTER_LED7_ON_H              0x23U
#define PCA9685_REGISTER_LED7_OFF_L             0x24U
#define PCA9685_REGISTER_LED7_OFF_H             0x25U
#define PCA9685_REGISTER_LED8_ON_L              0x26U
#define PCA9685_REGISTER_LED8_ON_H              0x27U
#define PCA9685_REGISTER_LED8_OFF_L             0x28U
#define PCA9685_REGISTER_LED8_OFF_H             0x29U
#define PCA9685_REGISTER_LED9_ON_L              0x2aU
#define PCA9685_REGISTER_LED9_ON_H              0x2bU
#define PCA9685_REGISTER_LED9_OFF_L             0x2cU
#define PCA9685_REGISTER_LED9_OFF_H             0x2dU
#define PCA9685_REGISTER_LED10_ON_L             0x2eU
#define PCA9685_REGISTER_LED10_ON_H             0x2fU
#define PCA9685_REGISTER_LED10_OFF_L            0x30U
#define PCA9685_REGISTER_LED10_OFF_H            0x31U
#define PCA9685_REGISTER_LED11_ON_L             0x32U
#define PCA9685_REGISTER_LED11_ON_H             0x33U
#define PCA9685_REGISTER_LED11_OFF_L            0x34U
#define PCA9685_REGISTER_LED11_OFF_H            0x35U
#define PCA9685_REGISTER_LED12_ON_L             0x36U
#define PCA9685_REGISTER_LED12_ON_H             0x37U
#define PCA9685_REGISTER_LED12_OFF_L            0x38U
#define PCA9685_REGISTER_LED12_OFF_H            0x39U
#define PCA9685_REGISTER_LED13_ON_L             0x3aU
#define PCA9685_REGISTER_LED13_ON_H             0x3bU
#define PCA9685_REGISTER_LED13_OFF_L            0x3cU
#define PCA9685_REGISTER_LED13_OFF_H            0x3dU
#define PCA9685_REGISTER_LED14_ON_L             0x3eU
#define PCA9685_REGISTER_LED14_ON_H             0x3fU
#define PCA9685_REGISTER_LED14_OFF_L            0x40U
#define PCA9685_REGISTER_LED14_OFF_H            0x41U
#define PCA9685_REGISTER_LED15_ON_L             0x42U
#define PCA9685_REGISTER_LED15_ON_H             0x43U
#define PCA9685_REGISTER_LED15_OFF_L            0x44U
#define PCA9685_REGISTER_LED15_OFF_H            0x45U

#define PCA9685_REGISTER_ALL_LED_ON_L           0xFAU
#define PCA9685_REGISTER_ALL_LED_ON_H           0xFBU
#define PCA9685_REGISTER_ALL_LED_OFF_L          0xFCU
#define PCA9685_REGISTER_ALL_LED_OFF_H          0xFDU
#define PCA9685_REGISTER_PRE_SCALE              0xFEU

// clang-format off
#define SET_PCA9685_REGISTER_MODE_1_RESTART(reg, value) \
    {                                                   \
        BIT_CLEAR(reg, 7U);                             \
        BIT_SET_VALUE(reg, 7U, value);                  \
    }

#define SET_PCA9685_REGISTER_MODE_1_EXTCLK(reg, value)  \
    {                                                   \
        BIT_CLEAR(reg, 6U);                             \
        BIT_SET_VALUE(reg, 6U value);                   \
    }

#define SET_PCA9685_REGISTER_MODE_1_AUTO_INC(reg, value)\
    {                                                   \
        BIT_CLEAR(reg, 5U);                             \
        BIT_SET_VALUE(reg, 5U, value);                  \
    }

#define SET_PCA9685_REGISTER_MODE_1_SLEEP(reg, value)   \
    {                                                   \
        BIT_CLEAR(reg, 4U);                             \
        BIT_SET_VALUE(reg, 4U, value);                  \
    }

#define SET_PCA9685_REGISTER_MODE_1_SUB1(reg, value)    \
    {                                                   \
        BIT_CLEAR(reg, 3U);                             \
        BIT_SET_VALUE(reg, 3U value);                   \
    }

#define SET_PCA9685_REGISTER_MODE_1_SUB2(reg, value)    \
    {                                                   \
        BIT_CLEAR(reg, 2U);                             \
        BIT_SET_VALUE(reg, 2U, value);                  \
    }

#define SET_PCA9685_REGISTER_MODE_1_SUB3(reg, value)    \
    {                                                   \
        BIT_CLEAR(reg, 1U);                             \
        BIT_SET_VALUE(reg, 1U, value);                  \
    }

#define SET_PCA9685_REGISTER_MODE_1_ALLCALL(reg, value) \
    {                                                   \
        BIT_CLEAR(reg, 0U);                             \
        BIT_SET_VALUE(reg, 0U, value);                  \
    }

#define SET_PCA9685_REGISTER_MODE_2_INVRT(reg, value)   \
    {                                                   \
        BIT_CLEAR(reg, 4U);                             \
        BIT_SET_VALUE(reg, 4U, value);                  \
    }

#define SET_PCA9685_REGISTER_MODE_2_OCH(reg, value)     \
    {                                                   \
        BIT_CLEAR(reg, 3U);                             \
        BIT_SET_VALUE(reg, 3U, value);                  \
    }

#define SET_PCA9685_REGISTER_MODE_2_OUTDRV(reg, value)  \
    {                                                   \
        BIT_CLEAR(reg, 2U);                             \
        BIT_SET_VALUE(reg, 2U, value);                  \
    }

#define SET_PCA9685_REGISTER_MODE_2_OUTNE(reg, value)   \
    {                                                   \
        reg &= ~0x03U;                                  \
        reg |= ((value) & (0x03U));                     \
    }

#define PCA9685_SET_LED_ON_COUNT(data, indexOffset, value)  \
    {                                                       \
        data[indexOffset] = ((value >> 8U) & 0xF);          \
        data[indexOffset + 1U] = (value & 0xFFU);           \
    }

#define PCA9685_SET_LED_OFF_COUNT(data, indexOffset, value) \
    {                                                       \
        data[indexOffset + 2U] = ((value >> 8U) & 0xF);     \
        data[indexOffset + 3U] = (value & 0xFFU);           \
    }

#define PCA9685_SET_LED_FULL_ON(data, indexOffset)          \
    {                                                       \
        PCA9685_SET_LED_ON_COUNT(data, indexOffset, 0U);    \
        PCA9685_SET_LED_OFF_COUNT(data, indexOffset, 0U);   \
        BIT_CLEAR(data[indexOffset + 1U], 4U);              \
        BIT_SET(data[indexOffset + 1U], 4U);                \
    }

#define PCA9685_SET_LED_FULL_OFF(data, indexOffset) \
    {                                               \
        PCA9685_SET_LED_ON_COUNT(data, indexOffset, 0U);    \
        PCA9685_SET_LED_OFF_COUNT(data, indexOffset, 0U);   \
        BIT_CLEAR(data[indexOffset+ 3U], 4U);       \
        BIT_SET(data[indexOffset + 3U], 4U);        \
    }

// clang-format on

/* T Y P E D E F S */

typedef struct
{
    drv_I2C_device_E i2cDevice;
    drv_GPIO_channel_E gpioOE;
    bool updateOnAck;
    bool invertOutput;
} PCA9685_config_S;

/* P U B L I C   F U N C T I O N S */

void PCA9685_init(PCA9685_config_S *config);
void PCA9685_configure(void);
void PCA9685_updateOutputs(void);

void PCA9685_setState(const PCA9685_channel_E channel, bool state);
bool PCA9685_getState(const PCA9685_channel_E channel);

void PCA9685_setBrightness(const PCA9685_channel_E channel, float brightness);
float PCA9685_getBrightness(const PCA9685_channel_E channel);
void PCA9685_toggle(const PCA9685_channel_E channel);

#endif  // FEATURE_PCA9685
#endif  /* PCA9685_H_ */

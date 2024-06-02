
/* I N C L U D E S */

#include "PCA9685.h"
#include "Utils.h"
#include "drv_I2C.h"

#if FEATURE_PCA9685

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* T Y P E D E F S */

typedef struct
{
    float desiredBrightness;
    float brightness;
} PCA9685_ledData_S;

typedef struct
{
    PCA9685_config_S *config;
    PCA9685_ledData_S ledData[PCA9685_CHANNEL_COUNT];
} PCA9685_data_S;

/* D A T A   D E F I N I T I O N S */

static PCA9685_data_S PCA9685_data;

/* P R I V A T E   F U N C T I O N S */

/* P U B L I C   F U N C T I O N S */

void PCA9685_init(PCA9685_config_S *config)
{
    if (config != NULL)
    {
        memset(&PCA9685_data, 0U, sizeof(PCA9685_data));

        PCA9685_data.config = config;
        PCA9685_configure();
    }
}

void PCA9685_configure(void)
{
    const PCA9685_config_S *config = PCA9685_data.config;

    // MODE_1
    uint8_t mode1Config = 0U;
    SET_PCA9685_REGISTER_MODE_1_AUTO_INC(mode1Config, 1U);

    uint8_t txDataMode1[] = {
        PCA9685_REGISTER_MODE1,  // register
        mode1Config,             // MODE_1 config
    };

    (void)drv_I2C_writeBytes(config->i2cDevice, txDataMode1, sizeof(txDataMode1));

    // MODE_2
    uint8_t mode2Config = 0U;
    SET_PCA9685_REGISTER_MODE_2_INVRT(mode2Config, config->invertOutput);
    SET_PCA9685_REGISTER_MODE_2_OCH(mode2Config, config->updateOnAck);
    SET_PCA9685_REGISTER_MODE_2_OUTNE(mode2Config, 0x00U);

    uint8_t txDataMode2[] = {
        PCA9685_REGISTER_MODE2,  // register
        mode2Config,             // MODE_2 config
    };

    (void)drv_I2C_writeBytes(config->i2cDevice, txDataMode2, sizeof(txDataMode2));

    PCA9685_updateOutputs();
}

void PCA9685_updateOutputs(void)
{
    const PCA9685_config_S *config = PCA9685_data.config;

    // register auto increment is used, so can update all LEDs in one transaction
    // uint8_t txData[(PCA9685_BYTES_PER_LED * PCA9685_CHANNEL_COUNT) + 1U] = {0U};
    uint8_t txData[71U] = {0U};

    // txData[0U]                                                           = PCA9685_REGISTER_LED0_ON_L;
    txData[0U] = PCA9685_REGISTER_MODE1;

    uint8_t mode1Config = 0U;
    SET_PCA9685_REGISTER_MODE_1_AUTO_INC(mode1Config, 1U);
    txData[1U] = mode1Config;

    // MODE_2
    uint8_t mode2Config = 0U;
    SET_PCA9685_REGISTER_MODE_2_INVRT(mode2Config, config->invertOutput);
    SET_PCA9685_REGISTER_MODE_2_OUTDRV(mode2Config, 0U);
    SET_PCA9685_REGISTER_MODE_2_OCH(mode2Config, config->updateOnAck);
    SET_PCA9685_REGISTER_MODE_2_OUTNE(mode2Config, 0x00U);
    txData[2U] = mode2Config;

    txData[3U] = 0xE2U;  // SUBADR1
    txData[4U] = 0xE4U;  // SUBADR2
    txData[5U] = 0xE8U;  // SUBADR3

    // drv_GPIO_setOutput(config->gpioOE, DRV_GPIO_HIGH);

    for (PCA9685_channel_E channel = (PCA9685_channel_E)0U; channel < PCA9685_CHANNEL_COUNT; channel++)
    {
        PCA9685_ledData_S *ledData = &PCA9685_data.ledData[channel];
        const uint8_t startByte    = (channel * PCA9685_BYTES_PER_LED) + 6U;

        // const uint16_t onCounts = PCA9685_PWM_PERCENT_TO_COUNT(ledData->desiredBrightness);
        // const uint16_t offCounts = (PCA9685_PWM_MAX_VALUE - onCounts);
        if (ledData->desiredBrightness > PCA9685_BRIGHTNESS_MIN_VALUE)
        {
            PCA9685_SET_LED_FULL_ON(txData[startByte + 1U], 1U);
            // PCA9685_SET_LED_FULL_OFF(txData[startByte +3U], 0U);
        }
        else
        {
            // PCA9685_SET_LED_FULL_ON(txData[startByte +1U], 0U);
            PCA9685_SET_LED_FULL_OFF(txData[startByte + 3U], 1U);
        }

        // txData[startByte + 2U]       = (uint8_t)(offCounts & 0xFFU);          // ON_L
        // txData[startByte + 3U]  = (uint8_t)((offCounts >> 8U) & 0x0FU);  // ON_H
        // PCA9685_SET_LED_FULL_OFF(txData[startByte +3U], 0U);
        // // txData[startByte + 0U]  = 0U; // & 0xFFU;// 0U;                                   // OFF_L - no phase
        // shift
        // // txData[startByte + 1U]  = 0U; //4095U >> 8U;//0U;
        // txData[startByte]       = (uint8_t)(onCounts & 0xFFU);          // ON_L
        // txData[startByte + 1U]  = (uint8_t)((onCounts >> 8U) & 0x0FU);  // ON_H

        // PCA9685_SET_LED_FULL_ON(txData[startByte], 0U)                             // OFF_H - no phase shift

        // txData[startByte]       = (uint8_t)(onCounts & 0xFFU);          // ON_L
        // txData[startByte + 1U]  = (uint8_t)((onCounts >> 8U) & 0x0FU);  // ON_H
        // PCA9685_SET_LED_FULL_ON(txData[startByte +1U], 0U);
        // txData[startByte + 2U]  = 0U; // & 0xFFU;// 0U;                                   // OFF_L - no phase shift
        // txData[startByte + 3U]  = 0U; //4095U >> 8U;//0U;
        // PCA9685_SET_LED_FULL_ON(txData[startByte +3U], 0U)                             // OFF_H - no phase shift
        ledData->brightness = ledData->desiredBrightness;
    }

    (void)drv_I2C_writeBytes(config->i2cDevice, txData, sizeof(txData));
    drv_GPIO_setOutput(config->gpioOE, DRV_GPIO_LOW);
}

void PCA9685_setState(const PCA9685_channel_E channel, bool state)
{
    PCA9685_ledData_S *ledData = &PCA9685_data.ledData[channel];
    ledData->desiredBrightness = state ? PCA9685_BRIGHTNESS_MAX_VALUE : PCA9685_BRIGHTNESS_MIN_VALUE;
}

bool PCA9685_getState(const PCA9685_channel_E channel)
{
    const PCA9685_ledData_S *ledData = &PCA9685_data.ledData[channel];
    return (ledData->brightness > PCA9685_BRIGHTNESS_MIN_VALUE);
}

void PCA9685_setBrightness(const PCA9685_channel_E channel, float brightness)
{
    PCA9685_ledData_S *ledData = &PCA9685_data.ledData[channel];
    ledData->desiredBrightness = brightness;
}

float PCA9685_getBrightness(const PCA9685_channel_E channel)
{
    const PCA9685_ledData_S *ledData = &PCA9685_data.ledData[channel];
    return ledData->desiredBrightness;
}

void PCA9685_toggle(const PCA9685_channel_E channel)
{
    const bool ledOn = PCA9685_getState(channel);
    PCA9685_setState(channel, (ledOn == false));
}

#endif  // FEATURE_PCA9685

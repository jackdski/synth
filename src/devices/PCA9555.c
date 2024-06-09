
/* I N C L U D E S */

#include "PCA9555.h"
#include "Utils.h"
#include "drv_I2C.h"
#include "main.h"

#if FEATURE_PCA9555

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* T Y P E D E F S */

typedef struct
{
    bool desiredState;
    bool actualState;
} PCA9555_pinData_S;

typedef struct
{
    PCA9555_config_S *config;
    PCA9555_pinData_S pinData[PCA9555_CHANNEL_COUNT];
} PCA9555_data_S;

/* D A T A   D E F I N I T I O N S */

static PCA9555_data_S PCA9555_data;

/* P R I V A T E   F U N C T I O N S */

/* P U B L I C   F U N C T I O N S */

void PCA9555_init(PCA9555_config_S *config)
{
    if (config != NULL)
    {
        PCA9555_data.config = config;
        PCA9555_configure();
    }
}

void PCA9555_configure(void)
{
    const PCA9555_config_S *config = PCA9555_data.config;

    // PORT0
    uint8_t port0InputConfig  = 0U;
    uint8_t port0InvertConfig = 0U;
    for (PCA9555_channel_E channel = PCA9555_PORT0_START_PIN; channel < PCA9555_PORT0_END_PIN; channel++)
    {
        const PCA9555_pinConfig_S *pinConfig = &PCA9555_data.config->pinConfig[channel];
        const bool isInput                   = (pinConfig->pinType == PCA9555_PIN_TYPE_INPUT);

        if (isInput)
        {
            BIT_SET(port0InputConfig, channel);
        }

        if (pinConfig->invert)
        {
            BIT_SET(port0InvertConfig, (channel));
        }
    }

    uint8_t txDataPort0InputConfig[] = {
        PCA9555_CONFIG_PORT_0,  // register
        port0InputConfig,       // port 0 config
    };

    (void)drv_I2C_writeBytes(config->i2cDevice, txDataPort0InputConfig, sizeof(txDataPort0InputConfig));

    uint8_t txDataPort0InvertConfig[] = {
        PCA9555_POL_INV_PORT0_CMD,
        port0InvertConfig,
    };

    (void)drv_I2C_writeBytes(config->i2cDevice, txDataPort0InvertConfig, sizeof(txDataPort0InvertConfig));

    // PORT1
    uint8_t port1InputConfig  = 0U;
    uint8_t port1InvertConfig = 0U;
    for (PCA9555_channel_E channel = PCA9555_PORT1_START_PIN; channel < PCA9555_PORT1_END_PIN; channel++)
    {
        const PCA9555_pinConfig_S *pinConfig = &PCA9555_data.config->pinConfig[channel - PCA9555_PORT1_START_PIN];
        const bool isInput                   = (pinConfig->pinType == PCA9555_PIN_TYPE_INPUT);

        if (isInput)
        {
            BIT_SET(port1InputConfig, (channel - PCA9555_PORT1_START_PIN));
        }

        if (pinConfig->invert)
        {
            BIT_SET(port1InvertConfig, (channel - PCA9555_PORT1_START_PIN));
        }
    }

    uint8_t txDataPort1[] = {
        PCA9555_CONFIG_PORT_1,
        port1InputConfig,
    };

    (void)drv_I2C_writeBytes(config->i2cDevice, txDataPort1, sizeof(txDataPort1));

    uint8_t txDataPort1InvertConfig[] = {
        PCA9555_POL_INV_PORT1_CMD,
        port1InvertConfig,
    };

    (void)drv_I2C_writeBytes(config->i2cDevice, txDataPort1InvertConfig, sizeof(txDataPort1InvertConfig));

    PCA9555_updateInputs();
}

void PCA9555_updateOutputs(void)
{
    uint8_t port0Outputs = 0U;
    for (PCA9555_channel_E channel = PCA9555_PORT0_START_PIN; channel < PCA9555_PORT0_END_PIN; channel++)
    {
        const PCA9555_pinConfig_S *pinConfig = &PCA9555_data.config->pinConfig[channel];
        PCA9555_pinData_S *pinData           = &PCA9555_data.pinData[channel];

        const bool isOutput = (pinConfig->pinType == PCA9555_PIN_TYPE_OUTPUT);
        if (pinData->desiredState && isOutput)
        {
            BIT_SET(port0Outputs, channel);
        }
    }

    uint8_t txDataPort0[] = {PCA9555_OUTPUT_PORT0_CMD, port0Outputs};
    drv_I2C_writeBytes(PCA9555_data.config->i2cDevice, txDataPort0, sizeof(txDataPort0));

    uint8_t port1Outputs = 0U;
    for (PCA9555_channel_E channel = PCA9555_PORT1_START_PIN; channel < PCA9555_PORT1_END_PIN; channel++)
    {
        const PCA9555_pinConfig_S *pinConfig = &PCA9555_data.config->pinConfig[channel];
        PCA9555_pinData_S *pinData           = &PCA9555_data.pinData[channel];

        const bool isOutput = (pinConfig->pinType == PCA9555_PIN_TYPE_OUTPUT);
        if (pinData->desiredState && isOutput)
        {
            BIT_SET(port1Outputs, (channel - PCA9555_PORT1_START_PIN));
        }
    }

    uint8_t txDataPort1[] = {PCA9555_OUTPUT_PORT1_CMD, port1Outputs};
    drv_I2C_writeBytes(PCA9555_data.config->i2cDevice, txDataPort1, sizeof(txDataPort1));
}

void PCA9555_updateInputs(void)
{
    uint8_t rxBufferPort0 = 0U;
    uint8_t txBufferPort0 = PCA9555_INPUT_PORT0_CMD;

    if (drv_I2C_readBytes(PCA9555_data.config->i2cDevice, &txBufferPort0, sizeof(txBufferPort0), &rxBufferPort0, sizeof(rxBufferPort0)))
    {

        for (PCA9555_channel_E channel = PCA9555_PORT0_START_PIN; channel < PCA9555_PORT0_END_PIN; channel++)
        {
            PCA9555_pinData_S *pinData = &PCA9555_data.pinData[channel];
            // pinData->actualState       = (rxBufferPort0 && BIT_U8(channel));
            pinData->actualState = GET_BIT(rxBufferPort0, channel);
        }
    }

    uint8_t rxBufferPort1 = 0U;
    uint8_t txBufferPort1 = PCA9555_INPUT_PORT1_CMD;

    if (drv_I2C_readBytes(PCA9555_data.config->i2cDevice, &txBufferPort1, sizeof(txBufferPort1), &rxBufferPort1, sizeof(rxBufferPort1)))
    {
        for (PCA9555_channel_E channel = PCA9555_PORT1_START_PIN; channel < PCA9555_PORT1_END_PIN; channel++)
        {
            PCA9555_pinData_S *pinData = &PCA9555_data.pinData[channel];
            // pinData->actualState       = (rxBufferPort1 && BIT_U8((channel - PCA9555_PORT1_START_PIN)));
            pinData->actualState = GET_BIT(rxBufferPort1, (channel - PCA9555_PORT1_START_PIN));
        }
    }
}

void PCA9555_setState(const PCA9555_channel_E channel, bool state)
{
    PCA9555_pinData_S *pinData = &PCA9555_data.pinData[channel];
    pinData->desiredState      = state;
}

bool PCA9555_getState(const PCA9555_channel_E channel)
{
    const PCA9555_pinData_S *pinData = &PCA9555_data.pinData[channel];
    return pinData->actualState;
}

#endif  // FEATURE_PCA9555

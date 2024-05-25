
/* I N C L U D E S */

#include "PCA9555.h"
#include "drv_I2C.h"
#include "main.h"
#include "Utils.h"

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
    PCA9555_config_S  * config;
    PCA9555_pinData_S   pinData[PCA9555_CHANNEL_COUNT];
} PCA9555_data_S;

/* D A T A   D E F I N I T I O N S */

static PCA9555_data_S data;

/* P R I V A T E   F U N C T I O N S */

/* P U B L I C   F U N C T I O N S */

void PCA9555_configure(void)
{
    const PCA9555_config_S * config = data.config;

    uint8_t txData[] =
    {
        PCA9555_CONFIG_PORT0_CMD, // register
        PCA9555_CONFIG_PORT_0,  // command type
        PCA9555_CONFIG_PORT_1,  // port 0 data
        PCA9555_CONFIG_PORT_1,  // port 1 data
    };

    (void)drv_I2C_writeBytes(config->i2cDevice, txData, sizeof(txData));
}

void PCA9555_updateOutputs(void)
{
    uint8_t port0Outputs = 0U;
    for (PCA9555_channel_E channel = PCA9555_PORT0_START_PIN; channel < PCA9555_PORT0_END_PIN; channel++)
    {
        const PCA9555_pinConfig_S * pinConfig = &data.config->pinConfig[channel];
        PCA9555_pinData_S * pinData = &data.pinData[channel];

        const bool isOutput = (pinConfig->pinType == PCA9555_PIN_TYPE_OUTPUT);
        port0Outputs |= ((pinData->desiredState && isOutput) << (uint32_t)channel);
    }

    uint8_t txDataPort0[] = { PCA9555_OUTPUT_PORT0_CMD, port0Outputs };
    drv_I2C_writeBytes(data.config->i2cDevice, txDataPort0, sizeof(txDataPort0));

    uint8_t port1Outputs = 0U;
    for (PCA9555_channel_E channel = PCA9555_PORT1_START_PIN; channel < PCA9555_PORT1_END_PIN; channel++)
    {
        const PCA9555_pinConfig_S * pinConfig = &data.config->pinConfig[channel];
        PCA9555_pinData_S * pinData = &data.pinData[channel];

        const bool isOutput = (pinConfig->pinType == PCA9555_PIN_TYPE_OUTPUT);
        port1Outputs |= ((pinData->desiredState && isOutput) << (uint32_t)channel);
    }

    uint8_t txDataPort1[] = { PCA9555_OUTPUT_PORT1_CMD, port1Outputs };
    drv_I2C_writeBytes(data.config->i2cDevice, txDataPort1, sizeof(txDataPort1));
}

void PCA9555_updateInputs(void)
{
    uint8_t rxBuffer[2U] = {0U};
    uint8_t txBuffer = PCA9555_INPUT_PORT0_CMD;

    if (drv_I2C_readBytes(data.config->i2cDevice, &txBuffer, sizeof(txBuffer), rxBuffer, sizeof(rxBuffer)))
    {

        for (PCA9555_channel_E channel = PCA9555_PORT0_START_PIN; channel < PCA9555_PORT0_END_PIN; channel++)
        {
            PCA9555_pinData_S * pinData = &data.pinData[channel];
            pinData->actualState = (rxBuffer[0U] && BIT_U8(channel));
        }

        for (PCA9555_channel_E channel = PCA9555_PORT1_START_PIN; channel < PCA9555_PORT1_END_PIN; channel++)
        {
            PCA9555_pinData_S * pinData = &data.pinData[channel];
            pinData->actualState = (rxBuffer[1U] && BIT_U8((channel - PCA9555_PORT1_START_PIN)));
        }
    }
}

#endif // FEATURE_PCA9555

#include "drv_I2C.h"

/* I N C L U D E S */

#include "features.h"

#include "i2c.h"
#include "main.h"
#include "stm32g4xx_hal.h"

/* D E F I N E S */

#define SGTL5000_I2C_ADDRESS            (0x0A << 1U)  // or 0x2A
#define PCA9685PW_I2C_ADDRESS(aValue)   ((0x40 | aValue) << 1U)  // or 0x80 | R/W
#define PCA9555DB_I2C_ADDRESS(aValue)   ((0x20 | aValue) << 1U)  // or 0x40 | R/W


/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

static bool drv_I2C_private_writeBytesBus0(drv_I2C_TransactionConfig_S * xfer);
static bool drv_I2C_private_readBytesBus0(drv_I2C_TransactionConfig_S * xfer);
static bool drv_I2C_private_isBusyBus0(void);

/* D A T A   D E F I N I T I O N S */

extern I2C_HandleTypeDef hi2c1;

static drv_I2C_busConfig_S i2cBusConfig[DRV_I2C_BUS_COUNT] =
{
    [DRV_I2C_BUS_1] =
    {
        .writeBytes = drv_I2C_private_writeBytesBus0,
        .readBytes  = drv_I2C_private_readBytesBus0,
        .isBusy     = drv_I2C_private_isBusyBus0,
    },
};

static drv_I2C_deviceConfig_S i2cDeviceConfig[DRV_I2C_DEVICE_COUNT] =
{
    [DRV_I2C_DEVICE_SGTL5000] =
    {
        .deviceAddress = SGTL5000_I2C_ADDRESS,
        .bus           = DRV_I2C_BUS_1,
    },
    [DRV_I2C_DEVICE_PORT_EXPANDER] =
    {
        .deviceAddress = PCA9555DB_I2C_ADDRESS(0U),
        .bus           = DRV_I2C_BUS_1,
    },
    [DRV_I2C_DEVICE_LED_DRIVER] =
    {
        .deviceAddress = PCA9685PW_I2C_ADDRESS(0U),
        .bus           = DRV_I2C_BUS_1,
    },
};

const drv_I2C_config_S drv_I2C_config =
{
    .busConfig = i2cBusConfig,
    .deviceConfig = i2cDeviceConfig
};


/* P R I V A T E   F U N C T I O N S */

static bool drv_I2C_private_writeBytesBus0(drv_I2C_TransactionConfig_S * xfer)
{
    bool ret = true;

    const uint16_t deviceAddress = i2cDeviceConfig[xfer->i2cDevice].deviceAddress;

    if (HAL_I2C_Master_Transmit(&hi2c1, deviceAddress, xfer->txBuffer, xfer->txLength, 5U) != HAL_OK)
    {
        ret = false;
    }
    return ret;
}

static bool drv_I2C_private_readBytesBus0(drv_I2C_TransactionConfig_S * xfer)
{
    bool ret = true;

    const uint16_t deviceAddress = i2cDeviceConfig[xfer->i2cDevice].deviceAddress;

    if (HAL_I2C_Master_Transmit(&hi2c1, deviceAddress, xfer->txBuffer, sizeof(xfer->txLength), 5U) == HAL_OK)
    {
        ret = (HAL_I2C_Master_Receive(&hi2c1, deviceAddress, xfer->rxBuffer, sizeof(xfer->rxLength), 5U) == HAL_OK);
    }
    else
    {
        ret = false;
    }
    return ret;
}

static bool drv_I2C_private_isBusyBus0(void)
{
    return (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
}

/* P U B L I C   F U N C T I O N S */

void drv_I2C_hardwareSpecific_init(void)
{
    MX_I2C1_Init();
}

#if I2C_ASYNC

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == &hi2c1)
    {
        drv_I2C_startNextTransactionISR(DRV_I2C_BUS_1);
    }
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == &hi2c1)
    {
        drv_I2C_startNextTransactionISR(DRV_I2C_BUS_1);
    }
}

#endif // I2C_ASYNC
#include "drv_I2C.h"

/* I N C L U D E S */

#include "Utils.h"
#include "features.h"

#if FEATURE_I2C

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"

/* D E F I N E S */

#define TRANSACTION_QUEUE_LENGTH 5U

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

#if I2C_ASYNC
static bool drv_I2C_private_startNextTransaction(const drv_I2C_bus_E i2cBus);
#endif

/* D A T A   D E F I N I T I O N S */

static QueueHandle_t i2cTransationQueue[DRV_I2C_BUS_COUNT];

extern drv_I2C_config_S drv_I2C_config;

/* P R I V A T E   F U N C T I O N S */

#if I2C_ASYNC
static bool drv_I2C_private_startNextTransaction(const drv_I2C_bus_E i2cBus)
{
    bool ret = false;

    drv_I2C_TransactionConfig_S xfer;
    if (xQueueReceive(i2cTransationQueue[i2cBus], &xfer, 0U))
    {
        const drv_I2C_busConfig_S *i2cBusConfig = &drv_I2C_config.busConfig[i2cBus];
        switch (xfer.transactionType)
        {
            case I2C_TRANSACTION_TX:
                (void)i2cBusConfig->writeBytes(&xfer);
                ret = true;
                break;

            case I2C_TRANSACTION_RX:
                (void)i2cBusConfig->readBytes(&xfer);
                ret = true;
                break;

            default:
                // skip
                break;
        }
    }

    return ret;
}
#endif

/* P U B L I C   F U N C T I O N S */

void drv_I2C_init(void)
{
    for (drv_I2C_bus_E bus = (drv_I2C_bus_E)0U; bus < DRV_I2C_BUS_COUNT; bus++)
    {
        i2cTransationQueue[bus] = xQueueCreate(TRANSACTION_QUEUE_LENGTH, sizeof(drv_I2C_TransactionConfig_S));
    }

    drv_I2C_hardwareSpecific_init();
}

bool drv_I2C_writeBytes(drv_I2C_device_E i2cDevice, uint8_t *txData, const uint32_t numBytes)
{
    bool ret                   = true;
    const drv_I2C_bus_E i2cBus = drv_I2C_config.deviceConfig[i2cDevice].bus;

    drv_I2C_TransactionConfig_S xfer = {
        .i2cDevice = i2cDevice, .transactionType = I2C_TRANSACTION_TX, .txBuffer = txData, .rxBuffer = NULL, .txLength = numBytes, .rxLength = 0U};

#if (I2C_ASYNC)
    if (xQueueSend(i2cTransationQueue[i2cBus], &xfer, (TickType_t)10) != pdPASS)
    {
        ret = false;
    }

    // send next transfer if I2C is not busy
    bool isBusy = false;
    if (drv_I2C_config.busConfig[i2cBus].isBusy != NULL)
    {
        isBusy = drv_I2C_config.busConfig[i2cBus].isBusy();
    }

    if (isBusy == false)
    {
        drv_I2C_private_startNextTransaction(i2cBus);
    }
#else
    const drv_I2C_busConfig_S *i2cBusConfig = &drv_I2C_config.busConfig[i2cBus];
    if (i2cBusConfig->writeBytes != NULL)
    {
        ret = i2cBusConfig->writeBytes(&xfer);
    }
#endif

    return ret;
}

// clang-format off
bool drv_I2C_readBytes(drv_I2C_device_E i2cDevice, uint8_t *txData, const uint32_t numTxBytes, uint8_t *rxData, const uint32_t numRxBytes)
{
    bool ret = true;
    // clang-format on

    const drv_I2C_bus_E i2cBus = drv_I2C_config.deviceConfig[i2cDevice].bus;

    drv_I2C_TransactionConfig_S xfer = {
        .i2cDevice       = i2cDevice,
        .transactionType = I2C_TRANSACTION_RX,
        .txBuffer        = txData,
        .rxBuffer        = rxData,
        .txLength        = numTxBytes,
        .rxLength        = numRxBytes,
    };

#if I2C_ASYNC
    if (xQueueSend(i2cTransationQueue[i2cBus], &xfer, (TickType_t)10) != pdPASS)
    {
        ret = false;
    }

    // send next transfer if I2C is not busy
    bool isBusy = false;
    if (drv_I2C_config.busConfig[i2cBus].isBusy != NULL)
    {
        isBusy = drv_I2C_config.busConfig[i2cBus].isBusy();
    }

    if (isBusy == false)
    {
        drv_I2C_private_startNextTransaction(i2cBus);
    }
#else
    const drv_I2C_busConfig_S *i2cBusConfig = &drv_I2C_config.busConfig[i2cBus];
    if (i2cBusConfig->readBytes != NULL)
    {
        ret = i2cBusConfig->readBytes(&xfer);
    }
#endif

    return ret;
}

#if I2C_ASYNC
bool drv_I2C_startNextTransactionISR(const drv_I2C_bus_E i2cBus)
{
    bool ret = false;

    drv_I2C_TransactionConfig_S xfer;
    if (xQueueReceive(i2cTransationQueue[i2cBus], &xfer, 0U))
    {
        const drv_I2C_busConfig_S *i2cBusConfig = &drv_I2C_config.busConfig[i2cBus];
        switch (xfer.transactionType)
        {
            case I2C_TRANSACTION_TX:
                (void)i2cBusConfig->writeBytes(&xfer);
                ret = true;
                break;

            case I2C_TRANSACTION_RX:
                (void)i2cBusConfig->readBytes(&xfer);
                ret = true;
                break;

            default:
                // skip
                break;
        }
    }

    return ret;
}
#endif

#endif  // FEATURE_I2C

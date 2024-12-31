#include "drv_SPI.h"

/* I N C L U D E S */

#include "features.h"

#if (FEATURE_SPI)

#include "drv_SPI_hardwareSpecific.h"

#include "FreeRTOS.h"
#include "queue.h"

/* D E F I N E S */

#define TRANSACTION_QUEUE_LENGTH 50U

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

static bool drv_SPI_private_startNextTransaction(const drv_SPI_bus_E spiBus);

/* D A T A   D E F I N I T I O N S */

static QueueHandle_t spiTransationQueue[DRV_SPI_BUS_COUNT];

extern drv_SPI_config_S drv_SPI_config;

/* P R I V A T E   F U N C T I O N S */

static bool drv_SPI_private_startNextTransaction(const drv_SPI_bus_E spiBus)
{
    bool ret = false;

    drv_SPI_TransactionConfig_S xfer;

    // peek so queue element remains in queue until transaction is complete
    if (xQueuePeek(spiTransationQueue[spiBus], &xfer, 0U))
    {

        if (xfer.startTransactionCallback != NULL)
        {
            xfer.startTransactionCallback();
        }

        const drv_SPI_busConfig_S *spiBusConfig = &drv_SPI_config.busConfig[spiBus];
        switch (xfer.transactionType)
        {
            case DRV_SPI_TRANSACTION_TYPE_WRITE:
                (void)spiBusConfig->writeBytes(&xfer);
                ret = true;
                break;

            case DRV_SPI_TRANSACTION_TYPE_READ:
                (void)spiBusConfig->readBytes(&xfer);
                ret = true;
                break;

            case DRV_SPI_TRANSACTION_TYPE_WRITE_READ:
                (void)spiBusConfig->writeReadBytes(&xfer);
                ret = true;
                break;

            default:
                // skip
                break;
        }
    }

    return ret;
}

static bool drv_SPI_private_startNextTransactionISR(const drv_SPI_bus_E spiBus)
{
    bool ret = false;

    drv_SPI_TransactionConfig_S xfer;

    // peek so queue element remains in queue until transaction is complete
    if (xQueuePeekFromISR(spiTransationQueue[spiBus], &xfer))
    {
        const drv_SPI_busConfig_S *spiBusConfig = &drv_SPI_config.busConfig[spiBus];
        switch (xfer.transactionType)
        {
            case DRV_SPI_TRANSACTION_TYPE_WRITE:
                (void)spiBusConfig->writeBytes(&xfer);
                ret = true;
                break;

            case DRV_SPI_TRANSACTION_TYPE_READ:
                (void)spiBusConfig->readBytes(&xfer);
                ret = true;
                break;

            case DRV_SPI_TRANSACTION_TYPE_WRITE_READ:
                (void)spiBusConfig->writeReadBytes(&xfer);
                ret = true;
                break;

            default:
                // skip
                break;
        }
    }

    return ret;
}

/* P U B L I C   F U N C T I O N S */

void drv_SPI_init(void)
{
    for (drv_SPI_bus_E bus = (drv_SPI_bus_E)0U; bus < DRV_SPI_BUS_COUNT; bus++)
    {
        spiTransationQueue[bus] = xQueueCreate(TRANSACTION_QUEUE_LENGTH, sizeof(drv_SPI_TransactionConfig_S));
    }

    drv_SPI_hardwareSpecific_init();
}

bool drv_SPI_write(drv_SPI_device_E spiDevice, uint8_t *txBuffer, uint32_t length, void (*startTransactionCallback)(void), void (*endTransactionCallback)(void))
{
    bool ret = true;

    drv_SPI_TransactionConfig_S xfer = {
        .spiDevice                = spiDevice,
        .transactionType          = DRV_SPI_TRANSACTION_TYPE_WRITE,
        .txBuffer                 = txBuffer,
        .rxBuffer                 = NULL,
        .length                   = length,
        .startTransactionCallback = startTransactionCallback,
        .endTransactionCallback   = endTransactionCallback,
    };

    const drv_SPI_deviceConfig_S *spiDeviceConfig = &drv_SPI_config.deviceConfig[spiDevice];
    const drv_SPI_busConfig_S *spiBusConfig       = &drv_SPI_config.busConfig[spiDeviceConfig->bus];

    if (xQueueSend(spiTransationQueue[spiDeviceConfig->bus], &xfer, (TickType_t)10) != pdPASS)
    {
        ret = false;
    }
    else
    {
        if (spiBusConfig->isBusy != NULL)
        {
            if (spiBusConfig->isBusy() == false)
            {
                drv_SPI_private_startNextTransaction(spiDeviceConfig->bus);
            }
        }
    }

    return ret;
}

bool drv_SPI_read(drv_SPI_device_E spiDevice, uint8_t *rxBuffer, uint32_t length, void (*startTransactionCallback)(void), void (*endTransactionCallback)(void))
{
    bool ret = true;

    drv_SPI_TransactionConfig_S xfer = {
        .spiDevice                = spiDevice,
        .transactionType          = DRV_SPI_TRANSACTION_TYPE_READ,
        .txBuffer                 = NULL,
        .rxBuffer                 = rxBuffer,
        .length                   = length,
        .startTransactionCallback = startTransactionCallback,
        .endTransactionCallback   = endTransactionCallback,
    };

    const drv_SPI_deviceConfig_S *spiDeviceConfig = &drv_SPI_config.deviceConfig[spiDevice];
    const drv_SPI_busConfig_S *spiBusConfig       = &drv_SPI_config.busConfig[spiDeviceConfig->bus];

    if (xQueueSend(spiTransationQueue[spiDeviceConfig->bus], &xfer, (TickType_t)10) != pdPASS)
    {
        ret = false;
    }
    else
    {
        if (spiBusConfig->isBusy != NULL)
        {
            if (spiBusConfig->isBusy() == false)
            {
                drv_SPI_private_startNextTransaction(spiDeviceConfig->bus);
            }
        }
    }

    return ret;
}

void drv_SPI_isrCallback(const drv_SPI_bus_E bus)
{
    for (drv_SPI_device_E device = (drv_SPI_device_E)0U; device < DRV_SPI_DEVICE_COUNT; device++)
    {
        const drv_SPI_deviceConfig_S *spiDeviceConfig = &drv_SPI_config.deviceConfig[device];
        if (spiDeviceConfig->bus == bus)
        {
            // set CS pin back high
            const drv_GPIO_channel_E csPin = drv_SPI_config.deviceConfig[device].csPin;
            drv_GPIO_setOutput(csPin, DRV_GPIO_HIGH);
        }
    }

    // process end of transaction callback
    BaseType_t xTaskWokenByReceive = pdFALSE;
    drv_SPI_TransactionConfig_S xfer;
    if (xQueueReceiveFromISR(spiTransationQueue[bus], &xfer, &xTaskWokenByReceive))
    {
        if (xfer.endTransactionCallback != NULL)
        {
            xfer.endTransactionCallback();
        }
    }

    (void)drv_SPI_private_startNextTransactionISR(bus);

    if (xTaskWokenByReceive != (char)pdFALSE)
    {
        taskYIELD();
    }
}

#endif  // FEATURE_SPI

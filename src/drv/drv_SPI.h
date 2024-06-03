#ifndef DRV_SPI_H_
#define DRV_SPI_H_

/* I N C L U D E S */

#include "features.h"
#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "queue.h"

#include "drv_SPI_hardwareSpecific.h"

#include "drv_GPIO.h"

#if (FEATURE_SPI)

/* D E F I N E S */

#define MAX_SPI_BUFFER_LENGTH 10U

/* T Y P E D E F S */

typedef enum
{
    DRV_SPI_TRANSACTION_TYPE_WRITE,
    DRV_SPI_TRANSACTION_TYPE_READ,
    DRV_SPI_TRANSACTION_TYPE_WRITE_READ,
} drv_SPI_transactionType_E;

typedef enum
{
    DRV_SPI_TRANSACTION_STATE_IDLE,
    DRV_SPI_TRANSACTION_STATE_BUSY,
} drv_SPI_transactionState_E;

typedef struct
{
    drv_SPI_device_E spiDevice;
    drv_SPI_transactionType_E transactionType;
    uint8_t *txBuffer;
    uint8_t *rxBuffer;
    uint32_t length;
    void (*startTransactionCallback)(void);
    void (*endTransactionCallback)(void);
} drv_SPI_TransactionConfig_S;

typedef struct
{
    bool (*writeBytes)(drv_SPI_TransactionConfig_S *xfer);
    bool (*writeReadBytes)(drv_SPI_TransactionConfig_S *xfer);
    bool (*readBytes)(drv_SPI_TransactionConfig_S *xfer);
    bool (*isBusy)(void);
} drv_SPI_busConfig_S;

typedef struct
{
    drv_GPIO_channel_E csPin;
    uint8_t frameSize;
    drv_SPI_bus_E bus;
} drv_SPI_deviceConfig_S;

typedef struct
{
    drv_SPI_busConfig_S *busConfig;
    drv_SPI_deviceConfig_S *deviceConfig;
} drv_SPI_config_S;

/* P U B L I C   F U N C T I O N S */

void drv_SPI_init(void);
void drv_SPI_registerMailbox(const drv_SPI_device_E device,
                             drv_SPI_mailbox_E mailbox,
                             uint8_t *txBuffer,
                             uint8_t *rxBuffer);
void drv_SPI_registerMailbox(const drv_SPI_device_E device,
                             drv_SPI_mailbox_E mailbox,
                             uint8_t *txBuffer,
                             uint8_t *rxBuffer);

bool drv_SPI_write(drv_SPI_device_E spiDevice,
                   uint8_t *txBuffer,
                   uint32_t length,
                   void (*startTransactionCallback)(void),
                   void (*endTransactionCallback)(void));
bool drv_SPI_read(drv_SPI_device_E spiDevice,
                  uint8_t *rxBuffer,
                  uint32_t length,
                  void (*startTransactionCallback)(void),
                  void (*endTransactionCallback)(void));

void drv_SPI_isrCallback(const drv_SPI_bus_E bus);

#endif  // FEATURE_SPI
#endif  // DRV_SPI_H_

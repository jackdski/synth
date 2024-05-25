#ifndef DRV_I2C_H_
#define DRV_I2C_H_

/* I N C L U D E S */

#include "features.h"

#include "Utils.h"
#include "main.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"

#include "drv_I2C_hardwareSpecific.h"

/* D E F I N E S */

#define MAX_BUFFER_LENGTH 10U

/* T Y P E D E F S */

typedef enum
{
    I2C_TRANSACTION_TX,
    I2C_TRANSACTION_RX
} I2C_transactionType_E;

typedef enum
{
    DRV_I2C_TRANSACTION_IDLE,
    DRV_I2C_TRANSACTION_BUSY,
} I2C_TransactionState;

typedef struct
{
    drv_I2C_device_E i2cDevice;
    I2C_transactionType_E transactionType;
    uint8_t *txBuffer;
    uint8_t *rxBuffer;

    uint32_t txLength;
    uint32_t rxLength;
} drv_I2C_TransactionConfig_S;

typedef struct
{
    bool (*writeBytes)(drv_I2C_TransactionConfig_S *xfer);
    bool (*readBytes)(drv_I2C_TransactionConfig_S *xfer);
    bool (*isBusy)(void);
} drv_I2C_busConfig_S;

typedef struct
{
    uint16_t deviceAddress;
    drv_I2C_bus_E bus;
} drv_I2C_deviceConfig_S;

typedef struct
{
    drv_I2C_busConfig_S *busConfig;
    drv_I2C_deviceConfig_S *deviceConfig;
} drv_I2C_config_S;

/* P U B L I C   F U N C T I O N S */

// clang-format off

void drv_I2C_init(void);
bool drv_I2C_writeBytes(drv_I2C_device_E i2cDevice, uint8_t *txData, const uint32_t numBytes);
bool drv_I2C_readBytes(drv_I2C_device_E i2cDevice, uint8_t *txData, const uint32_t numTxBytes, uint8_t *rxData, const uint32_t numRxBytes);

bool drv_I2C_startNextTransactionISR(const drv_I2C_bus_E i2cBus);

// clang-format on

#endif  //  DRV_I2C_H_

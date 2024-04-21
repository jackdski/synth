#ifndef I2C_BASE_HPP_
#define I2C_BASE_HPP_

#include "features.h"

#include "Utils.h"
#include "main.h"
#include "util_rollingBuffer.hpp"

#include <stdint.h>
#include <string.h>

namespace Drivers
{

#define MAX_BUFFER_LENGTH 10U

enum class I2C_transactionType
{
    I2C_TRANSACTION_TX,
    I2C_TRANSACTION_RX
};

enum class I2C_TransactionState
{
    RED,
    BLUE,
    WHITE
};

class I2CDevice
{
public:
    uint16_t deviceAddress = 0U;
    uint8_t frameSize      = 1U;

    I2CDevice(uint8_t deviceAddress, uint8_t frameSize) : deviceAddress(deviceAddress), frameSize(frameSize)
    {
    }

    I2CDevice(void)
    {
        // frameSize     = 1U;
        // deviceAddress = 0x00U;
    }
};

class I2C_Transaction
{
public:
    I2CDevice _i2cDevice;
    I2C_transactionType transactionType = I2C_transactionType::I2C_TRANSACTION_TX;
    uint8_t txBuffer[MAX_BUFFER_LENGTH];
    uint8_t rxBuffer[MAX_BUFFER_LENGTH];

    I2C_Transaction(void)
    {
    }

    I2C_Transaction(I2CDevice i2cDevice,
                    I2C_transactionType transactionType,
                    uint8_t *txData,
                    uint8_t txSize,
                    uint8_t *rxData,
                    uint8_t rxSize)
        : _i2cDevice(i2cDevice), transactionType(transactionType)
    {
        if (txBuffer != NULL)
        {
            memcpy(txBuffer, txData, txSize);
        }

        if (rxBuffer != NULL)
        {
            memcpy(rxBuffer, rxData, rxSize);
        }
    }
};

class I2CBus_base
{
private:
    static const uint32_t bufferSize = 5U;
    Utils::RollingBuffer<I2C_Transaction, 5U> transactionsBuffer;

    bool _writeI2CBytes(I2C_Transaction xfer)
    {
        UNUSED(xfer);
        return false;
    }

    bool _readI2CBytes(I2C_Transaction xfer)
    {
        UNUSED(xfer);
        return false;
    }

public:
    I2CBus_base(void){};

    bool write(I2CDevice i2cDevice, uint8_t *txData, const uint32_t numBytes)
    {
        bool ret = false;

        I2C_Transaction xfer(i2cDevice, I2C_transactionType::I2C_TRANSACTION_TX, txData, numBytes, NULL, 0U);

        ret = _writeI2CBytes(xfer);

        // if (transactionsBuffer.isFull() == false)
        // {
        //     transactionsBuffer.add(xfer);
        //     ret = true;
        // }

        // if (transactionsBuffer.getNumItems() == 1U)
        // {
        //     I2C_Transaction * newXfer = transactionsBuffer.dequeue();
        //     if (newXfer != nullptr)
        //     {
        //         switch (newXfer->transactionType)
        //         {
        //             case I2C_transactionType::I2C_TRANSACTION_TX:
        //                 ret = _writeI2CBytes(*newXfer);
        //                 break;

        //             case I2C_transactionType::I2C_TRANSACTION_RX:
        //                 ret = _readI2CBytes(*newXfer);
        //                 break;

        //             default:
        //                 // nothing
        //                 ret = false;
        //         }
        //     }
        // }
        return ret;
    }

    bool read(I2CDevice i2cDevice, const uint16_t reg, uint8_t *rxData, const uint32_t numBytes)
    {
        bool ret = false;

        uint8_t txBuffer[2U] = {
            (uint8_t)((reg & 0xFF00) >> 8U),
            (uint8_t)(reg & 0xFF),
        };

        I2C_Transaction xfer(
            i2cDevice, I2C_transactionType::I2C_TRANSACTION_RX, txBuffer, sizeof(txBuffer), rxData, numBytes);
        return _readI2CBytes(xfer);

        // if (transactionsBuffer.isFull() == false)
        // {
        //     transactionsBuffer.add(xfer);
        //     ret = true;
        // }

        // if (transactionsBuffer.getNumItems() == 1U)
        // {
        //     I2C_Transaction *newXfer;
        //     if (transactionsBuffer.dequeue())
        //     {
        //         switch (newXfer->transactionType)
        //         {
        //             case I2C_transactionType::I2C_TRANSACTION_TX:
        //                 ret = _writeI2CBytes(*newXfer);
        //                 break;

        //             case I2C_transactionType::I2C_TRANSACTION_RX:
        //                 ret = _readI2CBytes(*newXfer);
        //                 break;

        //             default:
        //                 // nothing
        //                 ret = false;
        //         }
        //     }
        // }
        return ret;
    }
};

}

#endif  //  I2C_BASE_HPP_

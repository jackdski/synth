#ifndef TCA9539_H_
#define TCA9539_H_

#include "drv_I2C.hpp"
#include "main.h"

#include <stdint.h>

namespace Devices
{
#define TCA9539_ADDRESS(A0, A1)   ((0x74U | (A1 << 1U) | A0) << 1U)

#define TCA9539_INPUT_PORT0_CMD   (0x00)
#define TCA9539_INPUT_PORT1_CMD   (0x01)
#define TCA9539_OUTPUT_PORT0_CMD  (0x02)
#define TCA9539_OUTPUT_PORT1_CMD  (0x03)
#define TCA9539_POL_INV_PORT0_CMD (0x04)
#define TCA9539_POL_INV_PORT1_CMD (0x05)
#define TCA9539_CONFIG_PORT0_CMD  (0x05)
#define TCA9539_CONFIG_PORT1_CMD  (0x05)

enum class TCA9539_ControlBits
{
    INPUT_PORT_0   = 0x00,
    INPUT_PORT_1   = 0x01,
    OUTPUT_PORT_0  = 0x02,
    OUTPUT_PORT_1  = 0x03,
    POL_INV_PORT_0 = 0x04,
    POL_INV_PORT_1 = 0x05,
    CONFIG_PORT_0  = 0x06,
    CONFIG_PORT_1  = 0x07,
};

enum class TCA9539_pinType
{
    OUTPUT = 0,
    INPUT  = 1,
};

class TCA9539_pinData
{
public:
    TCA9539_pinData(void){};

    TCA9539_pinType pinType = TCA9539_pinType::INPUT;
    bool input              = false;
    bool output             = false;
    bool invert             = false;
};

class TCA9539
{
private:
    const uint32_t port0StartPin = 0U;
    const uint32_t port0EndPin   = 7U;
    const uint32_t port1StartPin = 8U;
    const uint32_t port1EndPin   = 15U;

public:
    TCA9539_pinData pinData[16U];
    Drivers::I2CBus *i2c;
    Drivers::I2CDevice i2cDevice;

    TCA9539(Drivers::I2CBus *i2c, Drivers::I2CDevice i2cDevice) : i2c(i2c), i2cDevice(i2cDevice)
    {
    }

    void configure(void)
    {
        uint8_t txData[] = {
            static_cast<uint8_t>(TCA9539_ControlBits::CONFIG_PORT_0),  // command type
            static_cast<uint8_t>(TCA9539_ControlBits::CONFIG_PORT_1),  // port 0 data
            static_cast<uint8_t>(TCA9539_ControlBits::CONFIG_PORT_1),  // port 1 data
        };

        i2c->write(i2cDevice, txData, sizeof(txData));
    }

    void updateOutputs(void)
    {
        uint8_t port0Outputs = 0x00U;
        for (uint32_t pin = port0StartPin; pin < port0EndPin; pin++)
        {
            const bool isOutput = (pinData[pin].pinType == TCA9539_pinType::OUTPUT);
            port0Outputs |= ((pinData[pin].output && isOutput) << pin);
        }

        uint8_t port1Outputs = 0x00U;
        for (uint32_t pin = port1StartPin; pin < port1EndPin; pin++)
        {
            const bool isOutput = (pinData[pin].pinType == TCA9539_pinType::OUTPUT);
            port1Outputs |= ((pinData[pin].output && isOutput) << (pin - port1StartPin));
        }

        uint8_t txData[] = {
            static_cast<uint8_t>(TCA9539_ControlBits::OUTPUT_PORT_0),  // command type
            port0Outputs,                                              // port 0 data
            port1Outputs,                                              // port 1 data
        };

        i2c->write(i2cDevice, txData, sizeof(txData));
    }

    void updateInputs(void)
    {
        uint8_t rxBuffer[2U] = {0U};
        i2c->read(i2cDevice, static_cast<uint16_t>(TCA9539_ControlBits::INPUT_PORT_0), rxBuffer, sizeof(rxBuffer));

        for (uint32_t pin = port0StartPin; pin < port0EndPin; pin++)
        {
            pinData[pin].input = (rxBuffer[0U] && BIT_U8(pin));
        }

        for (uint32_t pin = port1StartPin; pin < port1EndPin; pin++)
        {
            pinData[pin].input = (rxBuffer[1U] && BIT_U8((pin - port1StartPin)));
        }
    }
};

}

#endif /* TCA9539_H_ */

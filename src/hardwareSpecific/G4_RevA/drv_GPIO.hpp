#ifndef GPIO_HPP_
#define DRV_GPIO_HPP_

#include "drv_GPIO_base.hpp"

#include "main.h"
#include "stm32g4xx_hal.h"

#include <stdint.h>

namespace Drivers
{

class GPIO : public GPIO_base
{
public:
    GPIO(GPIO_TypeDef *gpio, uint16 pin) : gpio(gpio), pin(pin)
    {
    }

private:
    GPIO_TypeDef *gpio;
    uint16_t pin;

    bool setOutput(GPIO_state_E state)
    {
        HAL_GPIO_WritePin(gpio, pin, (GPIO_PinState)state);
    }

    void GPIO_toggleOutput(void)
    {
        HAL_GPIO_TogglePin(gpio, pin);
    }

    bool getInput(GPIO_state_E state)
    {
        const GPIO_PinState readState = HAL_GPIO_ReadPin(gpio, pin);
        return (GPIO_state_E)readState;
    }
};

}

#endif  // DRV_GPIO_HPP_

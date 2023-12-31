#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include <stdint.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

namespace Devices
{

class Button
{
private:
    GPIO_TypeDef *gpio;
    uint32_t gpioPin;

public:
    Button(GPIO_TypeDef *gpio, uint32_t gpioPin) : gpio(gpio), gpioPin(gpioPin)
    {
    }

    bool isPressed(void)
    {
        return (HAL_GPIO_ReadPin(gpio, gpioPin) == GPIO_PIN_SET);
    }
};

}

#endif  // BUTTON_HPP_

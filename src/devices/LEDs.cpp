#include "LEDs.hpp"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

void LED::set(bool enable)
{
    HAL_GPIO_WritePin(GpioPeripheral, gpioPin, (GPIO_PinState)enable);
}

void LED::toggle(void)
{
    HAL_GPIO_TogglePin(GpioPeripheral, gpioPin);
}

bool LED::getState(void)
{
    return HAL_GPIO_ReadPin(GpioPeripheral, gpioPin);
}

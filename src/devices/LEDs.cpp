#include "LEDs.hpp"
#include "main.h"

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

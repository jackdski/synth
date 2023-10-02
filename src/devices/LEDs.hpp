#ifndef LEDS_H_
#define LEDS_H_

#include <stdbool.h>
#include <stdint.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"

class LED
{
public:
    LED(GPIO_TypeDef *GpioPeripheral, uint16_t gpioPin) : GpioPeripheral(GpioPeripheral), gpioPin(gpioPin)
    {
    }

    void set(bool enable);
    void toggle(void);
    bool getState(void);

private:
    GPIO_TypeDef *GpioPeripheral;
    uint16_t gpioPin;
};

#endif

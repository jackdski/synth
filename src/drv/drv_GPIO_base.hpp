#ifndef DRV_GPIO_BASE_HPP_
#define DRV_GPIO_BASE_HPP_

#include "features.h"

#include "Utils.h"
#include "main.h"

#if FEATURE_GPIO

namespace Drivers
{

class GPIO_base
{
public:
    typedef enum
    {
        GPIO_TYPE_INPUT,
        GPIO_TYPE_OUTPUT,
    } GPIO_type_E;

    typedef enum
    {
        GPIO_LOW,
        GPIO_HIGH,
    } GPIO_state_E;

    void GPIO_setOutput(GPIO_state_E state);
    void GPIO_toggleOutput(void);
    void GPIO_getInput(GPIO_state_E state);

private:
    GPIO_type_E type = GPIO_TYPE_INPUT;
};

}

#endif  // FEATURE_GPIO

#endif  // DRV_GPIO_BASE_HPP_

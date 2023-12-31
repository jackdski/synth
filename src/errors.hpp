#ifndef ERRORS_HPP_
#define ERRORS_HPP_

#include "Utils.h"
#include <stdint.h>

namespace Errors
{

typedef enum
{
    ERROR_NONE  = 0U,
    ERROR_I2C   = 1U,
    ERROR_I2S   = (1U << 2U),
    ERROR_AUDIO = (1U << 3U),
    ERROR_MAX   = (1U << 31U),
} Errors_E;

class Errors
{
private:
    uint32_t errors = Errors_E::ERROR_NONE;

public:
    Errors(void)
    {
    }

    void setError(const Errors_E error)
    {
        errors |= error;
    }

    void clearError(const Errors_E error)
    {
        errors &= ~error;
    }

    Errors_E getErrors(void)
    {
        return (Errors_E)errors;
    }
};

}

#endif  // ERRORS_HPP_

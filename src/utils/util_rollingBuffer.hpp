#ifndef UTIL_ROLLING_BUFFER_H_
#define UTIL_ROLLING_BUFFER_H_

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

namespace UTILS
{
template <typename T> class RollingBuffer
{
private:
    /* data */
    uint32_t head     = 0U;
    uint32_t tail     = 0U;
    uint32_t size     = 0U;
    uint32_t numItems = 0U;

    T *buffer;

public:
    void add(T item)
    {
        buffer[tail] = item;
        tail         = ((tail + 1U) % size);
        numItems++;
    }

    void reset(void)
    {
    }

    bool isFull(void)
    {
        return length == size;
    }
};
}

#endif

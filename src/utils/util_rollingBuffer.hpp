#ifndef UTIL_ROLLING_BUFFER_H_
#define UTIL_ROLLING_BUFFER_H_

#include <float.h>
#include <memory>
#include <stdbool.h>
#include <stdint.h>

namespace Utils
{

template <class T, uint32_t maxItems> class RollingBuffer
{
private:
    /* data */
    uint32_t size = 0U;

    uint32_t head     = 0U;
    uint32_t tail     = 0U;
    uint32_t numItems = 0U;

    T buffer[maxItems];

    size_t _head;
    size_t _numElements;

public:
    RollingBuffer(){};

    void add(T item)
    {
        buffer[tail] = item;
        tail         = ((tail + 1U) % size);
        numItems++;
    }

    T *dequeue(void)
    {
        if (numItems == 0U)
        {
            return nullptr;
        }

        numItems--;
        T *ret = &buffer[tail];
        tail   = ((tail + 1U) % size);
        return ret;
    }

    void reset(void)
    {
    }

    uint32_t getNumItems(void)
    {
        return numItems;
    }

    bool isFull(void)
    {
        return numItems == size;
    }
};

}

#endif

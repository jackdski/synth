#ifndef UTILS_H_
#define UTILS_H_

#include <float.h>
#include <stdint.h>

#define MIN(X, Y)                         (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y)                         (((X) > (Y)) ? (X) : (Y))

#define BIT_U8(X)                         ((uint8_t)(1U << X))
#define BIT_U16(X)                        ((uint16_t)(1U << X))
#define BIT_U32(X)                        ((uint32_t)(1U << X))

#define GET_BIT(X, BIT)                   ((X >> BIT) & 1U)

#define BIT_SET(X, Y)                     ((X) |= (1U << Y))
#define BIT_SET_VALUE(X, Y, VALUE)        ((X) |= (VALUE << Y))
#define BIT_CLEAR(X, Y)                   ((X) &= ~(1U << Y))
#define BIT_GET(X, Y)                     ((X) & (1U << Y))

#define SATURATE_MAX(X, SATURATION_VALUE) (X = (((X) >= (SATURATION_VALUE)) ? (SATURATION_VALUE) : (X)))
#define SATURATE_MIN(X, SATURATION_VALUE) (X = (((X) <= (SATURATION_VALUE)) ? (SATURATION_VALUE) : (X)))
#define SATURATE_INC(X, Y, INC)           (X = (((X) == (Y)) ? (X) : ((X) + INC)))
#define SATURATE_DEC(X, Y, INC)           (X = (((X) == (Y)) ? (X) : ((X)-INC)))

#define CONVERT_BPM_TO_TICKS(CLK_FREQ, BPM)     (CLK_FREQ * (60U / BPM))

#endif  // UTILS_H_

#ifndef UTILS_H_
#define UTILS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <float.h>
#include <stdint.h>

#define MIN(X, Y)                  (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y)                  (((X) > (Y)) ? (Y) : (X))

#define BIT_U8(X)                  ((uint8_t)(1U << X))
#define BIT_U16(X)                 ((uint16_t)(1U << X))
#define BIT_U32(X)                 ((uint32_t)(1U << X))

#define BIT_SET(X, Y)              ((X) |= (1U << Y))
#define BIT_SET_VALUE(X, Y, VALUE) ((X) |= (VALUE << Y))
#define BIT_CLEAR(X, Y)            ((X) &= ~(1U << Y))
#define BIT_GET(X, Y)              ((X) & (1U << Y))

#ifdef __cplusplus
}
#endif

#endif  // UTILS_H_

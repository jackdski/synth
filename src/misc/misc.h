#ifndef MISC_H_
#define MISC_H_

/* I N C L U D E S */

#include "features.h"

#if (FEATURE_MISC)

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* D E F I N E S */

/* T Y P E D E F S */

/* P U B L I C   F U N C T I O N S */

void misc1HzTask(void *pvParameters);
void misc10HzTask(void *pvParameters);
void misc100HzTask(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif  // FEATURE_MISC
#endif  // MISC_H_

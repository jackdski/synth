#ifndef HARDWARE_SPECIFIC_H_
#define HARDWARE_SPECIFIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "tim.h"

void hardwareSpecificInit(void);
void hardwareSpecific_deviceInit(void);

// int _write_debug(char *ptr, int len);

// profiling
void hardwareSpecific_configureTimerForRunTimeStats(void);
configRUN_TIME_COUNTER_TYPE hardwareSpecific_getRunTimeCounterValue(void);

#ifdef __cplusplus
}
#endif

#endif /* HARDWARE_SPECIFIC_H_ */

#ifndef HARDWARE_SPECIFIC_H_
#define HARDWARE_SPECIFIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"

void hardwareSpecific_init(void);
void hardwareSpecific_deinit(void);

// profiling
void hardwareSpecific_configureTimerForRunTimeStats(void);
configRUN_TIME_COUNTER_TYPE hardwareSpecific_getRunTimeCounterValue(void);

#ifdef __cplusplus
}
#endif

#endif /* HARDWARE_SPECIFIC_H_ */

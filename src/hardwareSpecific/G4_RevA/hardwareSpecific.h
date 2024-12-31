#ifndef HARDWARE_SPECIFIC_H_
#define HARDWARE_SPECIFIC_H_

#ifdef __cplusplus
extern "C" {
#endif

void hardwareSpecificInit(void);
void hardwareSpecific_deviceInit(void);

// int _write_debug(char *ptr, int len);

#ifdef __cplusplus
}
#endif

#endif /* HARDWARE_SPECIFIC_H_ */

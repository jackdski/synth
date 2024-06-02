#ifndef PCA9555_H_
#define PCA9555_H_

/* I N C L U D E S */

#include "drv_I2C.h"
#include "features.h"
#include "main.h"

#include <stdint.h>

#include "PCA9555_hardwareSpecific.h"

#if FEATURE_PCA9555

/* D E F I N E S */

#define PCA9555_ADDRESS(A0, A1)   ((0x74U | (A1 << 1U) | A0) << 1U)

#define PCA9555_INPUT_PORT0_CMD   0x00U
#define PCA9555_INPUT_PORT1_CMD   0x01U
#define PCA9555_OUTPUT_PORT0_CMD  0x02
#define PCA9555_OUTPUT_PORT1_CMD  0x03
#define PCA9555_POL_INV_PORT0_CMD 0x04
#define PCA9555_POL_INV_PORT1_CMD 0x05
#define PCA9555_CONFIG_PORT0_CMD  0x06
#define PCA9555_CONFIG_PORT1_CMD  0x07

/* T Y P E D E F S */

typedef enum
{
    PCA9555_INPUT_PORT_0   = 0x00,
    PCA9555_INPUT_PORT_1   = 0x01,
    PCA9555_OUTPUT_PORT_0  = 0x02,
    PCA9555_OUTPUT_PORT_1  = 0x03,
    PCA9555_POL_INV_PORT_0 = 0x04,
    PCA9555_POL_INV_PORT_1 = 0x05,
    PCA9555_CONFIG_PORT_0  = 0x06,
    PCA9555_CONFIG_PORT_1  = 0x07,
} PCA9555_ControlBits_E;

typedef enum
{
    PCA9555_PIN_TYPE_OUTPUT,
    PCA9555_PIN_TYPE_INPUT,
} PCA9555_pinType_E;

typedef struct
{
    PCA9555_pinType_E pinType;
    bool invert;
} PCA9555_pinConfig_S;

typedef struct
{
    drv_I2C_device_E i2cDevice;
    PCA9555_pinConfig_S *pinConfig;
    uint32_t pinConfigCount;
} PCA9555_config_S;

/* P U B L I C   F U N C T I O N S */

void PCA9555_init(PCA9555_config_S *config);
void PCA9555_configure(void);
void PCA9555_updateOutputs(void);
void PCA9555_updateInputs(void);

void PCA9555_setState(const PCA9555_channel_E channel, bool state);
bool PCA9555_getState(const PCA9555_channel_E channel);

#endif  // FEATURE_PCA9555
#endif  /* PCA9555_H_ */

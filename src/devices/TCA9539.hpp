#ifndef TCA9539_H_
#define TCA9539_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

namespace Devices
{
#define TCA9539_ADDRESS(A0, A1)   ((0x74U | (A1 << 1U) | A0) << 1U)

#define TCA9539_INPUT_PORT0_CMD   (0x00)
#define TCA9539_INPUT_PORT1_CMD   (0x01)
#define TCA9539_OUTPUT_PORT0_CMD  (0x02)
#define TCA9539_OUTPUT_PORT1_CMD  (0x03)
#define TCA9539_POL_INV_PORT0_CMD (0x04)
#define TCA9539_POL_INV_PORT1_CMD (0x05)
#define TCA9539_CONFIG_PORT0_CMD  (0x05)
#define TCA9539_CONFIG_PORT1_CMD  (0x05)

}

#endif /* TCA9539_H_ */

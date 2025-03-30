/*
 *  bootloader - main.c
**/

/* I N C L U D E S */
#include "stm32g4xx.h"
#include "stm32g4xx_hal.h"

#include "main.h"
#include "iwdg.h"
#include "usb_device.h"
#include "gpio.h"

#include "usbd_core.h"

#include <string.h>
#include <stdio.h>
#include <inttypes.h>

/* D E F I N E S */

extern uint32_t __app_start__;
#define APP_START_ADDRESS   (uint32_t *)(&__app_start__)


#define BOOTLOADER_JUMP_TO_APP_TIME_MS  3000U
#define BOOTLOADER_BLINK_DELAY_MS       500U

/* T Y P E D E F S */

typedef struct
{
    uint32_t previousTick;
} main_data_S;

typedef void (*FunctionPointer)(void);

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

static void updateBlinkLED(void);
static void deinit(void);
static void goToApp(void);

static void debugInit(void);
static void debugWrite(char * ptr, int length);

void SystemClock_Config(void);

/* P R I V A T E   D A T A   D E F I N I T I O N S */

extern IWDG_HandleTypeDef hiwdg;
extern USBD_HandleTypeDef hUsbDeviceFS;

static main_data_S data =
{
    .previousTick = 0U,
};

/* M A I N */

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    debugInit();
    printf("Bootloader init\n");
    // debugWrite(dbgMsg, strlen(dbgMsg));

    MX_GPIO_Init();
    HAL_GPIO_WritePin(LED_BLINKY_GPIO_Port, LED_BLINKY_Pin, GPIO_PIN_SET);

    // MX_IWDG_Init();
    MX_USB_Device_Init();

    // __HAL_DBGMCU_FREEZE_IWDG();
    // __HAL_IWDG_START(&hiwdg);

    __enable_irq();

    while (1)
    {
        // HAL_IWDG_Refresh(&hiwdg);
        updateBlinkLED();

        if (uwTick > BOOTLOADER_JUMP_TO_APP_TIME_MS)
        {
            goToApp();
        }
    }
}

/* P R I V A T E    F U N C T I O N S */

static void updateBlinkLED(void)
{
    if ( uwTick - data.previousTick >= BOOTLOADER_BLINK_DELAY_MS )
    {
        HAL_GPIO_TogglePin(LED_BLINKY_GPIO_Port, LED_BLINKY_Pin);
        data.previousTick = uwTick;

        // char * dbgMsg = "Blink...\n";
        // debugWrite(dbgMsg, strlen(dbgMsg));
    }
}

static void deinit(void)
{
    // de-initialize HW resources used by bootloader to their reset value
    USBD_DeInit(&hUsbDeviceFS);
    HAL_RCC_DeInit();
    HAL_GPIO_DeInit(LED_BLINKY_GPIO_Port, LED_BLINKY_Pin);

    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk); // stop SysTick

    __disable_irq();
    HAL_IWDG_Refresh(&hiwdg);
}

static void goToApp(void)
{
    FunctionPointer jumpToAddress;

    printf("Jumping to app");
    // printf("Addr: %08lx \n" PRIX32, jumpAddr);
    deinit();

#if 0
    const uint32_t jumpAddr = 0x0800E800UL;

    jumpToAddress = (FunctionPointer)(*(__IO uint32_t *)(jumpAddr + 4U));

    /* Initialize user application's stack pointer */
    __set_MSP(*(__IO uint32_t*)jumpAddr);
#else
    // const uint32_t jumpAddr = 0x08000000U;

    volatile uint32_t * addr = (volatile uint32_t *)&__app_start__;
    jumpToAddress = (FunctionPointer)((*addr + 4U));
    __set_MSP((*addr));
#endif

    jumpToAddress();
}

static void debugInit(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    CoreDebug->DHCSR = CoreDebug_DHCSR_C_DEBUGEN_Msk;
    DBGMCU->CR       = DBGMCU_CR_TRACE_IOEN;

    // from ref man pg. 2104
    ITM->LAR = 0xC5ACCE55;
    ITM->TCR = 0x00010005;  // (ITM_TCR_SWOENA_Msk | ITM_TCR_ITMENA_Msk);
    ITM->TER = 0x1U;
    ITM->TPR = 0x1U;
}

static void debugWrite(char * ptr, int length)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < length; DataIdx++)
	{
        ITM_SendChar(*ptr++);
    }
}

int _write(int fd, char *ptr, int len)
{
    if (fd == 1 || fd == 2)
    {
        debugWrite(ptr, len);
    }
    return -1;
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

/*
 *  bootloader - main.c
**/

/* I N C L U D E S */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include "main.h"
#include "iwdg.h"
#include "usb_device.h"
#include "gpio.h"

#include "usbd_core.h"

#include <string.h>

/* D E F I N E S */

#define BOOTLOADER_BLINK_DELAY_MS       1000U

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

extern uint32_t __app_start__;

extern IWDG_HandleTypeDef hiwdg;
extern PCD_HandleTypeDef hpcd_USB_OTG_HS;

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
    char * dbgMsg = "Bootloader init";
    debugWrite(dbgMsg, strlen(dbgMsg));

    MX_GPIO_Init();
    // MX_CRC_Init();
    MX_IWDG_Init();
    MX_USB_DEVICE_Init();

    __HAL_DBGMCU_FREEZE_IWDG();
    __HAL_IWDG_START(&hiwdg);

    __enable_irq();

    while (1)
    {
        HAL_IWDG_Refresh(&hiwdg);
        updateBlinkLED();

        if (uwTick > 2000U)
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
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
        data.previousTick = uwTick;
    }
}

static void deinit(void)
{
    // de-initialize HW resources used by bootloader to their reset value
    HAL_PCD_DeInit(&hpcd_USB_OTG_HS);
    HAL_RCC_DeInit();

    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk); // stop SysTick

    __disable_irq();
    HAL_IWDG_Refresh(&hiwdg);
}

static void goToApp(void)
{
    deinit();

    FunctionPointer jumpToAddress;
    jumpToAddress = (FunctionPointer)(*(__IO uint32_t *)(__app_start__ + 4U));

    // jump_to_address = (Function_Pointer)(*(__IO uint32_t *)(Address + 4U));

    /* Initialize user application's stack pointer */
    __set_MSP(*(__IO uint32_t*)__app_start__);
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


void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
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
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV8;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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

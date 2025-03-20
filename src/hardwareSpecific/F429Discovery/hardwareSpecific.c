#include "stm32f429xx.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include "main.h"
#include "iwdg.h"
// #include "usb_device.h"
#include "gpio.h"

#include <string.h>

/* D E F I N E S */

/* SVCall_IRQ_NBR added as SV_Call handler name is not the same for CM0 and for all other CMx */
#define SVCall_IRQ_NBR (IRQn_Type) - 5

/* P R I V A T E    F U N C T I O N   D E C L A R A T I O N S */

static void SystemClock_Config(void);
static void hardwareSpecific_deviceInit(void);
static void hardwareSpecific_deviceDeinit(void);
static void hardwareSpecific_debugInit(void);

/* P R I V A T E    F U N C T I O N S */

static void SystemClock_Config(void)
{
    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitTypeDef RCC_OscInitStruct = {
        .OscillatorType      = RCC_OSCILLATORTYPE_HSE,
        .HSEState            = RCC_HSE_ON,
        .LSEState            = 0U,
        .HSIState            = 0U,
        .HSICalibrationValue = 0U,
        .LSIState            = 0U,
        .PLL =
            {
                  .PLLState  = RCC_PLL_ON,
                  .PLLSource = RCC_PLLSOURCE_HSE,
                  .PLLM      = 4,
                  .PLLN      = 168,
                  .PLLP      = RCC_PLLP_DIV2,
                  .PLLQ      = 7,
                  },
    };

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Activate the Over-Drive mode
     */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {
        .ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
        .SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK,
        .AHBCLKDivider  = RCC_SYSCLK_DIV1,
        .APB1CLKDivider = RCC_HCLK_DIV4,
        .APB2CLKDivider = RCC_HCLK_DIV4,
    };

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

void hardwareSpecific_deviceInit(void)
{
}

static void hardwareSpecific_deviceDeinit(void)
{
    // set LEDs to off
    // clear screen
}

static void hardwareSpecific_debugInit(void)
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

/* P U B L I C   F U N C T I O N S */

void hardwareSpecific_init(void)
{
    HAL_Init();
    SystemClock_Config();
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    hardwareSpecific_deviceInit();

    // enable FPU
    SCB->CPACR = (SCB->CPACR | ((3UL << 10 * 2) | (3UL << 11 * 2))); /* set CP10 and CP11 Full Access */

    hardwareSpecific_debugInit();
    // char * dbgMsg = "Bootloader init";
    // debugWrite(dbgMsg, strlen(dbgMsg));

    MX_GPIO_Init();
    // MX_CRC_Init();
    MX_IWDG_Init();
    // MX_USB_DEVICE_Init();

    __HAL_DBGMCU_FREEZE_IWDG();
    __HAL_IWDG_START(&hiwdg);

    __enable_irq();

    NVIC_SetPriority(SVCall_IRQ_NBR, 0U);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

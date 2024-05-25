#include "stm32f429xx.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include "adc.h"
#include "dma.h"
#include "gpio.h"
#include "i2c.h"
#include "i2s.h"
#include "main.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

/* P R I V A T E    F U N C T I O N   D E C L A R A T I O N S */

static void SystemClock_Config(void);

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

/* P U B L I C   F U N C T I O N S */

void hardwareSpecificInit(void)
{
    HAL_Init();
    SystemClock_Config();
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    // enable FPU
    SCB->CPACR = (SCB->CPACR | ((3UL << 10 * 2) | (3UL << 11 * 2))); /* set CP10 and CP11 Full Access */

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_I2C2_Init();
    MX_I2C3_Init();
    MX_I2S2_Init();
    // MX_SDIO_SD_Init();
    MX_SPI5_Init();
    MX_USART1_UART_Init();
    MX_ADC1_Init();
    MX_I2C1_Init();
    MX_TIM10_Init();
    MX_ADC2_Init();
    MX_TIM2_Init();

    NVIC_SetPriority(SVCall_IRQ_NBR, 0U);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        HAL_IncTick();
    }
}
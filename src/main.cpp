/**
 * @file main.cpp
 */

#include "main.h"
#include "features.h"

#include "stm32f429xx.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include "adc.h"
#include "dma.h"
#include "dma2d.h"
#include "gpio.h"
#include "i2c.h"
#include "i2s.h"
#include "main.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "LEDs.hpp"
#include "audio.hpp"
#include "mixer.hpp"

#include "drv_ADC.hpp"

#include "knob.hpp"

/* SVCall_IRQ_NBR added as SV_Call handler name is not the same for CM0 and for all other CMx */
#define SVCall_IRQ_NBR (IRQn_Type) - 5

using namespace std;

/* Data definitions */
Drivers::drv_ADC volumeKnobAdc(&hadc1, ADC_CHANNEL_0);

Devices::Knob volumeKnob(volumeKnobAdc);

Devices::SGTL5000 sgtl500(&hi2c1);
Audio::Mixer mixer(sgtl500);

/* Private Functions Definitions */

static void SystemClock_Config(void);
static void PeriphCommonClock_Config(void);
static void blinky(void *pvParameters);

/* Main */
int main()
{
    HAL_Init();
    SystemClock_Config();
    PeriphCommonClock_Config();

    // enable FPU
    SCB->CPACR = (SCB->CPACR | ((3UL << 10 * 2) | (3UL << 11 * 2))); /* set CP10 and CP11 Full Access */

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_DMA2D_Init();
    MX_I2C2_Init();
    MX_I2C3_Init();
    MX_I2S2_Init();
    MX_I2S3_Init();
    // MX_SDIO_SD_Init();
    MX_SPI4_Init();
    MX_SPI5_Init();
    MX_USART1_UART_Init();
    MX_ADC1_Init();
    MX_I2C1_Init();
    MX_TIM10_Init();

    (void)xTaskCreate(blinky, "Blinky", configMINIMAL_STACK_SIZE, (void *)NULL, 0U, NULL);

#if (FEATURE_MIXER)
    (void)xTaskCreate(Audio::mixerControl, "mixerControl", configMINIMAL_STACK_SIZE, (void *)NULL, 0U, NULL);
#endif

    NVIC_SetPriority(SVCall_IRQ_NBR, 0U);
    vTaskStartScheduler();

    while (1)
    {
        // loop
    }
}

/* Private Functions */

static void blinky(void *pvParameters)
{
    LED blinkyLED(GPIOG, GPIO_PIN_13);

    while (1)
    {
        blinkyLED.toggle();
        vTaskDelay(500U);
    }
}

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

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
static void PeriphCommonClock_Config(void)
{
    /** Initializes the peripherals clock
     */
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {
        .PeriphClockSelection = RCC_PERIPHCLK_I2S,
        .PLLI2S =
            {
                     .PLLI2SN = 50,
                     .PLLI2SR = 2,
                     .PLLI2SQ = 0U,
                     },
        .PLLSAI =
            {
                     .PLLSAIN = 0U,
                     .PLLSAIQ = 0U,
                     .PLLSAIR = 0U,
                     },
        .PLLI2SDivQ        = 0U,
        .PLLSAIDivQ        = 0U,
        .PLLSAIDivR        = 0U,
        .RTCClockSelection = 0U,
        .TIMPresSelection  = 0U,
    };

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
}

/* Public Functions */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        HAL_IncTick();
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

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    // printf / breakpoint?
}
#endif /* USE_FULL_ASSERT */

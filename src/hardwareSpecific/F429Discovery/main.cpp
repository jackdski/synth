/**
 * @file main.cpp
 */

#include "main.h"
#include "features.h"

#include "stm32f429xx.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include "FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"
#include "task.h"

#include "LEDs.hpp"
#include "audio.hpp"
#include "display.hpp"
#include "mixer.hpp"
#include "note.hpp"

#include "ssd1306.hpp"

#include "button.hpp"

#include "lvgl.h"

#include "Utils.h"
#include "errors.hpp"

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
// #include "usb_device.h"

/* SVCall_IRQ_NBR added as SV_Call handler name is not the same for CM0 and for all other CMx */
#define SVCall_IRQ_NBR (IRQn_Type) - 5

using namespace std;

/* Data definitions */
Errors::Errors errors;

// Drivers::drv_ADC volumeKnobAdc(&hadc1, ADC_CHANNEL_0);

// Devices::Knob volumeKnob(volumeKnobAdc);
// Devices::Button middleCButton(B1_GPIO_Port, B1_Pin);
Drivers::I2CBus I2C1_BUS(&hi2c1);
Drivers::I2CDevice sgtl5000I2C(SGTL5000_ADDRESS, 2U);

// Devices::SGTL5000 sgtl5000(&hi2c1);
Devices::SGTL5000 sgtl5000(&I2C1_BUS, sgtl5000I2C);

// Audio::Mixer mixer(sgtl500, &hi2s2, volumeKnob, middleCButton);
Audio::Mixer mixer(sgtl5000, &hi2s2);

#if (FEATURE_DISPLAY)
Devices::SSD1306 ssd1306(&hi2c2);
Display::display mDisplay(ssd1306);
#endif

/* Private Functions Definitions */

static void SystemClock_Config(void);
static void initTask(void *pvParameters);
static void misc10HzTask(void *pvParameters);

/* Main */
int main()
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

#if (FEATURE_DISPLAY)
    lv_init();
#endif

    NVIC_SetPriority(SVCall_IRQ_NBR, 0U);
    (void)xTaskCreate(initTask, "init", configMINIMAL_STACK_SIZE, (void *)NULL, 0U, NULL);
    vTaskStartScheduler();

    while (1)
    {
        // loop
    }
}

/* Private Functions */

static void initTask(void *pvParameters)
{
    (void)xTaskCreate(misc10HzTask, "misc10Hz", configMINIMAL_STACK_SIZE, (void *)NULL, 3U, NULL);

#if (FEATURE_MIXER)
    (void)xTaskCreate(Audio::mixerControl, "mixerControl", configMINIMAL_STACK_SIZE, (void *)NULL, 1U, NULL);
#endif

#if (FEATURE_DISPLAY)
    (void)xTaskCreate(Display::displayControl, "displayControl", 512, (void *)NULL, 2U, NULL);
#endif
    vTaskDelete(NULL);
}

static void misc10HzTask(void *pvParameters)
{
    LED blinkyLED(GPIOG, GPIO_PIN_13);
    LED errorLED(GPIOG, GPIO_PIN_13);

    while (1)
    {
        const bool errorActive = (errors.getErrors() != Errors::Errors_E::ERROR_NONE);
        errorLED.set(errorActive);

        blinkyLED.toggle();
        vTaskDelay(100);
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

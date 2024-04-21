/**
 * @file main.cpp
 */

#include "main.h"
#include "features.h"

#include "stm32g4xx.h"
#include "stm32g4xx_hal.h"

#include "FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"
#include "task.h"

#include "LEDs.hpp"
#include "audio.hpp"
#include "display.hpp"
#include "mixer.hpp"
#include "sequencer.hpp"
#include "note.hpp"


#include "drv_I2C.hpp"

// devices
#include "TCA9539.hpp"
#include "ssd1306.hpp"
#include "button.hpp"

#include "lvgl.h"

#include "Utils.h"
#include "errors.hpp"

#include "app_fatfs.h"
#include "cordic.h"
#include "dma.h"
#include "fmac.h"
#include "gpio.h"
#include "i2c.h"
#include "i2s.h"
#include "rng.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "wwdg.h"

/* SVCall_IRQ_NBR added as SV_Call handler name is not the same for CM0 and for all other CMx */
#define SVCall_IRQ_NBR (IRQn_Type) - 5

using namespace std;

/* Data definitions */
Errors::Errors errors;

// Drivers
Drivers::I2CBus I2C3_BUS(&hi2c3);
Drivers::I2CDevice sgtl5000I2C(SGTL5000_ADDRESS, 2U);
Drivers::I2CDevice portExpanderButtonInputsI2C(SGTL5000_ADDRESS, 1U);
Drivers::I2CDevice portExpanderLEDI2C(SGTL5000_ADDRESS, 1U);

// Devices
Devices::SGTL5000 sgtl5000(&I2C3_BUS, sgtl5000I2C);
Devices::TCA9539 buttonInputs(&I2C3_BUS, portExpanderButtonInputsI2C);
Devices::TCA9539 ledOutputs(&I2C3_BUS, portExpanderLEDI2C);

// Application objects
Audio::Sequencer sequencer;
Audio::Mixer mixer(sgtl5000, &hi2s2);

#if (FEATURE_DISPLAY)
Devices::SSD1306 ssd1306(&hi2c2);
Display::display mDisplay(ssd1306);
#endif

/* Private Functions Definitions */

static void initTask(void *pvParameters);
static void misc10HzTask(void *pvParameters);

/* Main */
int main()
{
    HAL_Init();
    SystemClock_Config();
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    // enable FPU
    SCB->CPACR = (SCB->CPACR | ((3UL << (10 * 2)) | (3UL << (11 * 2)))); /* set CP10 and CP11 Full Access */

    MX_GPIO_Init();
    MX_DMA_Init();
    // MX_ADC1_Init();
    MX_I2C1_Init();
    MX_I2S2_Init();
    MX_SPI1_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_TIM8_Init();
    MX_WWDG_Init();
    MX_USB_Device_Init();
    MX_CORDIC_Init();
    MX_FMAC_Init();
    MX_I2C3_Init();
    MX_SPI3_Init();
    // if (MX_FATFS_Init() != APP_OK) {
    //     Error_Handler();
    // }
    MX_RNG_Init();

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
    LED blinkyLED(GPIOA, GPIO_PIN_9);

    while (1)
    {
        // const bool errorActive = (errors.getErrors() != Errors::Errors_E::ERROR_NONE);
        // errorLED.set(errorActive);

        blinkyLED.toggle();
        vTaskDelay(100);
    }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    memset(&RCC_OscInitStruct, 0U, sizeof(RCC_OscInitTypeDef));

    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    memset(&RCC_ClkInitStruct, 0U, sizeof(RCC_ClkInitTypeDef));

    /** Configure the main internal regulator output voltage
     */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.HSI48State     = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN       = 42;
    RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ       = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR       = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM6)
    {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

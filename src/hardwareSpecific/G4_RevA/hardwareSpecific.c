/*
 * hardwareSpecific.h
 */
#include "hardwareSpecific.h"

/* I N C L U D E S */

#include "features.h"
#include "stm32g4xx.h"
#include "stm32g4xx_hal.h"

#include "lvgl.h"

#include "LEDs.h"
#include "PCA9555.h"
#include "PCA9685.h"
#include "button.h"
#include "knob.h"
#include "sgtl5000.h"

#include "drv_GPIO.h"
#include "drv_I2C.h"
#include "drv_SPI.h"
#include "drv_encoder.h"

#include "adc.h"
#include "cordic.h"
#include "dma.h"
// #include "app_fatfs.h"
#include "fmac.h"
#include "gpio.h"
#include "i2c.h"
#include "i2s.h"
#include "iwdg.h"
#include "rng.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "wwdg.h"

/* D E F I N E S */

/* SVCall_IRQ_NBR added as SV_Call handler name is not the same for CM0 and for all other CMx */
#define SVCall_IRQ_NBR (IRQn_Type) - 5

/* P R I V A T E    F U N C T I O N   D E C L A R A T I O N S */

static void SystemClock_Config(void);
static void hardwareSpecific_deviceInit(void);
static void hardwareSpecific_debugInit(void);

/* P R I V A T E    F U N C T I O N S */

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
    RCC_OscInitStruct.LSIState       = RCC_LSI_ON;
    RCC_OscInitStruct.HSI48State     = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN       = 40;
    RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ       = RCC_PLLQ_DIV8;
    RCC_OscInitStruct.PLL.PLLR       = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }
}

static void hardwareSpecific_deviceInit(void)
{
#if FEATURE_PCA9555
    extern PCA9555_config_S PCA9555_config;
    PCA9555_init(&PCA9555_config);
#endif

#if FEATURE_PCA9685
    extern PCA9685_config_S PCA9685_config;
    PCA9685_init(&PCA9685_config);
#endif

#if FEATURE_LEDS
    extern LED_config_S ledConfig;
    LED_init(&ledConfig);
#endif

#if FEATURE_BUTTON
    extern Button_config_S buttonConfig;
    Button_init(&buttonConfig);
#endif

#if FEATURE_KNOB
    extern Knob_config_S Knob_config;
    Knob_init(&Knob_config);
#endif
}

static void hardwareSpecific_debugInit(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    CoreDebug->DHCSR = CoreDebug_DHCSR_C_DEBUGEN_Msk;
    DBGMCU->CR = DBGMCU_CR_TRACE_IOEN;

    // from ref man pg. 2104
    ITM->LAR = 0xC5ACCE55;
    ITM->TCR = 0x00010005; // (ITM_TCR_SWOENA_Msk | ITM_TCR_ITMENA_Msk);
    ITM->TER = 0x1U;
    ITM->TPR = 0x1U;
}

/* P U B L I C   F U N C T I O N S */

void hardwareSpecificInit(void)
{
    HAL_Init();
    SystemClock_Config();
    // HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    // enable FPU
    SCB->CPACR = (SCB->CPACR | ((3UL << (10 * 2)) | (3UL << (11 * 2)))); /* set CP10 and CP11 Full Access */

    drv_GPIO_init();
    MX_DMA_Init();
    MX_FMAC_Init();
#if FEATURE_I2C
    drv_I2C_init();
#endif
    MX_I2S2_Init();

#if FEATURE_SPI
    drv_SPI_init();
#endif

#if (FEATURE_FATFS)
    if (MX_FATFS_Init() != APP_OK)
    {
        Error_Handler();
    }
#endif

    // MX_USB_Device_Init();
    // MX_ADC1_Init();
    MX_CORDIC_Init();
#if FEATURE_ENCODER
    drv_encoder_init();
#endif
    MX_TIM15_Init();
    extern TIM_HandleTypeDef htim15;
    (void)HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);

    MX_TIM6_Init();
    // MX_IWDG_Init();
    // MX_RNG_Init();
    // MX_WWDG_Init();

    hardwareSpecific_deviceInit();

#if FEATURE_DISPLAY
    lv_init();
#endif

    NVIC_SetPriority(SVCall_IRQ_NBR, 0U);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        HAL_IncTick();
    }
}

int _write(int file, char *ptr, int len)
{
     int DataIdx;
     for (DataIdx = 0; DataIdx < len; DataIdx++)
     {
         ITM_SendChar(*ptr++);
     }
     return len;
}


/* I N C L U D E S */

#include "sgtl5000.h"

#include "features.h"

#include "drv_I2C.h"
#include "main.h"
#include <string.h>

#if FEATURE_SGTL5000

/* D E F I N E S */

/* T Y P E D E F S */

typedef struct
{
    // raw register values
    uint16_t chipIdRegister;
    uint16_t digPowerRegister;
    uint16_t clkCtrlRegister;
    uint16_t i2sCtrlRegister;
    uint16_t sssCtrlRegister;
    uint16_t adcDacCtrlRegister;
    uint16_t dacVolRegister;
    uint16_t chipPadStrengthRegister;
    uint16_t anaAdcCtrlRegister;
    uint16_t anaHpCtrlRegister;
    uint16_t anaCtrlRegister;
    uint16_t linRegCtrlRegister;
    uint16_t refCtrlRegister;
    uint16_t micCtrlRegister;
    uint16_t chipLineOutCtrlRegister;
    uint16_t chipLineOutVolRegister;
    uint16_t anaPowerRegister;
    uint16_t pllCtrlRegister;
    uint16_t clkTopCtrlRegister;
    uint16_t anaStatusRegister;
    uint16_t anaTest1Register;
    uint16_t anaTest2Register;
    uint16_t shortCtrlRegister;
    // uint16_t dapControlRegister;
    // uint16_t dapPeqRegister;
    // uint16_t dapBassEnhanceRegister;
    // uint16_t dapBassEnhanceCtrlRegister;
    // uint16_t dapAudioEqRegister;
    // uint16_t dpSgtlSurroundRegister;
    // uint16_t dapFilterCoefAccessRegister;
    // uint16_t dapCoefWrB0MsbRegister;
    // uint16_t dapCoefWrB0LsbRegister;
    // uint16_t dapAudioEqBassBand0Register;
    // uint16_t dapAudioEqBassBand1Register;
    // uint16_t dapAudioEqBassBand2Register;
    // uint16_t dapAudioEqBassBand3Register;
    // uint16_t dapAudioEqTrebleBand4Register;
    // uint16_t dapMainChanRegister;
    // uint16_t dapMixChanRegister;
} SGTL5000_registerData_S;

typedef struct
{
    bool dacRightVolBusy;
    bool dacLeftVolBusy;
    bool volumeRampEnabled;
    bool dacMuteRight;
    bool dacMuteLeft;

    SGTL5000_registerData_S registerData;

    // float32_t                   dacVolumeDb;        // 0dB to -90dB in 0.5017dB steps
    // float32_t                   headphoneVolumeDb;  // +12dB to -51.5dB in 0.5dB steps
} SGTL5000_data_S;

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* D A T A   D E F I N I T I O N S */

SGTL5000_config_S config = {
    .i2cDevice      = DRV_I2C_DEVICE_SGTL5000,
    .rateMode       = SGTL5000_RATE_MODE_SYS_FS,
    .sysFs          = SGTL5000_DIG_POWER_SYS_FS_44_1_KHZ,
    .mclkFreq       = SGTL5000_MCLK_FREQ_256_FS,
    .lineOutCurrent = SGTL5000_LINE_OUT_CURRENT_0_54_MA,
};

static SGTL5000_data_S SGTL5000_data;

/* P R I V A T E   F U N C T I O N S */

/* P U B L I C   F U N C T I O N S */

bool SGTL5000_init(void)
{
    memset(&SGTL5000_data, 0U, sizeof(SGTL5000_data_S));
    SGTL5000_configure();
    return true;
}

bool SGTL5000_configure(void)
{
    bool initSuccessful = true;

    {
        const uint16_t anaPowerData = ((1U << SGTL5000_DAC_MONO_POWER_POS) | (1U << SGTL5000_STARTUP_POWERUP_POS) |
                                       (1U << SGTL5000_ADC_MONO_POWERUP_POS) | (1U << SGTL5000_REFTOP_POWERUP_POS));

        initSuccessful &= SGTL5000_writeI2C(SGTL5000_CHIP_ANA_POWER_REG, anaPowerData);
    }

    {
        const uint16_t linRegData = ((1U << SGTL5000_VDDC_MAIN_ASSN_POS) | (1U << SGTL5000_VDDC_MAIN_OVRD_POS) |
                                     SGTL5000_D_PROGRAMMING_GET_VAL_FROM_MV(1000U));

        initSuccessful &= SGTL5000_writeI2C(SGTL5000_CHIP_LINREG_CTRL_REG, linRegData);
    }

    // CHIP_REF_CTRL
    {
        const uint16_t chipRefData =
            ((SGTL5000_CHIP_REF_CTRL_VAG_VAL_1_575V << SGTL5000_CHIP_REF_CTRL_VAG_VAL_POS) |
             (SGTL5000_CHIP_REF_CTRL_BIAS_CTRL_pos12_5p << SGTL5000_CHIP_REF_CTRL_BIAS_CTRL_POS));

        initSuccessful &= SGTL5000_writeI2C(SGTL5000_CHIP_REF_CTRL_REG, chipRefData);
    }

    // CHIP_LINE_OUT_CTRL
    {
        const uint16_t lineOutData =
            ((SGTL5000_LINE_OUT_CURRENT_0_54_MA << SGTL5000_LINE_OUT_CURRENT_POS) | (SGTL5000_LO_ANALOG_VOLTAGE_MAX));

        initSuccessful &= SGTL5000_writeI2C(SGTL5000_CHIP_LINE_OUT_CTRL_REG, lineOutData);
    }

    // CHIP_SHORT_CTRL
    {
        const uint16_t shortCtrlData =
            ((SGTL5000_CHIP_SHORT_CTRL_LVLADJR_125MA << SGTL5000_CHIP_SHORT_CTRL_LVLADJR_POS) |
             (SGTL5000_CHIP_SHORT_CTRL_LVLADJL_125MA << SGTL5000_CHIP_SHORT_CTRL_LVLADJR_POS) |
             (SGTL5000_CHIP_SHORT_CTRL_LVLADJC_250MA << SGTL5000_CHIP_SHORT_CTRL_LVLADJC_POS) |
             (SGTL5000_CHIP_SHORT_CTRL_MODE_LR_ENABLE_MANUAL_RESET << SGTL5000_CHIP_SHORT_CTRL_MODE_LR_POS) |
             (SGTL5000_CHIP_SHORT_CTRL_MODE_CM_ENABLE_AUTO_RESET << SGTL5000_CHIP_SHORT_CTRL_MODE_CM_POS));

        initSuccessful &= SGTL5000_writeI2C(SGTL5000_CHIP_SHORT_CTRL_REG, shortCtrlData);
    }

    // CHIP_ANA_CTRL
    {
        const uint16_t anaCtrlData = ((SGTL5000_MUTE_LINEOUT_MUTE_ENABLED) | (SGTL5000_EN_ZCD_HEADPHONE_ENABLED) |
                                      (SGTL5000_MUTE_HEADPHONE_OUTPUT_MUTE_ENABLED) |
                                      (SGTL5000_SELECT_ADC_INPUT_LINEIN << SGTL5000_SELECT_ADC_INPUT_POS) |
                                      (SGTL5000_EN_ZCD_ADC_ENABLED) | (SGTL5000_MUTE_ADC_MUTE_ENABLED));

        initSuccessful &= SGTL5000_writeI2C(SGTL5000_CHIP_ANA_CTRL_REG, anaCtrlData);
    }

    {
        const uint16_t digPowerData =
            ((1U << SGTL5000_DIG_POWER_ADC_POWERUP_POS) | (1U << SGTL5000_DIG_POWER_DAC_POWERUP_POS) |
             (1U << SGTL5000_DIG_POWER_DAP_POWERUP_POS) | (1U << SGTL5000_DIG_POWER_I2S_OUT_POWERUP_POS) |
             (1U << SGTL5000_DIG_POWER_I2S_IN_POWERUP_POS));

        initSuccessful &= SGTL5000_writeI2C(SGTL5000_DIG_POWER_REG, digPowerData);
    }

    {
        const uint16_t anaPowerData =
            ((1U << SGTL5000_DAC_MONO_POWER_POS) | (1U << SGTL5000_STARTUP_POWERUP_POS) |
             (1U << SGTL5000_ANALOG_VOLTAGE_POWERUP_POS) | (1U << SGTL5000_ADC_MONO_POWERUP_POS) |
             (1U << SGTL5000_REFTOP_POWERUP_POS) | (1U << SGTL5000_HEADPHONE_POWERUP_POS) |
             (1U << SGTL5000_DAC_POWERUP_POS) | (1U << SGTL5000_CAPLESS_HEADPHONE_POWERUP_POS) |
             (1U << SGTL5000_ADC_POWERUP_POS) | (1U << SGTL5000_LINEOUT_POWERUP_POS));

        initSuccessful &= SGTL5000_writeI2C(SGTL5000_CHIP_ANA_POWER_REG, anaPowerData);
    }

    {
        // 400ms wait?
        const uint16_t lineOutVolData = 0x1D1D;  // ??
        initSuccessful &= SGTL5000_writeI2C(SGTL5000_LINE_OUT_VOL_REG, lineOutVolData);
    }

    {
        const uint16_t chipClkData =
            ((SGTL5000_RATE_MODE_SYS_FS << SGTL5000_CHIP_CLK_CTRL_RATE_MODE_POS) |  // rate mode
             (SGTL5000_DIG_POWER_SYS_FS_44_1_KHZ << SGTL5000_CHIP_CLK_CTRL_SYS_FS_POS) | (SGTL5000_MCLK_FREQ_256_FS));

        initSuccessful &= SGTL5000_writeI2C(SGTL5000_CHIP_CLK_CTRL_REG, chipClkData);
    }

    {
        const uint16_t chipI2sConfigData = (SGTL5000_CHIP_I2S_CTRL_SCLKFREQ_32FS | SGTL5000_CHIP_I2S_CTRL_I2S_SLAVE |
                                            SGTL5000_CHIP_I2S_CTRL_SCLK_INV_RISING_EDGE |
                                            (SGTL5000_DATALENGTH_16_BITS << SGTL5000_CHIP_I2S_CTRL_DLEN_POS) |
                                            (SGTL5000_I2S_MODE_LEFT_JUSTIFIED << SGTL5000_CHIP_I2S_CTRL_I2S_MODE_POS));

        initSuccessful &= SGTL5000_writeI2C(SGTL5000_CHIP_I2S_CTRL_REG, chipI2sConfigData);
    }

    {
        const uint16_t sssCtrlConfigData = SGTL5000_CHIP_SSS_CTRL_DAC_SELECT_I2S_IN;  // I2S->DAC
        initSuccessful &= SGTL5000_writeI2C(SGTL5000_CHIP_SSS_CTRL_REG, sssCtrlConfigData);
    }

    {
        const uint16_t adcDacCtrlConfigData =
            // ((1U << SGTL5000_ADC_DAC_CTRL_VOL_RAMP_EN_POS) | (0U << SGTL5000_ADC_DAC_CTRL_DAC_MUTE_RIGHT_POS) |
            ((0U << SGTL5000_ADC_DAC_CTRL_DAC_MUTE_RIGHT_POS) |
             (0U << SGTL5000_ADC_DAC_CTRL_DAC_MUTE_LEFT_POS));  // enable volume ramp, disable DAC L+R mute

        initSuccessful &= SGTL5000_writeI2C(SGTL5000_CHIP_ADC_DAC_CTRL_REG, adcDacCtrlConfigData);
    }

    {
        const uint16_t dacVolConfigData =
            (SGTL5000_DAC_VOL_SET_RIGHT_VOLUME(SGTL5000_DAC_VOL_GET_DB_VALUE(-12.0f)) |
             SGTL5000_DAC_VOL_SET_LEFT_VOLUME(SGTL5000_DAC_VOL_GET_DB_VALUE(-12.0f)));  // digital gain, 0dB
        initSuccessful &= SGTL5000_writeI2C(SGTL5000_CHIP_DAC_VOL_REG, dacVolConfigData);
    }

    // ANA_HP_CTRL
    {
        // const uint16_t hpConfigData = 0x7F7F;  // set HP volume (lowest level)
        const uint16_t hpConfigData = ((0x40 << SGTL5000_HP_VOL_RIGHT_POS) | (0x40));
        initSuccessful &= SGTL5000_writeI2C(SGTL5000_CHIP_ANA_HP_CTRL_REG, hpConfigData);
    }
    // ANA_CTRL
    {
        const uint16_t anaCtrlConfigData = (SGTL5000_MUTE_LINEOUT_MUTE_ENABLED | SGTL5000_EN_ZCD_HEADPHONE_ENABLED |
                                            SGTL5000_EN_ZCD_ADC_ENABLED | SGTL5000_MUTE_ADC_MUTE_ENABLED);
        initSuccessful &= SGTL5000_writeI2C(SGTL5000_CHIP_ANA_CTRL_REG, anaCtrlConfigData);
    }

    // DAP_CONTROL
    {
        const uint16_t dapControlConfigData = (1U << SGTL5000_DAP_EN_POS);
        initSuccessful &= SGTL5000_writeI2C(SGTL5000_DAP_CONTROL_REG, dapControlConfigData);
    }
    return initSuccessful;
}

bool SGTL5000_pollRegisters(void)
{
    bool ret = true;

    SGTL5000_registerData_S *registerData = &SGTL5000_data.registerData;

    ret &= SGTL5000_readI2C(SGTL5000_CHIP_ID_REG, &registerData->chipIdRegister);
    ret &= SGTL5000_readI2C(SGTL5000_DIG_POWER_REG, &registerData->digPowerRegister);
    ret &= SGTL5000_readI2C(SGTL5000_CHIP_CLK_CTRL_REG, &registerData->clkCtrlRegister);
    ret &= SGTL5000_readI2C(SGTL5000_CHIP_I2S_CTRL_REG, &registerData->i2sCtrlRegister);
    ret &= SGTL5000_readI2C(SGTL5000_CHIP_SSS_CTRL_REG, &registerData->sssCtrlRegister);
    ret &= SGTL5000_readI2C(SGTL5000_CHIP_ADC_DAC_CTRL_REG, &registerData->adcDacCtrlRegister);
    ret &= SGTL5000_readI2C(SGTL5000_CHIP_DAC_VOL_REG, &registerData->dacVolRegister);
    // ret &= SGTL5000_readI2C(SGTL5000_PAD_S, &registerData->chipPadStrengthRegister);
    // ret &= SGTL5000_readI2C(SGTL5000_ADC_CTRL_REG, &registerData->anaAdcCtrlRegister);
    ret &= SGTL5000_readI2C(SGTL5000_CHIP_ANA_HP_CTRL_REG, &registerData->anaHpCtrlRegister);
    ret &= SGTL5000_readI2C(SGTL5000_CHIP_ANA_CTRL_REG, &registerData->anaCtrlRegister);
    ret &= SGTL5000_readI2C(SGTL5000_CHIP_LINREG_CTRL_REG, &registerData->linRegCtrlRegister);
    ret &= SGTL5000_readI2C(SGTL5000_CHIP_REF_CTRL_REG, &registerData->refCtrlRegister);
    ret &= SGTL5000_readI2C(SGTL5000_CHIP_MIC_CTRL_REG, &registerData->micCtrlRegister);
    ret &= SGTL5000_readI2C(SGTL5000_CHIP_LINE_OUT_CTRL_REG, &registerData->chipLineOutCtrlRegister);
    ret &= SGTL5000_readI2C(SGTL5000_LINE_OUT_VOL_REG, &registerData->chipLineOutVolRegister);
    ret &= SGTL5000_readI2C(SGTL5000_CHIP_ANA_POWER_REG, &registerData->anaPowerRegister);
    ret &= SGTL5000_readI2C(SGTL5000_CHIP_PLL_CTRL_REG, &registerData->pllCtrlRegister);
    ret &= SGTL5000_readI2C(SGTL5000_CHIP_CLK_TOP_CTRL_REG, &registerData->clkTopCtrlRegister);
    ret &= SGTL5000_readI2C(SGTL5000_CHIP_ANA_STATUS_REG, &registerData->anaStatusRegister);
    // ret &= SGTL5000_readI2C(SGTL5000_ANA_TEST_1, &registerData->anaTest1Register);
    // ret &= SGTL5000_readI2C(SGTL5000_CHIP_ID_REG, &registerData->anaTest2Register);
    // ret &= SGTL5000_readI2C(SGTL5000_SHORT, &registerData->shortCtrlRegister);

    return ret;
}

bool SGTL5000_writeI2C(const uint16_t reg, const uint16_t txData)
{
    uint8_t txBuffer[] = {
        (uint8_t)((reg & 0xFF00) >> 8U),
        (uint8_t)(reg & 0x00FF),
        (uint8_t)((txData & 0xFF00) >> 8U),
        (uint8_t)(txData & 0x00FF),
    };

    return drv_I2C_writeBytes(config.i2cDevice, txBuffer, sizeof(txBuffer));
}

bool SGTL5000_readI2C(const uint16_t reg, uint16_t *rxData)
{
    uint8_t txBuffer[] = {
        (uint8_t)((reg & 0xFF00) >> 8U),
        (uint8_t)(reg & 0x00FF),
    };

    uint8_t privateRXData[2U] = {0U};
    const bool ret            = drv_I2C_readBytes(config.i2cDevice, txBuffer, sizeof(txBuffer), privateRXData, 2U);

    if (rxData != NULL)
    {
        *rxData = ((privateRXData[1U] << 8U) | (privateRXData[0U]));
    }

    return ret;
}

void SGTL5000_updateVolume(float volume)
{
    // range: [+12dB, -51.5dB]
    const float dB              = -1.0f * ((63.5f * volume) - 12.0f);
    const uint16_t hpConfigData = (((uint8_t)dB << SGTL5000_HP_VOL_RIGHT_POS) | (uint8_t)dB);
    (void)SGTL5000_writeI2C(SGTL5000_CHIP_ANA_HP_CTRL_REG, hpConfigData);
}

#endif  // FEATURE_SGTL5000

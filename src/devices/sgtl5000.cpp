#include "sgtl5000.hpp"
#include "drv_I2C.hpp"
#include "main.h"

using namespace Devices;

bool SGTL5000::init(void)
{
    bool initSuccessful = true;

    {
        const uint16_t anaPowerData = ((1U << SGTL5000_DAC_MONO_POWER_POS) | (1U << SGTL5000_STARTUP_POWERUP_POS) |
                                       (1U << SGTL5000_ADC_MONO_POWERUP_POS) | (1U << SGTL5000_REFTOP_POWERUP_POS));

        initSuccessful &= writeI2C(SGTL5000_CHIP_ANA_POWER_REG, anaPowerData);
    }

    const uint16_t linRegData = ((1U << SGTL5000_VDDC_MAIN_ASSN_POS) | (1U << SGTL5000_VDDC_MAIN_OVRD_POS) |
                                 SGTL5000_D_PROGRAMMING_GET_VAL_FROM_MV(1000U));

    initSuccessful &= writeI2C(SGTL5000_CHIP_LINREG_CTRL_REG, linRegData);

    // CHIP_REF_CTRL
    const uint16_t chipRefData = ((SGTL5000_CHIP_REF_CTRL_VAG_VAL_1_575V << SGTL5000_CHIP_REF_CTRL_VAG_VAL_POS) |
                                  (SGTL5000_CHIP_REF_CTRL_BIAS_CTRL_pos12_5p << SGTL5000_CHIP_REF_CTRL_BIAS_CTRL_POS));

    initSuccessful &= writeI2C(SGTL5000_CHIP_REF_CTRL_REG, chipRefData);

    // CHIP_LINE_OUT_CTRL
    const uint16_t lineOutData =
        ((SGTL5000_LINE_OUT_CURRENT_0_54_MA << SGTL5000_LINE_OUT_CURRENT_POS) | (SGTL5000_LO_ANALOG_VOLTAGE_MAX));

    initSuccessful &= writeI2C(SGTL5000_CHIP_LINE_OUT_CTRL_REG, lineOutData);

    // CHIP_SHORT_CTRL
    const uint16_t shortCtrlData =
        ((SGTL5000_CHIP_SHORT_CTRL_LVLADJR_125MA << SGTL5000_CHIP_SHORT_CTRL_LVLADJR_POS) |
         (SGTL5000_CHIP_SHORT_CTRL_LVLADJL_125MA << SGTL5000_CHIP_SHORT_CTRL_LVLADJR_POS) |
         (SGTL5000_CHIP_SHORT_CTRL_LVLADJC_250MA << SGTL5000_CHIP_SHORT_CTRL_LVLADJC_POS) |
         (SGTL5000_CHIP_SHORT_CTRL_MODE_LR_ENABLE_MANUAL_RESET << SGTL5000_CHIP_SHORT_CTRL_MODE_LR_POS) |
         (SGTL5000_CHIP_SHORT_CTRL_MODE_CM_ENABLE_AUTO_RESET << SGTL5000_CHIP_SHORT_CTRL_MODE_CM_POS));

    initSuccessful &= writeI2C(SGTL5000_CHIP_SHORT_CTRL_REG, shortCtrlData);

    // CHIP_ANA_CTRL
    const uint16_t anaCtrlData = ((SGTL5000_MUTE_LINEOUT_MUTE_ENABLED) | (SGTL5000_EN_ZCD_HEADPHONE_ENABLED) |
                                  (SGTL5000_MUTE_HEADPHONE_OUTPUT_MUTE_ENABLED) |
                                  (SGTL5000_SELECT_ADC_INPUT_LINEIN << SGTL5000_SELECT_ADC_INPUT_POS) |
                                  (SGTL5000_EN_ZCD_ADC_ENABLED) | (SGTL5000_MUTE_ADC_MUTE_ENABLED));

    initSuccessful &= writeI2C(SGTL5000_CHIP_ANA_CTRL_REG, anaCtrlData);

    const uint16_t digPowerData =
        ((1U << SGTL5000_DIG_POWER_ADC_POWERUP_POS) | (1U << SGTL5000_DIG_POWER_DAC_POWERUP_POS) |
         (1U << SGTL5000_DIG_POWER_DAP_POWERUP_POS) | (1U << SGTL5000_DIG_POWER_I2S_OUT_POWERUP_POS) |
         (1U << SGTL5000_DIG_POWER_I2S_IN_POWERUP_POS));

    initSuccessful &= writeI2C(SGTL5000_DIG_POWER_REG, digPowerData);

    {
        const uint16_t anaPowerData =
            ((1U << SGTL5000_DAC_MONO_POWER_POS) | (1U << SGTL5000_STARTUP_POWERUP_POS) |
             (1U << SGTL5000_ANALOG_VOLTAGE_POWERUP_POS) | (1U << SGTL5000_ADC_MONO_POWERUP_POS) |
             (1U << SGTL5000_REFTOP_POWERUP_POS) | (1U << SGTL5000_HEADPHONE_POWERUP_POS) |
             (1U << SGTL5000_DAC_POWERUP_POS) | (1U << SGTL5000_CAPLESS_HEADPHONE_POWERUP_POS) |
             (1U << SGTL5000_ADC_POWERUP_POS) | (1U << SGTL5000_LINEOUT_POWERUP_POS));

        initSuccessful &= writeI2C(SGTL5000_CHIP_ANA_POWER_REG, anaPowerData);
    }

    // 400ms wait?
    const uint16_t lineOutVolData = 0x1D1D;  // ??
    initSuccessful &= writeI2C(SGTL5000_LINE_OUT_VOL_REG, lineOutVolData);

    const uint16_t chipClkData =
        ((SGTL5000_RATE_MODE_SYS_FS << SGTL5000_CHIP_CLK_CTRL_RATE_MODE_POS) |  // rate mode
         (SGTL5000_DIG_POWER_SYS_FS_44_1_KHZ << SGTL5000_CHIP_CLK_CTRL_SYS_FS_POS) | (SGTL5000_MCLK_FREQ_256_FS));

    initSuccessful &= writeI2C(SGTL5000_CHIP_CLK_CTRL_REG, chipClkData);

    const uint16_t chipI2sConfigData = (SGTL5000_CHIP_I2S_CTRL_SCLKFREQ_32FS | SGTL5000_CHIP_I2S_CTRL_I2S_SLAVE |
                                        SGTL5000_CHIP_I2S_CTRL_SCLK_INV_RISING_EDGE |
                                        (SGTL5000_DATALENGTH_16_BITS << SGTL5000_CHIP_I2S_CTRL_DLEN_POS) |
                                        (SGTL5000_I2S_MODE_LEFT_JUSTIFIED << SGTL5000_CHIP_I2S_CTRL_I2S_MODE_POS));

    initSuccessful &= writeI2C(SGTL5000_CHIP_I2S_CTRL_REG, chipI2sConfigData);

    const uint16_t sssCtrlConfigData = SGTL5000_CHIP_SSS_CTRL_DAC_SELECT_I2S_IN;  // I2S->DAC
    initSuccessful &= writeI2C(SGTL5000_CHIP_SSS_CTRL_REG, sssCtrlConfigData);

    const uint16_t adcDacCtrlConfigData =
        // ((1U << SGTL5000_ADC_DAC_CTRL_VOL_RAMP_EN_POS) | (0U << SGTL5000_ADC_DAC_CTRL_DAC_MUTE_RIGHT_POS) |
        ((0U << SGTL5000_ADC_DAC_CTRL_DAC_MUTE_RIGHT_POS) |
         (0U << SGTL5000_ADC_DAC_CTRL_DAC_MUTE_LEFT_POS));  // enable volume ramp, disable DAC L+R mute

    initSuccessful &= writeI2C(SGTL5000_CHIP_ADC_DAC_CTRL_REG, adcDacCtrlConfigData);

    const uint16_t dacVolConfigData =
        (SGTL5000_DAC_VOL_SET_RIGHT_VOLUME(SGTL5000_DAC_VOL_GET_DB_VALUE(-12.0f)) |
         SGTL5000_DAC_VOL_SET_LEFT_VOLUME(SGTL5000_DAC_VOL_GET_DB_VALUE(-12.0f)));  // digital gain, 0dB
    initSuccessful &= writeI2C(SGTL5000_CHIP_DAC_VOL_REG, dacVolConfigData);

    // ANA_HP_CTRL
    // const uint16_t hpConfigData = 0x7F7F;  // set HP volume (lowest level)
    const uint16_t hpConfigData = ((0x40 << SGTL5000_HP_VOL_RIGHT_POS) | (0x40));
    initSuccessful &= writeI2C(SGTL5000_CHIP_ANA_HP_CTRL_REG, hpConfigData);

    // ANA_CTRL
    const uint16_t anaCtrlConfigData = (SGTL5000_MUTE_LINEOUT_MUTE_ENABLED | SGTL5000_EN_ZCD_HEADPHONE_ENABLED |
                                        SGTL5000_EN_ZCD_ADC_ENABLED | SGTL5000_MUTE_ADC_MUTE_ENABLED);
    initSuccessful &= writeI2C(SGTL5000_CHIP_ANA_CTRL_REG, anaCtrlConfigData);

    // DAP_CONTROL
    const uint16_t dapControlConfigData = (1U << SGTL5000_DAP_EN_POS);
    initSuccessful &= writeI2C(SGTL5000_DAP_CONTROL_REG, dapControlConfigData);

    return initSuccessful;
}

bool SGTL5000::updateRegisters(void)
{
    bool ret = true;

    ret &= (readI2C(SGTL5000_CHIP_ID_REG, &chipIdRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_DIG_POWER_REG, &digPowerRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_CHIP_CLK_CTRL_REG, &clkCtrlRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_CHIP_I2S_CTRL_REG, &i2sCtrlRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_CHIP_SSS_CTRL_REG, &sssCtrlRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_CHIP_ADC_DAC_CTRL_REG, &adcDacCtrlRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_CHIP_DAC_VOL_REG, &dacVolRegister) == HAL_OK);
    // ret &= (readI2C(SGTL5000_PAD_S, &chipPadStrengthRegister) == HAL_OK);
    // ret &= (readI2C(SGTL5000_ADC_CTRL_REG, &anaAdcCtrlRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_CHIP_ANA_HP_CTRL_REG, &anaHpCtrlRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_CHIP_ANA_CTRL_REG, &anaCtrlRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_CHIP_LINREG_CTRL_REG, &linRegCtrlRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_CHIP_REF_CTRL_REG, &refCtrlRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_CHIP_MIC_CTRL_REG, &micCtrlRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_CHIP_LINE_OUT_CTRL_REG, &chipLineOutCtrlRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_LINE_OUT_VOL_REG, &chipLineOutVolRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_CHIP_ANA_POWER_REG, &anaPowerRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_CHIP_PLL_CTRL_REG, &pllCtrlRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_CHIP_CLK_TOP_CTRL_REG, &clkTopCtrlRegister) == HAL_OK);
    ret &= (readI2C(SGTL5000_CHIP_ANA_STATUS_REG, &anaStatusRegister) == HAL_OK);
    // ret &= (readI2C(SGTL5000_ANA_TEST_1, &anaTest1Register) == HAL_OK);
    // ret &= (readI2C(SGTL5000_CHIP_ID_REG, &anaTest2Register) == HAL_OK);
    // ret &= (readI2C(SGTL5000_SHORT, &shortCtrlRegister) == HAL_OK);

    return ret;
}

bool SGTL5000::writeI2C(const uint16_t reg, const uint16_t txData)
{
    uint8_t txBuffer[] = {
        (uint8_t)((reg & 0xFF00) >> 8U),
        (uint8_t)(reg & 0x00FF),
        (uint8_t)((txData & 0xFF00) >> 8U),
        (uint8_t)(txData & 0x00FF),
    };

    return i2c->write(i2cDevice, txBuffer, sizeof(txBuffer));
}

bool SGTL5000::readI2C(const uint16_t reg, uint16_t *rxData)
{
    return i2c->read(i2cDevice, reg, reinterpret_cast<uint8_t *>(rxData), sizeof(rxData));
}

void SGTL5000::updateVolume(float volume)
{
    // range: [+12dB, -51.5dB]
    const float dB              = -1.0f * ((63.5f * volume) - 12.0f);
    const uint16_t hpConfigData = (((uint8_t)dB << SGTL5000_HP_VOL_RIGHT_POS) | (uint8_t)dB);
    (void)writeI2C(SGTL5000_CHIP_ANA_HP_CTRL_REG, hpConfigData);
}

#include "sgtl5000.hpp"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

bool SGTL5000::init(void)
{
    bool initSuccessful = true;

    uint16_t chipIDRxBuffer = 0U;
    initSuccessful &= readI2C(SGTL5000_CHIP_ID_REG, &chipIDRxBuffer);

    const uint16_t anaPowerBuffer = ((1U << SGTL5000_STARTUP_POWERUP_POS) | (1U << SGTL5000_ADC_MONO_POWERUP_POS) |
                                     (1U << SGTL5000_REFTOP_POWERUP_POS));

    initSuccessful &= writeI2C(SGTL5000_CHIP_ANA_POWER_REG, anaPowerBuffer);

    const uint16_t linRegData = ((1U << SGTL5000_VDDC_MAIN_ASSN_POS) | (1U << SGTL5000_VDDC_MAIN_OVRD_POS) |
                                 SGTL5000_D_PROGRAMMING_GET_VAL_FROM_MV(1000U));

    initSuccessful &= writeI2C(SGTL5000_CHIP_LINREG_CTRL_REG, linRegData);

    // CHIP_REF_CTRL
    // CHIP_LINE_OUT_CTRL
    // CHIP_SHORT_CTRL
    // CHIP_ANA_CTRL

    const uint16_t anaPower2Data =
        ((1U << SGTL5000_STARTUP_POWERUP_POS),
         ((1U << SGTL5000_ANALOG_VOLTAGE_POWERUP_POS) | (1U << SGTL5000_ADC_MONO_POWERUP_POS) |
          (1U << SGTL5000_REFTOP_POWERUP_POS) | (1U << SGTL5000_HEADPHONE_POWERUP_POS) |
          (1U << SGTL5000_DAC_POWERUP_POS) | (1U << SGTL5000_CAPLESS_HEADPHONE_POWERUP_POS) |
          (1U << SGTL5000_ADC_POWERUP_POS) | (1U << SGTL5000_LINEOUT_POWERUP_POS)));

    initSuccessful &= writeI2C(SGTL5000_CHIP_ANA_POWER_REG, anaPower2Data);

    const uint16_t digPowerData =
        ((1U << SGTL5000_DIG_POWER_ADC_POWERUP_POS) | (1U << SGTL5000_DIG_POWER_DAC_POWERUP_POS) |
         (1U << SGTL5000_DIG_POWER_DAP_POWERUP_POS) | (1U << SGTL5000_DIG_POWER_I2S_OUT_POWERUP_POS) |
         (1U << SGTL5000_DIG_POWER_I2S_IN_POWERUP_POS));

    initSuccessful &= writeI2C(SGTL5000_DIG_POWER_REG, digPowerData);

    // 400ms wait?
    const uint16_t lineOutVolData = 0x1D1D;
    initSuccessful &= writeI2C(SGTL5000_CHIP_LINE_OUT_CTRL_REG, lineOutVolData);

    const uint16_t chipClkData =
        ((SGTL5000_RATE_MODE_SYS_FS << SGTL5000_CHIP_CLK_CTRL_RATE_MODE_POS) |  // rate mode
         (SGTL5000_DIG_POWER_SYS_FS_44_1_KHZ << SGTL5000_CHIP_CLK_CTRL_SYS_FS_POS) | (SGTL5000_MCLK_FREQ_256_FS));

    initSuccessful &= writeI2C(SGTL5000_CHIP_CLK_CTRL_REG, chipClkData);

    const uint16_t chipI2sConfigData = (SGTL5000_CHIP_I2S_CTRL_SCLKFREQ_64FS | SGTL5000_CHIP_I2S_CTRL_I2S_SLAVE |
                                        SGTL5000_CHIP_I2S_CTRL_SCLK_INV_RISING_EDGE |
                                        (SGTL5000_DATALENGTH_16_BITS << SGTL5000_CHIP_I2S_CTRL_DLEN_POS) |
                                        (SGTL5000_I2S_MODE_LEFT_JUSTIFIED << SGTL5000_CHIP_I2S_CTRL_I2S_MODE_POS));

    initSuccessful &= writeI2C(SGTL5000_CHIP_I2S_CTRL_REG, chipI2sConfigData);

    // SSS_CTRL
    // ADCDAC_CTRL
    // DAC_VOL
    // ANA_HP_CTRL
    // ANA_CTRL

    return initSuccessful;
}

bool SGTL5000::writeI2C(const uint8_t reg, const uint16_t txData)
{
    uint8_t txBuffer[] = {
        (uint8_t)((txData & 0xFF00) >> 8U),
        (uint8_t)(txData & 0x00FF),
    };

    bool ret = true;
    if (HAL_I2C_Master_Transmit(i2cDriver, SGTL5000_ADDRESS, txBuffer, sizeof(txBuffer), 1) != HAL_OK)
    {
        ret = false;
    }
    return ret;
}

bool SGTL5000::readI2C(const uint8_t reg, uint16_t *rxData)
{
    bool ret = true;
    uint8_t rxBuffer[2] = {0U};

    ret &= (HAL_I2C_Master_Receive(i2cDriver, SGTL5000_ADDRESS, rxBuffer, sizeof(rxBuffer), 1) == HAL_OK);

    if (ret)
    {
        *rxData = ((rxBuffer[0] << 8U) | (rxBuffer[1]));
    }

    return ret;
}

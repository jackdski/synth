#ifndef SGTL5000_H_
#define SGTL5000_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

#define SGTL5000_ADDRESS                             0b0001010U

#define SGTL5000_CHIP_ID_REG                         0x0000U
#define SGTL5000_PART_ID_BITMASK                     0xFF00U  // 0xA0
#define SGTL5000_REV_ID_BITMASK                      0x00FFU

#define SGTL5000_DIG_POWER_REG                       0x0002U
#define SGTL5000_DIG_POWER_ADC_POWERUP_POS           6U
#define SGTL5000_DIG_POWER_DAC_POWERUP_POS           5U
#define SGTL5000_DIG_POWER_DAP_POWERUP_POS           4U
#define SGTL5000_DIG_POWER_I2S_OUT_POWERUP_POS       1U
#define SGTL5000_DIG_POWER_I2S_IN_POWERUP_POS        0U

#define SGTL5000_CHIP_CLK_CTRL_REG                   0x0004U
#define SGTL5000_CHIP_CLK_CTRL_RATE_MODE_BITMASK     (0x03U << 4U)
#define SGTL5000_CHIP_CLK_CTRL_RATE_MODE_POS         4U
#define SGTL5000_CHIP_CLK_CTRL_SYS_FS_POS            2U
#define SGTL5000_CHIP_CLK_CTRL_MCLK_FREQ_BITMASK     0x03U

#define SGTL5000_CHIP_I2S_CTRL_REG                   0x0006
#define SGTL5000_CHIP_I2S_CTRL_SCLKFREQ_POS          8U
#define SGTL5000_CHIP_I2S_CTRL_SCLKFREQ_64FS         (0U << SGTL5000_CHIP_I2S_CTRL_SCLKFREQ_POS)
#define SGTL5000_CHIP_I2S_CTRL_SCLKFREQ_32FS         (1U << SGTL5000_CHIP_I2S_CTRL_SCLKFREQ_POS)
#define SGTL5000_CHIP_I2S_CTRL_MS_POS                7U
#define SGTL5000_CHIP_I2S_CTRL_I2S_SLAVE             (0U << SGTL5000_CHIP_I2S_CTRL_MS_POS)
#define SGTL5000_CHIP_I2S_CTRL_I2S_MASTER            (1U << SGTL5000_CHIP_I2S_CTRL_MS_POS)
#define SGTL5000_CHIP_I2S_CTRL_SCLK_INV_POS          6U
#define SGTL5000_CHIP_I2S_CTRL_SCLK_INV_RISING_EDGE  (0U << SGTL5000_CHIP_I2S_CTRL_SCLK_INV_POS)
#define SGTL5000_CHIP_I2S_CTRL_SCLK_INV_FALLING_EDGE (1U << SGTL5000_CHIP_I2S_CTRL_SCLK_INV_POS)
#define SGTL5000_CHIP_I2S_CTRL_DLEN_POS              4U
#define SGTL5000_CHIP_I2S_CTRL_DLEN_BITMASK          (0x3 << SGTL5000_CHIP_I2S_CTRL_DLEN_POS)

typedef enum
{
    SGTL5000_DATALENGTH_32_BITS,
    SGTL5000_DATALENGTH_24_BITS,
    SGTL5000_DATALENGTH_20_BITS,
    SGTL5000_DATALENGTH_16_BITS,
} SGTL5000_i2sDataLength_E;

#define SGTL5000_CHIP_I2S_CTRL_I2S_MODE_POS     2U
#define SGTL5000_CHIP_I2S_CTRL_I2S_MODE_BITMASK (0x3 << SGTL5000_CHIP_I2S_CTRL_I2S_MODE_POS)

typedef enum
{
    SGTL5000_I2S_MODE_LEFT_JUSTIFIED,
    SGTL5000_I2S_MODE_RIGHT_JUSTIFIED,
    SGTL5000_I2S_MODE_PCM_FORMAT_A_B,
} SGTL5000_i2sMode_E;

#define SGTL5000_CHIP_I2S_CTRL_LRALIGN_POS           1U
#define SGTL5000_CHIP_I2S_CTRL_LRPOL_POS             0U

#define SGTL5000_CHIP_ADC_DAC_CTRL_REG               0x000EU
#define SGTL5000_ADC_DAC_CTRL_VOL_BUSY_DAC_RIGHT_POS 13U
#define SGTL5000_ADC_DAC_CTRL_VOL_BUSY_DAC_LEFT_POS  12U
#define SGTL5000_ADC_DAC_CTRL_VOL_RAMP_EN_POS        9U
#define SGTL5000_ADC_DAC_CTRL_VOL_EXPO_RAMP_POS      8U
#define SGTL5000_ADC_DAC_CTRL_DAC_MUTE_RIGHT_POS     3U
#define SGTL5000_ADC_DAC_CTRL_DAC_MUTE_LEFT_POS      2U
#define SGTL5000_ADC_DAC_CTRL_DAC_ADC_HPF_FREEZE_POS 1U
#define SGTL5000_ADC_DAC_CTRL_DAC_ADC_HPF_BYPASS_POS 0U

#define SGTL5000_CHIP_DAC_VOL_REG                    0x0010U
#define SGTL5000_DAC_VOL_RIGHT_BITMASK               (0xFFU << 8)
#define SGTL5000_DAC_VOL_LEFT_BITMASK                0xFFY
#define SGTL5000_DAC_VOL_NO_VOLUME                   0x3CU    // 0dB
#define SGTL5000_DAC_VOL_MAX_VOLUME                  0xF0U    // -90dB
#define SGTL5000_DAC_VOL_MUTED_VOLUME                0xFCU    // 0xFC and greater = Muted
#define SGTL5000_DAC_VOL_DB_STEP                     0.5017F  // dB / step from 0 (0xF0) to -90dB (0x3C)

#define SGTL5000_CHIP_ANA_HP_CTRL_REG                0x0022U
#define SGTL5000_HP_VOL_RIGHT_BITMASK                (0x7FU << 8U)
#define SGTL5000_HP_VOL_LEFT_BITMASK                 0x7FU
#define SGTL5000_HEADPHONE_VOLUME_MIN                0x00U  // +12dB
#define SGTL5000_HEADPHONE_VOLUME_MAX                0x7FU  // -51.5dB
#define SGTL5000_HEADPHONE_VOLUME_DB_PER_STEP        0.5F   // 0.5 dB / step

#define SGTL5000_CHIP_ANA_CTRL_REG                   0x0024U
#define SGTL5000_MUTE_LINEOUT_POS                    8U
#define SGTL5000_SELECT_HEADPHONE_INPUT_POS          6U
#define SGTL5000_EN_ZCD_HEADPHONE_POS                5U
#define SGTL5000_MUTE_HEADPHONE_OUTPUT_POS           4U
#define SGTL5000_SELECT_ADC_INPUT_POS                2U
#define SGTL5000_EN_ZCD_ADC_POS                      1U
#define SGTL5000_MUTE_ADC_POS                        0U

#define SGTL5000_CHIP_LINREG_CTRL_REG                0x0026U
#define SGTL5000_VDDC_MAIN_ASSN_POS                  6U  // 0 = VDDA, 1 = VDDIO
#define SGTL5000_VDDC_MAIN_OVRD_POS                  5U  // 0 = charge pump auto assigned, 1 = charge pump manually assigned

// 0x0 = 1.6V, 0xF = 0.85V, 50mV steps. Must clear LINREG_SIMPLE_POWERUP and STARTUP_POWERUP bits in 0x0030 register
// after power up, for this setting to produce the proper VDDD voltage
#define SGTL5000_D_PROGRAMMING_MAX_VALUE             0xFU
#define SGTL5000_D_PROGRAMMING_MAX_VOLTAGE_MV        1600U
#define SGTL5000_D_PROGRAMMING_MIN_VOLTAGE_MV        850U
#define SGTL5000_D_PROGRAMMING_MILLIVOLTS_PER_STEP   50U
#define SGTL5000_D_PROGRAMMING_GET_VAL_FROM_MV(MV)                                                                     \
    ((SGTL5000_D_PROGRAMMING_MAX_VOLTAGE_MV - MV) / SGTL5000_D_PROGRAMMING_MILLIVOLTS_PER_STEP)

#define SGTL5000_CHIP_LINE_OUT_CTRL_REG               0x002CU
#define SGTL5000_LINE_OUT_CURRENT_BITMASK             (0x0FU << 8U)
#define SGTL5000_LINE_OUT_ANALOG_VOLTAGE_CTRL_BITMASK 0x3FU   // usually set to VDDIO/2
#define SGTL5000_LO_ANALOG_VOLTAGE_MIN                0x00    // 0.800V
#define SGTL5000_LO_ANALOG_VOLTAGE_MAX                0x23    // 1.675V
#define SGTL5000_LO_ANALOG_VOLTAGE_PER_STEP           0.025F  // 25mV per step

// reference table 32 in the datasheet to set this register
#define SGTL5000_LINE_OUT_VOL_REG                     0x002EU
#define SGTL5000_LINEOUT_VOL_RIGHT_BITMASK            (0x0FU << 8U)
#define SGTL5000_LINEOUT_VOL_LEFT_BITMASK             0x0FU

#define SGTL5000_CHIP_ANA_POWER_REG                   0x0030U
#define SGTL5000_DAC_MONO_POWER_POS                   14U  // 0 = mono (left only), 1 - Stereo
#define SGTL5000_LINREG_SIMPLE_POWERUP_POS            13U
#define SGTL5000_STARTUP_POWERUP_POS                  12U
#define SGTL5000_VDDC_CHRGPMP_POWERUP_POS             11U
#define SGTL5000_PLL_POWERUP_POS                      10U
#define SGTL5000_LINREG_D_POWERUP_POS                 9U
#define SGTL5000_VCOAMP_POWERUP_POS                   8U
#define SGTL5000_ANALOG_VOLTAGE_POWERUP_POS           7U
#define SGTL5000_ADC_MONO_POWERUP_POS                 6U  // 0 = mono (left only), 1 - Stereo
#define SGTL5000_REFTOP_POWERUP_POS                   5U
#define SGTL5000_HEADPHONE_POWERUP_POS                4U
#define SGTL5000_DAC_POWERUP_POS                      3U
#define SGTL5000_CAPLESS_HEADPHONE_POWERUP_POS        2U
#define SGTL5000_ADC_POWERUP_POS                      1U
#define SGTL5000_LINEOUT_POWERUP_POS                  0U

#define SGTL5000_CHIP_PLL_CTRL_REG                    0x0032U

// INT_DIVISOR = FLOOR(PLL_OUTPUT_FREQ / INPUT_FREQ)
// PLL_OUTPUT_REQ = 180.6336 MHZ if System sample rate = 44.1 kHz else 196.608 MHz
// INPUT_FREQ = Frequency of the external MCLK provided if CHIP_CLK_TOP_CTRL->INPUT_FREQ_DIV2 = 0x0
//  else INPUT_FREQ = (Frequency of the external MCLK provided / 2) if CHIP_CLK_TOP_CTRL->INPUT_FREQ_DIV2 = 0x1
#define SGTL5000_INT_DIVISOR_BITMASK                  (0xF8U << 8U)

// FRAC_DIVISOR = ((PLL_OUTPUT_FREQ / INPUT_FREQ) - INT_DIVISOR) * 2048
// PLL_OUTPUT_REQ = 180.6336 MHZ if System sample rate = 44.1 kHz else 196.608 MHz
// INPUT_FREQ = Frequency of the external MCLK provided if CHIP_CLK_TOP_CTRL->INPUT_FREQ_DIV2 = 0x0
//  else INPUT_FREQ = (Frequency of the external MCLK provided / 2) if CHIP_CLK_TOP_CTRL->INPUT_FREQ_DIV2 = 0x1

#define SGTL5000_FRAC_DIVISOR_BITMASK                 0x7FFU

#define SGTL5000_CHIP_CLK_TOP_CTRL_REG                0x0034U
#define SGTL5000_ENABLE_INTERNAL_OSC_POS              11U
#define SGTL5000_INPUT_FREQ_DIV2_POS                  3U  // 0 = pass through, 1 = SYS_MCLK is divided by 2 before entering PLL

#define SGTL5000_CHIP_ANA_STATUS_REG                  0x0036U
#define SGTL5000_LRSHORT_STS_POS                      9U
#define SGTL5000_CSHORT_STS_POS                       8U
#define SGTL5000_PLL_IS_LOCKED_POS                    4U

#define SGTL5000_DAP_CONTROL_REG                      0x0100
#define SGTL5000_DAP_MIXER_EN_POS                     4U
#define SGTL5000_DAP_EN_POS                           0U

#define SGTL5000_PEQ_REG                              0x0102U

#define SGTL5000_DAP_BASS_ENHANCE_REG                 0x0104U
#define SGTL5000_BASS_BYPASS_HPF_POS                  8U
#define SGTL5000_BASS_CUTOFF_FREQ_BITMASK             0x70U
#define SGTL5000_BASS_ENHANCE_EN_POS                  0U

typedef enum
{
    SGTL5000_RATE_MODE_SYS_FS,
    SGTL5000_RATE_MODE_HALF_SYS_FS,
    SGTL5000_RATE_MODE_QUARTER_SYS_FS,
    SGTL5000_RATE_MODE_SIXTH_SYS_FS,
} SGTL5000_rateMode_E;

typedef enum
{
    SGTL5000_DIG_POWER_SYS_FS_32_KHZ,
    SGTL5000_DIG_POWER_SYS_FS_44_1_KHZ,
    SGTL5000_DIG_POWER_SYS_FS_48_KHZ,
    SGTL5000_DIG_POWER_SYS_FS_96_KHZ,
} SGTL5000_sysFs_E;

typedef enum
{
    SGTL5000_MCLK_FREQ_256_FS,
    SGTL5000_MCLK_FREQ_384_FS,
    SGTL5000_MCLK_FREQ_512_FS,
    SGTL5000_MCLK_FREQ_PLL,
} SGTL5000_mclkFreq_E;

/*
    Controls te output bias currents to the LINEOUT amplifiers. Nominal recommended setting
    for a 10k Ohm load with 1.0nF load cap is 0x3.
*/
typedef enum
{
    SGTL5000_LINE_OUT_CURRENT_0_18_MA = 0U,
    SGTL5000_LINE_OUT_CURRENT_0_27_MA = 1U,
    SGTL5000_LINE_OUT_CURRENT_0_36_MA = 3U,
    SGTL5000_LINE_OUT_CURRENT_0_45_MA = 7U,
    SGTL5000_LINE_OUT_CURRENT_0_54_MA = 0xFU,
} SGTL5000_lineOutCurrent_E;

class SGTL5000
{
public:
    SGTL5000(I2C_HandleTypeDef *i2cDriver) : i2cDriver(i2cDriver)
    {
    }

    bool init(void);
    bool writeI2C(const uint8_t reg, const uint16_t txData);
    bool readI2C(const uint8_t reg, uint16_t *rxData);

private:
    I2C_HandleTypeDef *i2cDriver;

    SGTL5000_rateMode_E rateMode;
    SGTL5000_sysFs_E sysFs;
    SGTL5000_mclkFreq_E mclkFreq;
    SGTL5000_lineOutCurrent_E lineOutCurrent;

    bool dacRightVolBusy;
    bool dacLeftVolBusy;
    bool volumeRampEnabled;
    bool dacMuteRight;
    bool dacMuteLeft;
    // float32_t                   dacVolumeDb;        // 0dB to -90dB in 0.5017dB steps
    // float32_t                   headphoneVolumeDb;  // +12dB to -51.5dB in 0.5dB steps
    uint16_t chipIdRegister;
    uint16_t digPowerRegister;
};

#endif /* SGTL5000_H_ */

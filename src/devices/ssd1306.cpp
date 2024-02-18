#include "ssd1306.hpp"
#include "main.h"

using namespace Devices;

bool SSD1306::init(void)
{
    bool initSuccessful = true;
    //   Init sequence
    uint8_t init1[] = {SSD1306_CMD_DISPLAY_OFF,          // 0xAE
                       SSD1306_CMD_SET_DISPLAY_CLK_DIV,  // 0xD5
                       0x80,                             // the suggested ratio 0x80
                       SSD1306_CMD_SET_MUX_RATIO};       // 0xA8

    initSuccessful &= writeBytes(init1, sizeof(init1));

    initSuccessful &= writeCommand(SSD1306_HEIGHT - 1U);

    uint8_t init2[] = {SSD1306_CMD_SET_DISPLAY_OFFSET,      // 0xD3
                       0x00U,                               // no offset
                       SSD1306_CMD_SET_DISPLAY_START_LINE,  // line #0
                       SSD1306_CMD_SET_CHARGE_PUMP};        // 0x8D

    initSuccessful &= writeBytes(init2, sizeof(init2));

    writeCommand(SSD1306_EXTERNAL_VCC);

    uint8_t init3[] = {SSD1306_CMD_SET_MEMORY_ADDR_MODE,  // 0x20
                       0x00,                              // horz mode
                       SSD1306_CMD_SET_SEGMENT_REMAP,
                       SSD1306_CMD_SET_COM_SCAN_MODE_REMAP};

    writeBytes(init3, sizeof(init3));

    initSuccessful &= writeCommand(SSD1306_CMD_SET_COM_PIN_MAP);
    initSuccessful &= writeCommand(0x12);
    initSuccessful &= writeCommand(SSD1306_CMD_SET_CONTRAST);
    initSuccessful &= writeCommand(0xAF);
    initSuccessful &= writeCommand(SSD1306_CMD_SET_PRECHARGE);  // 0xd9
    initSuccessful &= writeCommand(0xF1);

    uint8_t init5[] = {SSD1306_CMD_SET_VCOMH_DESELCT,  // 0xDB
                       0x40,
                       SSD1306_CMD_DISPLAY_RAM,     // 0xA4
                       SSD1306_CMD_DISPLAY_NORMAL,  // 0xA6
                       SSD1306_DEACTIVATE_SCROLL,
                       SSD1306_CMD_DISPLAY_ON};  // Main screen turn on

    initSuccessful &= writeBytes(init5, sizeof(init5));

    initialized = initSuccessful;
    return initialized;
}

bool SSD1306::set_pixel(uint16_t x, uint16_t y, bool color)
{
    // uint16_t byte_index = x + ((y >> 3) * buf_w);
    // uint8_t bit_index   = y & 0x7;

    // if (color)
    // {
    //     BIT_SET(buf[byte_index], bit_index);
    // }
    // else
    // {
    //     BIT_CLEAR(buf[byte_index], bit_index);
    // }
    return true;
}

bool SSD1306::sleep(void)
{
    uint8_t conf[] = {
        SSD1306_CONTROL_BYTE_CMD_STREAM,
        SSD1306_CMD_DISPLAY_OFF,
    };
    return (HAL_I2C_Master_Transmit(i2c, SSD1306_I2C_ADDRESS, conf, sizeof(conf), 10U) == HAL_OK);
}

bool SSD1306::wakeUp(void)
{
    uint8_t conf[] = {
        SSD1306_CONTROL_BYTE_CMD_STREAM,
        SSD1306_CMD_DISPLAY_ON,
    };
    return (HAL_I2C_Master_Transmit(i2c, SSD1306_I2C_ADDRESS, conf, sizeof(conf), 10U) == HAL_OK);
}

bool SSD1306::writeCommand(uint8_t command)
{
    uint8_t privCommand[2U] = {0x00, command};
    return (HAL_I2C_Master_Transmit(i2c, SSD1306_I2C_ADDRESS, privCommand, 2U, 10U) == HAL_OK);
}

bool SSD1306::writeBytes(uint8_t *data, uint8_t size)
{
    return (HAL_I2C_Master_Transmit(i2c, SSD1306_I2C_ADDRESS, data, size, 10U) == HAL_OK);
}

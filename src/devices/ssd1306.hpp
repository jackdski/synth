#ifndef SSD1306_HPP_
#define SSD1306_HPP_

#include "main.h"
#include <stdint.h>

#define SSD1306_DISPLAY_ORIENTATION                  TFT_ORIENTATION_LANDSCAPE

// SLA (0x3C) + WRITE_MODE (0x00) =  0x78 (0b01111000)
#define SSD1306_I2C_ADDRESS                          0x78  // 0x3D  // might be 0x3D
#define SSD1306_WIDTH                                128
#define SSD1306_HEIGHT                               64
#define SSD1306_COLUMNS                              128
#define SSD1306_PAGES                                8
#define SSD1306_PIXEL_PER_PAGE                       8

// Control byte
#define SSD1306_CONTROL_BYTE_CMD_SINGLE              0x80
#define SSD1306_CONTROL_BYTE_CMD_STREAM              0x00
#define SSD1306_CONTROL_BYTE_DATA_STREAM             0x40

// Fundamental commands (pg.28)
#define SSD1306_CMD_SET_CONTRAST                     0x81  // follow with 0x7F
#define SSD1306_CMD_DISPLAY_RAM                      0xA4
#define SSD1306_CMD_DISPLAY_ALLON                    0xA5
#define SSD1306_CMD_DISPLAY_NORMAL                   0xA6
#define SSD1306_CMD_DISPLAY_INVERTED                 0xA7
#define SSD1306_CMD_DISPLAY_OFF                      0xAE
#define SSD1306_CMD_DISPLAY_ON                       0xAF

// Addressing Command Table (pg.30)
#define SSD1306_CMD_SET_MEMORY_ADDR_MODE             0x20  // follow with 0x00 = HORZ mode
#define SSD1306_CMD_SET_COLUMN_RANGE                 0x21  // can be used only in HORZ/VERT mode - follow with 0x00 and 0x7F = COL127
#define SSD1306_CMD_SET_PAGE_RANGE                   0x22  // can be used only in HORZ/VERT mode - follow with 0x00 and 0x07 = PAGE7

// Hardware Config (pg.31)
#define SSD1306_CMD_SET_DISPLAY_START_LINE           0x40
#define SSD1306_CMD_SET_SEGMENT_REMAP                0xA1
#define SSD1306_CMD_SET_MUX_RATIO                    0xA8  // follow with 0x3F = 64 MUX
#define SSD1306_CMD_SET_COM_SCAN_MODE_NORMAL         0xC0
#define SSD1306_CMD_SET_COM_SCAN_MODE_REMAP          0xC8
#define SSD1306_CMD_SET_DISPLAY_OFFSET               0xD3  // follow with 0x00
#define SSD1306_CMD_SET_COM_PIN_MAP                  0xDA  // follow with 0x12
#define SSD1306_CMD_NOP                              0xE3  // NOP

// Timing and Driving Scheme (pg.32)
#define SSD1306_CMD_SET_DISPLAY_CLK_DIV              0xD5  // follow with 0x80
#define SSD1306_CMD_SET_PRECHARGE                    0xD9  // follow with 0xF1
#define SSD1306_CMD_SET_VCOMH_DESELCT                0xDB  // follow with 0x30

// Charge Pump (pg.62)
#define SSD1306_CMD_SET_CHARGE_PUMP                  0x8D  // follow with 0x14

#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26  ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27  ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29  ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A  ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL                    0x2E  ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F  ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3  ///< Set scroll range

#define SSD1306_EXTERNAL_VCC                         0x10  ///< External display voltage source
#define SSD1306_SWITCHCAP_VCC                        0x14  ///< Gen. display voltage from 3.3V

namespace Devices
{

class SSD1306
{
public:
    SSD1306(I2C_HandleTypeDef *i2c) : i2c(i2c)
    {
    }

    I2C_HandleTypeDef *i2c;

    bool init(void);

    bool sleep(void);
    bool wakeUp(void);
    bool set_pixel(uint16_t x, uint16_t y, bool color);

    bool writeCommand(uint8_t command);
    bool writeBytes(uint8_t *data, uint8_t size);

private:
    bool initialized = false;
};

}

#endif  // SSD1306_HPP_

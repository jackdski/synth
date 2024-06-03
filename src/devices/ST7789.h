#ifndef ST7789_H_
#define ST7789_H_

/* I N C L U D E S */

#include "drv_GPIO.h"
#include "drv_SPI.h"
#include "features.h"

#if (FEATURE_ST7789)

/* D E F I N E S */

#define ST7789_LCD_WIDTH        240U
#define ST7789_LCD_HEIGHT       135U

#define ST7789_REG_NOP                    0x00
#define ST7789_REG_SWRESET                0x01
#define ST7789_REG_READ_DIS_ID            0x04  // RDDID
#define ST7789_REG_READ_DIS_STATUS        0x09  // RDDST
#define ST7789_REG_READ_DIS_PWR_MODE      0x0A  // RDDPM
#define ST7789_REG_READ_DIS_MADCTL        0x0B  // RDDMADCTL
#define ST7789_REG_READ_DIS_COLMOD        0x0C  // RDDCOLMOD
#define ST7789_REG_READ_DIS_IMG_MODE      0x0D  // RDDIM
#define ST7789_REG_READ_DIS_SIG_MODE      0x0E  // RDDSM
#define ST7789_REG_SLEEP_IN               0x10  // SLPIN
#define ST7789_REG_SLEEP_OUT              0x11  // SLPOUT
#define ST7789_REG_PARTIAL_DIS_MODE_ON    0x12  // PTLON
#define ST7789_REG_NORMAL_DIS_MODE_ON     0x13  // NORON
#define ST7789_REG_INVERT_OFF             0x20  // INVOFFF
#define ST7789_REG_INVERT_ON              0x21  // INVON
#define ST7789_REG_DISPLAY_OFF            0x28  // DISOFF
#define ST7789_REG_DISPLAY_ON             0x29  // DISON
#define ST7789_REG_COLUMN_ADDR_SET        0x2A  // CASET
#define ST7789_REG_ROW_ADDR_SET           0x2B  // RASET
#define ST7789_REG_MEM_WRITE              0x2C  // RAMWR
#define ST7789_REG_MEM_READ               0x2D  // RAMRD
#define ST7789_REG_PARTIAL_AREA           0x30  // PTLAR
#define ST7789_REG_VERT_SCROLL_DEF        0x33  // VSCRDEF
#define ST7789_REG_TEAR_EFFECT_LINE_OFF   0x34  // TEOFF
#define ST7789_REG_TEAR_EFFECT_LINE_ON    0x35  // TEON
#define ST7789_REG_MEM_DATA_ACCESS_CTL    0x36  // MADCTL
#define ST7789_REG_VERT_SCROLL_START_ADDR 0x37  // VSCSAD
#define ST7789_REG_IDLE_MODE_OFF          0x38  // IDMOFF
#define ST7789_REG_IDLE_MODE_ON           0x39  // IDMON
#define ST7789_REG_COLMOD                 0x3A  // interface pixel format
#define ST7789_REG_WRITE_MEM_CONTINUE     0x3C
#define ST7789_REG_READ_MEM_CONTINUE      0x3E
#define ST7789_REG_WRITE_DIS_BRIGHTNESS   0x51  // WRDISBV
#define ST7789_REG_READ_DIS_BRIGHTNESS    0x52  // RDDISBV
#define ST7789_REG_WRITE_CTRL_DISPLAY     0x53  // WRCTRLD
#define ST7789_REG_READ_CTRL_DISPLAY      0x54  // RDCTRLD

#define ST7789_REG_VDV_SET                0xC4

#define ST7789_REG_READ_ID_1              0xDA  // RDID1
#define ST7789_REG_READ_ID_2              0xDB  // RDID2
#define ST7789_REG_READ_ID_3              0xDC  // RDID3
#define ST7789_REG_READ_CTRL_DISPLAY      0x54  // RDCTRLD

/* T Y P E D E F S */

typedef enum
{
    ST7789_STATE_INITIALIZE,
    ST7789_STATE_CONFIGURE,
    ST7789_STATE_ACTIVE,
    ST7789_STATE_OFF,

    ST7789_STATE_COUNT
} ST7789_state_E;

typedef struct
{
    drv_GPIO_channel_E csPin;
    drv_GPIO_channel_E rstPin;
    drv_GPIO_channel_E dcPin;

    drv_SPI_device_E spiDevice;

    uint32_t heightSize;  // 135
    uint32_t widthSize;   // 240
} ST7789_config_S;

/* P U B L I C   F U N C T I O N S */

void ST7789_init(void);
bool ST7789_isInitialized(void);

void ST7789_drawBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t* data, size_t size);

#endif  // FEATURE_ST7789
#endif  // ST7789_H_

#include "ST7789.h"

/* I N C L U D E S */

#include "features.h"

#if (FEATURE_ST7789)

/* D E F I N E S */

// #define MISC_COMMAND_BUFFER_SIZE 20U

/* T Y P E D E F S */

typedef struct
{
    bool initialized;
    // uint8_t miscCommandMailboxPosition;
} ST7789_data_S;

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

// static uint8_t ST7789_private_getNextCommandBufferPosition(void);

static void ST7789_private_setCommandPin(void);
static void ST7789_private_clearCommandPin(void);
static bool ST7789_private_writeCommand(uint8_t command);
static bool ST7789_private_writeByte(uint8_t byte);
static bool ST7789_private_writeData(uint8_t * data, uint32_t length);
static void ST7789_private_hardwareReset(void);
static void ST7789_private_softwareReset(void);
static void ST7789_private_sleepOut(void);
static void ST7789_private_colMod(void);
static void ST7789_private_memoryDataAccessControl(void);
static void ST7789_private_columnAddressSet(void);
static void ST7789_private_rowAddressSet(void);
static void ST7789_private_setAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
static void ST7789_writeToRam(uint8_t* data, size_t size);

/* D A T A   D E F I N I T I O N S */

static ST7789_data_S ST7789_data; // = {
//     .miscCommandMailboxPosition = 0U,
// };

// static uint8_t drv_SPI_displayCommandTxBuffer[MISC_COMMAND_BUFFER_SIZE];
// static uint8_t drv_SPI_displayCommandRxBuffer[MISC_COMMAND_BUFFER_SIZE];

/* P R I V A T E   F U N C T I O N S */

// static uint8_t ST7789_private_getNextCommandBufferPosition(void)
// {
//     if (ST7789_data.miscCommandMailboxPosition < MISC_COMMAND_BUFFER_SIZE)
//     {
//         ST7789_data.miscCommandMailboxPosition++;
//     }
//     return ST7789_data.miscCommandMailboxPosition;
// }

static void ST7789_private_setCommandPin(void)
{
    drv_GPIO_setOutput(DRV_GPIO_CHANNEL_LCD_DC, DRV_GPIO_LOW);
}

static void ST7789_private_clearCommandPin(void)
{
    drv_GPIO_setOutput(DRV_GPIO_CHANNEL_LCD_DC, DRV_GPIO_HIGH);
}

static bool ST7789_private_writeCommand(uint8_t command)
{
    return drv_SPI_write(DRV_SPI_DEVICE_ST7789_DISPLAY, &command, 1U, ST7789_private_setCommandPin, ST7789_private_clearCommandPin);
}

static bool ST7789_private_writeByte(uint8_t byte)
{
    return drv_SPI_write(DRV_SPI_DEVICE_ST7789_DISPLAY, &byte, 1U, ST7789_private_clearCommandPin, ST7789_private_clearCommandPin);
}

static bool ST7789_private_writeData(uint8_t * data, uint32_t length)
{
    return drv_SPI_write(DRV_SPI_DEVICE_ST7789_DISPLAY, data, length, ST7789_private_clearCommandPin, ST7789_private_clearCommandPin);
}

static void ST7789_private_hardwareReset(void)
{
    drv_GPIO_setOutput(DRV_GPIO_CHANNEL_LCD_RST, DRV_GPIO_LOW);
    vTaskDelay(pdMS_TO_TICKS(1));
    drv_GPIO_setOutput(DRV_GPIO_CHANNEL_LCD_RST, DRV_GPIO_HIGH);

    // If hardware reset started while sleep out, reset time may be up to 120ms
    // Unconditionally wait as hardware reset doesn't need to be performant
    //   sleepIn = true;
    //   lastSleepExit = xTaskGetTickCount();
    vTaskDelay(pdMS_TO_TICKS(125U));
}

static void ST7789_private_softwareReset(void)
{
    ST7789_private_writeCommand(ST7789_REG_SWRESET);
    vTaskDelay(pdMS_TO_TICKS(150U));
}

static void ST7789_private_sleepOut(void)
{
    ST7789_private_writeCommand(ST7789_REG_SLEEP_OUT);
    vTaskDelay(pdMS_TO_TICKS(10U));
}

static void ST7789_private_colMod(void)
{
    ST7789_private_writeCommand(ST7789_REG_COLMOD);
    ST7789_private_writeByte(0x55);
    vTaskDelay(pdMS_TO_TICKS(10U));
}

static void ST7789_private_memoryDataAccessControl(void)
{
    ST7789_private_writeCommand(ST7789_REG_MEM_DATA_ACCESS_CTL);
    ST7789_private_writeByte(0x08U); // bottom-top refresh
}

static void ST7789_private_columnAddressSet(void)
{
  ST7789_private_writeCommand(ST7789_REG_COLUMN_ADDR_SET);
  uint8_t data[] = {0x00, 0x00, ST7789_LCD_WIDTH >> 8U, ST7789_LCD_WIDTH & 0xFFU};
  ST7789_private_writeData(data, 4U);
//   ST7789_private_writeByte(0x00);
//   ST7789_private_writeByte(0x00);
//   ST7789_private_writeByte(ST7789_LCD_WIDTH >> 8U);
//   ST7789_private_writeByte(ST7789_LCD_WIDTH & 0xFFU);
}

static void ST7789_private_rowAddressSet(void)
{
    ST7789_private_writeCommand(ST7789_REG_ROW_ADDR_SET);
    uint8_t data[] = {0x00, 0x00, ST7789_LCD_HEIGHT >> 8U, ST7789_LCD_HEIGHT & 0xFFU};
    ST7789_private_writeData(data, 4U);
    // ST7789_private_writeByte(0x00);
    // ST7789_private_writeByte(0x00);
    // ST7789_private_writeByte(ST7789_LCD_HEIGHT >> 8U);
    // ST7789_private_writeByte(ST7789_LCD_HEIGHT & 0xFFU);
}

// static void ST7789_private_setVdv(void)
// {
//   ST7789_private_writeCommand(ST7789_REG_VDV_SET);
//   ST7789_private_writeByte(0x10);
// }

static void ST7789_private_setAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    {
        ST7789_private_writeCommand(ST7789_REG_COLUMN_ADDR_SET);
        uint8_t data[] = {x0 >> 8U, x0 & 0xFFU, x1 >> 8U, x1 & 0xFF};
        ST7789_private_writeData(data, 4U);
        // ST7789_private_writeByte(x0 >> 8);
        // ST7789_private_writeByte(x0 & 0xff);
        // ST7789_private_writeByte(x1 >> 8);
        // ST7789_private_writeByte(x1 & 0xff);
    }

    {
        ST7789_private_writeCommand(ST7789_REG_ROW_ADDR_SET);
        uint8_t data[] = {y0 >> 8U, y0 & 0xFFU, y1 >> 8U, y1 & 0xFF};
        ST7789_private_writeData(data, 4U);
        // ST7789_private_writeByte(y0 >> 8);
        // ST7789_private_writeByte(y0 & 0xff);
        // ST7789_private_writeByte(y1 >> 8);
        // ST7789_private_writeByte(y1 & 0xff);
    }
    ST7789_private_writeCommand(ST7789_REG_MEM_WRITE);
}

static void ST7789_writeToRam(uint8_t* data, size_t size)
{
    ST7789_private_writeCommand(ST7789_REG_MEM_WRITE);
    ST7789_private_writeData(data, size);
}

/* P U B L I C   F U N C T I O N S */

void ST7789_init(void)
{
    // clang-format off
    // drv_SPI_registerMailbox(DRV_SPI_DEVICE_ST7789_DISPLAY, DRV_SPI_MAILBOX_DISPLAY_COMMAND, drv_SPI_displayCommandTxBuffer, drv_SPI_displayCommandTxBuffer);
    // drv_SPI_registerMailbox(DRV_SPI_DEVICE_ST7789_DISPLAY, DRV_SPI_MAILBOX_DISPLAY_DRAW, drv_SPI_displayCommandTxBuffer, drv_SPI_displayCommandTxBuffer);
    // clang-format on

    ST7789_private_writeCommand(ST7789_REG_SWRESET); // send a byte so that the CLK line goes low - stm32 bug?

    ST7789_private_hardwareReset();
    ST7789_private_softwareReset();
    ST7789_private_sleepOut();
    ST7789_private_colMod();
    ST7789_private_memoryDataAccessControl();

    ST7789_private_columnAddressSet();
    ST7789_private_rowAddressSet();
    ST7789_private_writeCommand(ST7789_REG_INVERT_ON);
    vTaskDelay(pdMS_TO_TICKS(10U));
    ST7789_private_writeCommand(ST7789_REG_NORMAL_DIS_MODE_ON);
    vTaskDelay(pdMS_TO_TICKS(10U));
    // ST7789_private_setVdv();
    ST7789_private_writeCommand(ST7789_REG_DISPLAY_ON);
    vTaskDelay(pdMS_TO_TICKS(10U));
    ST7789_data.initialized = true;
}

bool ST7789_isInitialized(void)
{
    return ST7789_data.initialized;
}

void ST7789_drawBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t* data, size_t size)
{
  ST7789_private_setAddressWindow(x, y, x + width - 1, y + height - 1);
  ST7789_writeToRam(data, size);
}

#endif  // FEATURE_ST7789

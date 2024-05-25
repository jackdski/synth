#include "ST7789.h"

/* I N C L U D E S */

#include "features.h"

#if (FEATURE_ST7789)

/* D E F I N E S */

#define MISC_COMMAND_BUFFER_SIZE 20U

/* T Y P E D E F S */

typedef struct
{
    uint8_t miscCommandMailboxPosition;
} ST7789_data_S;

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* D A T A   D E F I N I T I O N S */

static ST7789_data_S ST7789_data = {
    .miscCommandMailboxPosition = 0U,
};

static uint8_t drv_SPI_displayCommandTxBuffer[MISC_COMMAND_BUFFER_SIZE];
static uint8_t drv_SPI_displayCommandRxBuffer[MISC_COMMAND_BUFFER_SIZE];

/* P R I V A T E   F U N C T I O N S */

static uint8_t ST7789_private_getNextCommandBufferPosition(void)
{
    if ()
        return ST7789_data.miscCommandMailboxPosition;
}

/* P U B L I C   F U N C T I O N S */

void ST7789_init(void)
{
    // clang-format off
    drv_SPI_registerMailbox(DRV_SPI_DEVICE_ST7789_DISPLAY, DRV_SPI_MAILBOX_DISPLAY_COMMAND, drv_SPI_displayCommandTxBuffer, drv_SPI_displayCommandTxBuffer);
    drv_SPI_registerMailbox(DRV_SPI_DEVICE_ST7789_DISPLAY, DRV_SPI_MAILBOX_DISPLAY_DRAW, drv_SPI_displayCommandTxBuffer, drv_SPI_displayCommandTxBuffer);
    // clang-format on

    hardwareReset();
    softwareReset();
    sleepOut();
    colMod();
    memdataaccesscon();
}

#endif  // FEATURE_ST7789

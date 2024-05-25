#include "drv_SPI.h"

/* I N C L U D E S */

#include "features.h"

#if (FEATURE_SPI)

/* D E F I N E S */


/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */


/* D A T A   D E F I N I T I O N S */


/* P R I V A T E   F U N C T I O N S */


/* P U B L I C   F U N C T I O N S */

void drv_SPI_init(void)
{
    drv_SPI_hardwareSpecific_init();
}

#endif // FEATURE_SPI
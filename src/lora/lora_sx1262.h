
#ifndef __LORA_SX1262_H
#define __LORA_SX1262_H

#include "app_cfg.h"
#include <SPI.h>

class lora_sx1262 {
public:
    bool rx_isAvailable = false;

    bool init(SPIClass &spi);

    /* return number of bytes received */
    int getRxAvailable(void);

    /* return number of bytes received */
    bool getRx(uint8_t *data, uint16_t data_size);

    bool send(uint8_t *data, uint16_t data_size);

    int getRSSI(void);
    int getSNR(void);

private:

};

extern lora_sx1262 g_lora_sx1262;

#endif // __LORA_SX1262_H
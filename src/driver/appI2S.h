
#ifndef __APPI2S_H
#define __APPI2S_H

#include "app_cfg.h"

void appI2S_init(void);

void appI2S_deinit(void);

bool appI2S_start(void);

void appI2S_stop(void);

void appI2S_clear_buff(void);

int appI2S_read_data(void *data, uint16_t read_len);

#endif // __APPI2S_H
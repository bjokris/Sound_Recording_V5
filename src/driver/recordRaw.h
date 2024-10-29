
#ifndef __RECORDRAW_H
#define __RECORDRAW_H

#include "app_cfg.h"
#include "audio_cfg.h"

void recordRaw_toFlash_start(const char *filePath, int record_size);

void recordRaw_toRam_start(uint8_t *mic_buf, int record_size);

bool recordRaw_writeFile(const char *filePath, uint8_t *mic_buf, int size);

int recordRaw_readFile(const char *filePath, uint8_t *buf, int size);

#endif // __RECORDRAW_H
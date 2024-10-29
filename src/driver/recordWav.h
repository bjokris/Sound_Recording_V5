
#ifndef __RECORDWAV_H
#define __RECORDWAV_H

#include "app_cfg.h"
#include "audio_cfg.h"

int recordWav_generateWAV_header(uint8_t *header, int wav_size);

void recordWav_start(void);

#endif // __RECORDWAV_H

#ifndef __SEEK1_H
#define __SEEK1_H

#include "app_cfg.h"
#include "seekTable.h"

/* implement seek1 function to analysic array data */
void seek1_do_calc(seek_data_t *seek_data, uint8_t *array, uint32_t size);

/* check warning/alarm */
void seek1_checkWarningAlarm(seek_data_t *seek_data);

/* send data via lora */
bool seek1_sendToLora(seek_data_t *seek_data);

#endif // __SEEK1_H
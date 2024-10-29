
#ifndef __SEEK2_H
#define __SEEK2_H

#include "app_cfg.h"
#include "seekTable.h"

/* implement seek2 function to analysic array data */
void seek2_do_calc(seek_data_t *seek_data, uint8_t *array, uint32_t size);

/* check warning/alarm */
void seek2_checkWarningAlarm(seek_data_t *seek_data);

/* send data via lora */
bool seek2_sendToLora(seek_data_t *seek_data);

#endif // __SEEK2_H
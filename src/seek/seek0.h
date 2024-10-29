
#ifndef __SEEK0_H
#define __SEEK0_H

#include "app_cfg.h"
#include "seekTable.h"

/* implement seek0 function to analysic array data */
void seek0_do_calc(seek_data_t *seek_data, uint8_t *array, uint32_t size);

/* check warning/alarm */
void seek0_checkWarningAlarm(seek_data_t *seek_data);

/* send data via lora */
bool seek0_sendToLora(seek_data_t *seek_data);

#endif // __SEEK0_H
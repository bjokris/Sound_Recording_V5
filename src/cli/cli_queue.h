/*
 * cli_queue.h
 *
 *  Created on: Jun 29, 2020
 *      Author: thaob
 */

#ifndef CLI_COMMAND_CLI_QUEUE_H_
#define CLI_COMMAND_CLI_QUEUE_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "cli_config.h"

#if (CONFIG_CLI_ENABLED == 1)

typedef struct
{
	char		buf[CLI_QUEUE_SIZE + 1];
	volatile uint16_t	length;
	volatile uint16_t	read_pos;
	volatile uint16_t	write_pos;
}cli_queue_t;


void cli_queue_free(cli_queue_t *queueObj);

uint16_t cli_queue_get_size(cli_queue_t *queueObj);

uint8_t cli_queue_Push(cli_queue_t *queueObj, uint8_t value);

uint8_t cli_queue_Pop(cli_queue_t *queueObj, uint8_t *value);

bool cli_queue_IsFull(cli_queue_t *queueObj);

bool cli_queue_IsEmpty(cli_queue_t *queueObj);

#endif // CONFIG_CLI_ENABLED 

#endif /* CLI_COMMAND_CLI_QUEUE_H_ */

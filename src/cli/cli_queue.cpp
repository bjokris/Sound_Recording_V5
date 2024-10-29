/*
 * cli_queue.c
 *
 *  Created on: Jun 29, 2020
 *      Author: thaob
 */
#include "cli_queue.h"

#if (CONFIG_CLI_ENABLED == 1)


void cli_queue_free(cli_queue_t *queueObj)
{
	for (uint16_t i = 0; i < CLI_QUEUE_SIZE; i++)
	{
		queueObj->buf[i] = 0;
	}
	queueObj->length = 0;
	queueObj->read_pos = 0;
	queueObj->write_pos = 0;
}


uint16_t cli_queue_length(cli_queue_t *queueObj)
{
	return queueObj->length;
}


uint8_t cli_queue_Push(cli_queue_t *queueObj, uint8_t value)
{
	if (cli_queue_IsFull(queueObj) == true)
		return 0;
	queueObj->buf[queueObj->write_pos++] = value;
	queueObj->length++;
	if (queueObj->write_pos >= CLI_QUEUE_SIZE)
		queueObj->write_pos = 0;
	return 1;
}


uint8_t cli_queue_Pop(cli_queue_t *queueObj, uint8_t *value)
{
	if (cli_queue_IsEmpty(queueObj) == true)
		return 0;

	*value = queueObj->buf[queueObj->read_pos++];
	queueObj->length--;
	if (queueObj->read_pos >= CLI_QUEUE_SIZE)
		queueObj->read_pos = 0;
	return 1;
}


bool cli_queue_IsFull(cli_queue_t *queueObj)
{
	return (queueObj->length >= CLI_QUEUE_SIZE);
}


bool cli_queue_IsEmpty(cli_queue_t *queueObj)
{
	return (queueObj->length == 0);
}

#endif // CONFIG_CLI_ENABLED













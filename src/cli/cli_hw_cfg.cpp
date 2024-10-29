/*
 * cli_hw_cfg.c
 *
 *  Created on: Jun 29, 2020
 *      Author: thaob
 */
#include "cli_config.h"

#if (CONFIG_CLI_ENABLED == 1)

void cli_reset_fcn(void)
{
	esp_restart();
}


void cli_PutChar(char c)
{
	printf("%c", c);
	fflush(stdout);
}

void cli_PutBuff(char *c)
{
	uint16_t len = 0;
	for (len = 0; c[len] != 0; len++);
	if (len == 0) {
		return;
	}
	printf("%s", c);
	fflush(stdout);
}

#endif // CONFIG_CLI_ENABLED













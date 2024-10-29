/*
 * cli_typedef.h
 *
 *  Created on: Jun 29, 2020
 *      Author: thaob
 */

#ifndef CLI_COMMAND_CLI_TYPEDEF_H_
#define CLI_COMMAND_CLI_TYPEDEF_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "cli_config.h"

#if (CONFIG_CLI_ENABLED == 1)

typedef struct
{
	uint8_t		argc;
	char		cmd[CLI_COMMAND_MAX_SIZE + 1];
	char 		argv[CLI_MAX_ARGUMENT][CLI_ARG_MAX_SIZE + 1];
}cli_args_t;

typedef struct
{
	const char	*command;
	uint8_t		arg_num;
	const char 	*despcription;
	uint8_t		(*cli_cb)(void);
}cli_cmd_t;

#endif // CONFIG_CLI_ENABLED

#endif /* CLI_COMMAND_CLI_TYPEDEF_H_ */

/*
 * cli_command.h
 *
 *  Created on: Jun 29, 2020
 *      Author: thaob
 */

#ifndef CLI_COMMAND_CLI_COMMAND_H_
#define CLI_COMMAND_CLI_COMMAND_H_

#include "cli_app.h"
#include "cli_output.h"

#if (CONFIG_CLI_ENABLED == 1)

void cli_command_init(void);

void cli_command_getchar(char c);

void cli_command_running(void);

#endif // CONFIG_CLI_ENABLED

#endif /* CLI_COMMAND_CLI_COMMAND_H_ */

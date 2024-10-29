/*
 * cli_output.h
 *
 *  Created on: Jul 7, 2020
 *      Author: Thao
 */

#ifndef CLI_COMMAND_CLI_OUTPUT_H_
#define CLI_COMMAND_CLI_OUTPUT_H_

#include "cli_config.h"
#include "cli_typedef.h"

#if (CONFIG_CLI_ENABLED == 1)

bool cli_output_checkcommand(cli_cmd_t *cmd_list, uint8_t cmd_count, char *buff, uint16_t buff_len);

int32_t cli_output_getArg_Decimal(uint8_t arg_index);

float cli_output_getArg_Float(uint8_t arg_index);

char *cli_output_getArg_String(uint8_t arg_index);

#endif // CONFIG_CLI_ENABLED

#endif /* CLI_COMMAND_CLI_OUTPUT_H_ */

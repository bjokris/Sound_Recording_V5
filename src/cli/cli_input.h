/*
 * cli_input.h
 *
 *  Created on: Jun 30, 2020
 *      Author: thaob
 */

#ifndef CLI_COMMAND_CLI_INPUT_H_
#define CLI_COMMAND_CLI_INPUT_H_

#include "cli_config.h"
#include "cli_typedef.h"

#if (CONFIG_CLI_ENABLED == 1)

typedef struct
{
	bool		inValid;

	uint8_t		keycode;
	bool		isAlphabet;

	bool		isEnter;
	bool		isBackspace;
	bool		isKeyup;
	bool		isKeydown;
	bool		isKeyright;
	bool		isKeyleft;
	bool		isKeydel;
	bool		isKeyhome;
	bool		isKeyend;
}cli_input_param_t;


typedef struct
{
	char		data[CLI_INPUT_MAX_SIZE + 1];
	uint16_t	length;
	uint16_t	cursor_pos;
}cli_input_buff_t;


#if (CLI_LOG_ENABLE != 0)
typedef struct
{
	char		log[CLI_LOG_SIZE][CLI_INPUT_MAX_SIZE + 1];
	int8_t		cur_pos;
	int8_t		num;
}cli_log_cmd_t;
#endif


void cli_input_get(char c);

cli_input_param_t cli_input_check_keycode(void);

void cli_input_add_char(char c);

void cli_input_keycode_enter(cli_cmd_t *cmd_list);

void cli_input_keycode_backspace(void);

void cli_input_keycode_left(void);

void cli_input_keycode_right(void);

void cli_input_keycode_up(void);

void cli_input_keycode_down(void);

void cli_input_keycode_delete(void);

void cli_input_keycode_home(void);

void cli_input_keycode_end(void);

#endif // CONFIG_CLI_ENABLED

#endif /* CLI_COMMAND_CLI_INPUT_H_ */

















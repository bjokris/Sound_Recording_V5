/*
 * cli_command.c
 *
 *  Created on: Jun 29, 2020
 *      Author: thaob
 */
#include "cli_command.h"
#include "cli_typedef.h"
#include "cli_input.h"

#if (CONFIG_CLI_ENABLED == 1)

static uint8_t cli_command_help(void);
static uint8_t cli_command_reset(void);


static cli_cmd_t cli_list_cmd[CLI_MAX_COMMAND] =
{
//	command			arg number	description												functions
	{"help"			, 0			, "help command"										, cli_command_help					},
	{"reset"		, 0			, "reset mcu"											, cli_command_reset					},
	{"sd-dis"		, 0			, "disconnect SDCard"									, cli_app_unmount					},
	{"sd-list"		, 0			, "list file from direction"							, cli_app_listFileSDCard			},
	/* example: sd-del /test.txt */
	{"sd-del"		, 1			, "delete file from direction"							, cli_app_deleteFileSDCard			},
	/* example: sd-size /test.txt */
	{"sd-size"		, 1			, "get size of file from direction"						, cli_app_getFileSizeSDCard			},
	/* example: record /audio1.csv */
	{"record"		, 1			, "start to record audio to file"						, cli_app_startRecord				},
	/* example: record /audio1.csv <seek index> */
	{"recordseek"	, 2			, "start to record audio to file with seek index"		, cli_app_startRecordSeek			},
	/* example: recordWav /audio1.wav */
	{"recordWav"	, 1			, "start to record audio to wav file"					, cli_app_recordWavToFile			},
	/* example: readfile /audio1.csv */
	{"readfile"		, 1			, "read data from file and print to terminal"			, cli_app_readFile					},
};


void cli_command_init(void)
{
	CLI_PutBuff((char *)CLI_STRING_ARROW);
}


void cli_command_getchar(char c)
{
	cli_input_get(c);
}


//call each minimum 5ms
void cli_command_running(void)
{
	cli_input_param_t input_value = cli_input_check_keycode();

	if (input_value.inValid == false)
	{
		return;
	}
	if (input_value.isEnter == true)
	{
		cli_input_keycode_enter(cli_list_cmd);
	}
	else if (input_value.isBackspace == true)
	{
		cli_input_keycode_backspace();
	}
	else if (input_value.isKeyup == true)
	{
		cli_input_keycode_up();
	}
	else if (input_value.isKeydown == true)
	{
		cli_input_keycode_down();
	}
	else if (input_value.isKeyright == true)
	{
		cli_input_keycode_right();
	}
	else if (input_value.isKeyleft == true)
	{
		cli_input_keycode_left();
	}
	else if (input_value.isKeydel == true)
	{
		cli_input_keycode_delete();
	}
	else if (input_value.isKeyhome == true)
	{
		cli_input_keycode_home();
	}
	else if (input_value.isKeyend == true)
	{
		cli_input_keycode_end();
	}
	else if (input_value.isAlphabet == true)
	{
		cli_input_add_char(input_value.keycode);
	}
}


static uint8_t cli_command_help(void)
{
	CLI_PutBuff((char *)"\r\n");
	for (uint8_t index = 0; index < CLI_MAX_COMMAND; index++)
	{
		if (cli_list_cmd[index].command == NULL)
			continue;
		CLI_PutBuff((char *)"   ");
		CLI_PutBuff((char *)cli_list_cmd[index].command);
		for (uint8_t ui = strlen(cli_list_cmd[index].command); ui < 32; ui++)
			CLI_PutChar(' ');
		CLI_PutBuff((char *)cli_list_cmd[index].despcription);
		CLI_PutBuff((char *)CLI_STRING_ENTER);
	}

	return 0;
}


static uint8_t cli_command_reset(void)
{
	cli_reset_fcn();
	return 0;
}

#endif // CONFIG_CLI_ENABLED




























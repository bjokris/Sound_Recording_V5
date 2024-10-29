/*
 * cli_input.c
 *
 *  Created on: Jun 30, 2020
 *      Author: thaob
 */
#include "cli_input.h"
#include "cli_config.h"
#include "cli_queue.h"
#include "cli_output.h"

#if (CONFIG_CLI_ENABLED == 1)

#define INPUT_POP_MAX_LEN			4
#define CLI_NULL					'\0'

typedef union
{
	uint32_t	u32;
	uint8_t		u8[INPUT_POP_MAX_LEN];
}key_code_t;

typedef struct
{
	key_code_t		buf;
	uint8_t 		count;
}key_buf_t;

static key_code_t keycode_enter		= {.u8 = {0x0D, 0x00, 0x00, 0x00}};
static key_code_t keycode_backspace	= {.u8 = {0x08, 0x00, 0x00, 0x00}};
static key_code_t keycode_up		= {.u8 = {0x1B, 0x5B, 0x41, 0x00}};
static key_code_t keycode_down		= {.u8 = {0x1B, 0x5B, 0x42, 0x00}};
static key_code_t keycode_right	 	= {.u8 = {0x1B, 0x5B, 0x43, 0x00}};
static key_code_t keycode_left		= {.u8 = {0x1B, 0x5B, 0x44, 0x00}};
static key_code_t keycode_del		= {.u8 = {0x7F, 0x00, 0x00, 0x00}};
static key_code_t keycode_home		= {.u8 = {0x1B, 0x5B, 0x31, 0x7E}};
static key_code_t keycode_end		= {.u8 = {0x1B, 0x5B, 0x34, 0x7E}};

static cli_input_buff_t cli_input_buff = {0};
static cli_queue_t cli_queue = {0};

#if (CLI_LOG_ENABLE != 0)
static cli_log_cmd_t cli_log_cmd = {0};
static bool	log_IsUP = true;
static int8_t log_old_pos = 0;
#endif

static key_buf_t key_buf = {0};


static void cli_input_refresh(char *cmd);
static void cli_input_saved_cmd(char *cmd);
static char *cli_input_get_logcmd(bool isUp);


void cli_input_get(char c)
{
	cli_queue_Push(&cli_queue, c);
}


cli_input_param_t cli_input_check_keycode(void)
{
	cli_input_param_t i_value = {0};
	bool in_wait = false;

	i_value.inValid = true;
	cli_queue_Pop(&cli_queue, &key_buf.buf.u8[key_buf.count]);

	if (key_buf.buf.u32 == keycode_enter.u32)
		i_value.isEnter = true;
	else if (key_buf.buf.u32 == keycode_backspace.u32)
		i_value.isBackspace = true;
	else if (key_buf.buf.u32 == keycode_up.u32)
		i_value.isKeyup = true;
	else if (key_buf.buf.u32 == keycode_down.u32)
		i_value.isKeydown = true;
	else if (key_buf.buf.u32 == keycode_right.u32)
		i_value.isKeyright = true;
	else if (key_buf.buf.u32 == keycode_left.u32)
		i_value.isKeyleft = true;
	else if (key_buf.buf.u32 == keycode_del.u32)
		i_value.isKeydel = true;
	else if (key_buf.buf.u32 == keycode_home.u32)
		i_value.isKeyhome = true;
	else if (key_buf.buf.u32 == keycode_end.u32)
		i_value.isKeyend = true;
	else if (key_buf.buf.u8[0] == 0x1B/*'\e'*/)
	{
		key_buf.count++;
		in_wait = true;
	}
	else if ((key_buf.buf.u8[key_buf.count] >= 0x20) && (key_buf.buf.u8[key_buf.count] <= 0x7E))
	{
		i_value.isAlphabet = true;
		i_value.keycode = key_buf.buf.u8[key_buf.count];
	}
	else
	{
		key_buf.count = 0;
		key_buf.buf.u32 = 0;
		i_value.inValid = false;
		return i_value;
	}

	if ((in_wait == false) || (key_buf.count >= INPUT_POP_MAX_LEN))
	{
		key_buf.count = 0;
		key_buf.buf.u32 = 0;
	}

	return i_value;
}


void cli_input_add_char(char c)
{
	if (cli_input_buff.length >= CLI_INPUT_MAX_SIZE)
		return;

	cli_input_buff.data[cli_input_buff.length] = c;
	cli_input_buff.length++;
	cli_input_buff.cursor_pos++;
	CLI_PutChar(c);
}


void cli_input_keycode_enter(cli_cmd_t *cmd_list)
{
	CLI_PutBuff((char *)CLI_STRING_ENTER);
	if (cli_input_buff.length > 0)
	{
		if (cli_output_checkcommand(cmd_list, CLI_MAX_COMMAND, cli_input_buff.data, cli_input_buff.length) == false)
		{
			CLI_PutBuff((char *)"command input error. Pls check \"help\" to get command list");
		}
		CLI_PutBuff((char *)CLI_STRING_ENTER);

		//save cmd
		cli_input_saved_cmd(cli_input_buff.data);

		//reset buffer
		for (uint16_t i = 0; i < cli_input_buff.length; i++)
			cli_input_buff.data[i] = 0;
		cli_input_buff.cursor_pos = 0;
		cli_input_buff.length = 0;
	}
	cli_log_cmd.cur_pos = 0;
	CLI_PutBuff((char *)CLI_STRING_ARROW);
}


void cli_input_keycode_backspace(void)
{
	if (cli_input_buff.cursor_pos > 0)
	{
		CLI_PutBuff((char *)keycode_left.u8);
		CLI_PutBuff((char *)&cli_input_buff.data[cli_input_buff.cursor_pos]);
		CLI_PutChar(' ');

		for (uint16_t i = cli_input_buff.cursor_pos - 1; i < cli_input_buff.length; i++)
			CLI_PutBuff((char *)keycode_left.u8);

		if (cli_input_buff.cursor_pos == cli_input_buff.length)
			cli_input_buff.data[cli_input_buff.length - 1] = 0;
		else
		{
			for (uint16_t i = cli_input_buff.cursor_pos; i < cli_input_buff.length; i++)
			{
				cli_input_buff.data[i - 1] = cli_input_buff.data[i];
			}
			cli_input_buff.data[cli_input_buff.length - 1] = 0;
		}
		cli_input_buff.length--;
		cli_input_buff.cursor_pos--;
	}
}


void cli_input_keycode_left(void)
{
	if (cli_input_buff.cursor_pos > 0)
	{
		cli_input_buff.cursor_pos--;
		CLI_PutBuff((char *)keycode_left.u8);
	}
}


void cli_input_keycode_right(void)
{
	if (cli_input_buff.cursor_pos < cli_input_buff.length)
	{
		cli_input_buff.cursor_pos++;
		CLI_PutBuff((char *)keycode_right.u8);
	}
}


void cli_input_keycode_up(void)
{
#if (CLI_LOG_ENABLE != 0)
	cli_input_refresh(cli_input_get_logcmd(true));
#endif
}


void cli_input_keycode_down(void)
{
#if (CLI_LOG_ENABLE != 0)
	cli_input_refresh(cli_input_get_logcmd(false));
#endif
}


void cli_input_keycode_delete(void)
{
	if (cli_input_buff.cursor_pos < cli_input_buff.length)
	{
		CLI_PutBuff((char *)&cli_input_buff.data[cli_input_buff.cursor_pos + 1]);
		CLI_PutChar(' ');

		for (uint16_t i = cli_input_buff.cursor_pos; i < cli_input_buff.length; i++)
			CLI_PutBuff((char *)keycode_left.u8);

		if (cli_input_buff.cursor_pos == cli_input_buff.length)
			cli_input_buff.data[cli_input_buff.length - 1] = 0;
		else
		{
			for (uint16_t i = cli_input_buff.cursor_pos; i < cli_input_buff.length; i++)
			{
				cli_input_buff.data[i] = cli_input_buff.data[i + 1];
			}
			cli_input_buff.data[cli_input_buff.length - 1] = 0;
		}
		cli_input_buff.length--;
	}
}


void cli_input_keycode_home(void)
{
	if (cli_input_buff.cursor_pos > 0)
	{
		for (uint16_t i = 0; i < cli_input_buff.cursor_pos; i++)
			CLI_PutBuff((char *)keycode_left.u8);
		cli_input_buff.cursor_pos = 0;
	}
}


void cli_input_keycode_end(void)
{
	if (cli_input_buff.cursor_pos < cli_input_buff.length)
	{
		for (uint16_t i = cli_input_buff.cursor_pos; i < cli_input_buff.length; i++)
			CLI_PutBuff((char *)keycode_right.u8);
		cli_input_buff.cursor_pos = cli_input_buff.length;
	}
}


static void cli_input_refresh(char *cmd)
{
	if (cmd == NULL)
		return;

	cli_input_keycode_home();
	CLI_PutBuff(cmd);
	cli_input_buff.cursor_pos = strlen(cmd);

	int offset = 0;
	if (cli_input_buff.length > cli_input_buff.cursor_pos)
	{
		offset = cli_input_buff.length - cli_input_buff.cursor_pos;
		for (uint16_t i = 0; i < offset; i++)
		{
			CLI_PutChar(' ');
		}

		for (uint16_t i = 0; i < offset; i++)
		{
			CLI_PutBuff((char *)keycode_left.u8);
		}
	}

	strcpy(cli_input_buff.data, cmd);
	cli_input_buff.length = cli_input_buff.cursor_pos;
}


static void cli_input_saved_cmd(char *cmd)
{
	uint8_t num = cli_log_cmd.num;
	if (cli_log_cmd.num == CLI_LOG_SIZE)
	{
		num--;
	}
	else
	{
		cli_log_cmd.num++;
	}

	for (int8_t i = num; i > 0; i--)
	{
		strcpy(cli_log_cmd.log[i], cli_log_cmd.log[i - 1]);
	}

	strcpy(cli_log_cmd.log[0], cmd);
}


static char *cli_input_get_logcmd(bool isUp)
{
	if (isUp == true)
	{
		if (cli_log_cmd.num == 0)
			return NULL;
		if (log_IsUP == false)
		{
			if (log_old_pos < (cli_log_cmd.num - 1))
				cli_log_cmd.cur_pos = log_old_pos + 1;
			else
				cli_log_cmd.cur_pos = log_old_pos;
		}
		log_IsUP = true;
		log_old_pos = cli_log_cmd.cur_pos;
		char *cmd = cli_log_cmd.log[cli_log_cmd.cur_pos];
		if (cli_log_cmd.cur_pos < (cli_log_cmd.num - 1))
			cli_log_cmd.cur_pos++;
		return cmd;
	}
	else
	{
		if (cli_log_cmd.num == 0)
			return NULL;
		if (log_IsUP == true)
		{
			if (log_old_pos > 0)
				cli_log_cmd.cur_pos = log_old_pos - 1;
			else
			cli_log_cmd.cur_pos = log_old_pos;
		}
		log_IsUP = false;
		log_old_pos = cli_log_cmd.cur_pos;
		char *cmd = cli_log_cmd.log[cli_log_cmd.cur_pos];
		if (cli_log_cmd.cur_pos > 0)
			cli_log_cmd.cur_pos--;
		return cmd;
	}

	return NULL;
}

#endif // CONFIG_CLI_ENABLED




















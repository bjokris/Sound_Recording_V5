/*
 * cli_config.h
 *
 *  Created on: Jun 29, 2020
 *      Author: thaob
 */

#ifndef CLI_COMMAND_CLI_CONFIG_H_
#define CLI_COMMAND_CLI_CONFIG_H_

#include "app_cfg.h"

#if (CONFIG_CLI_ENABLED == 1)

//command config
#define CLI_MAX_COMMAND					(10)
#define CLI_MAX_ARGUMENT				(2)
#define CLI_COMMAND_MAX_SIZE			(16)
#define CLI_ARG_MAX_SIZE				(32)
#define CLI_LOG_SIZE					(3)
#define CLI_STRING_ENTER				"\r\n"
#define CLI_STRING_ARROW				"\033[0;32mESP32>> \033[0m"
#define CLI_QUEUE_SIZE					(64)

#define CLI_INPUT_MAX_SIZE				(CLI_COMMAND_MAX_SIZE + CLI_MAX_ARGUMENT * (CLI_ARG_MAX_SIZE + 1) + 1)

//cli config
#define CLI_PRINT_OUTPUT				(1)
#define CLI_LOG_ENABLE					(1)
#define CLI_AUTO_COMPLETE				(1)
#define ECHO_EN							(1)
#define TERM_DEFAULT_ALLOC_EN			(1)
#define TERM_DEFAULT_STRING_EN			(1)


// ************************* IO Terminal Settings ***************************

#if (CLI_PRINT_OUTPUT == 1)
void cli_reset_fcn(void);
void cli_PutChar(char c);
void cli_Print(const char* str);
void cli_PutBuff(char *c);

#if (ECHO_EN == 1)
#define CLI_PutChar						cli_PutChar
#define CLI_PutBuff						cli_PutBuff
#else	// ECHO_EN != 1
#define CLI_PutChar
#endif	// ECHO_EN == 1

#else	// CLI_PRINT_OUTPUT != 1
#define CLI_Printf
#define CLI_PutChar
#endif	// CLI_PRINT_OUTPUT == 1
// **************************************************************************

// ********************** memory allocate functions *************************

//#if (TERM_DEFAULT_ALLOC_EN == 1)
//#include <malloc.h>
//#define cli_malloc		malloc
//#define cli_free		free
//#else
//#define cli_malloc		pvPortMalloc
//#define cli_free		vPortFree
//#endif

// **************************************************************************

// *************************** string functions *****************************

#if (TERM_DEFAULT_STRING_EN == 1)
#include <string.h>
#define cli_memcpy		memcpy
#else
#define cli_memcpy		// your implementation
#endif

// **************************************************************************


#endif //CONFIG_CLI_ENABLED

#endif /* CLI_COMMAND_CLI_CONFIG_H_ */

#include "cli_task.h"
#include "cli/cli_command.h"

#if (CONFIG_CLI_ENABLED == 1)

CONFIG_LOG_TAG(CLI_TASK)


void cliTask(void *pArg)
{
    DBG_OUT_I("CLI started\r\n");

    while (1) {
        FUNC_DELAY_MS(50);

        if (CONFIG_DEBUG_PORT.available() > 0) {
            char c = 0;
            if (CONFIG_DEBUG_PORT.read(&c, 1) > 0) {
                cli_command_getchar(c);
                cli_command_running();
            }
        }
    }
}
#endif //CONFIG_CLI_ENABLED
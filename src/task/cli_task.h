
#ifndef __CLI_TASK_H
#define __CLI_TASK_H

#include "app_cfg.h"

#if (CONFIG_CLI_ENABLED == 1)

void cliTask(void *pArg);

#endif //CONFIG_CLI_ENABLED

#endif // __CLI_TASK_H
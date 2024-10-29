
#ifndef __TX_TASK_H
#define __TX_TASK_H

#include "app_cfg.h"

#if CONFIG_MODE_RX == false

void tx_task(void *pArg);

#endif // #if CONFIG_MODE_RX == false

#endif // __TX_TASK_H
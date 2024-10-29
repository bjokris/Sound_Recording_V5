
#ifndef __RX_TASK_H
#define __RX_TASK_H

#include "app_cfg.h"

#if CONFIG_MODE_RX == true

void rx_task(void *pArg);

#endif // #if CONFIG_MODE_RX == true

#endif // __RX_TASK_H
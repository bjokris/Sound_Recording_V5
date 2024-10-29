/*
 * cli_app.h
 *
 *  Created on: May 22, 2021
 *      Author: thaob
 */

#ifndef cli_app_CLI_APP_CLI_APP_H_
#define cli_app_CLI_APP_CLI_APP_H_

#include "app_cfg.h"

uint8_t cli_app_unmount(void);

uint8_t cli_app_listFileSDCard(void);

uint8_t cli_app_deleteFileSDCard(void);

uint8_t cli_app_getFileSizeSDCard(void);

uint8_t cli_app_startRecord(void);

uint8_t cli_app_startRecordSeek(void);

uint8_t cli_app_recordWavToFile(void);

uint8_t cli_app_readFile(void);

#endif /* cli_app_CLI_APP_CLI_APP_H_ */

/*
 * cli_app.c
 *
 *  Created on: May 22, 2021
 *      Author: thaob
 */
#include "cli_app.h"
#include "cli_command.h"
#include "driver/sdcard.h"
#include "seek/seekTable.h"

#if (CONFIG_CLI_ENABLED == 1)

CONFIG_LOG_TAG(CLI_APP)
extern bool g_start_record;
extern bool g_start_record_wav;
extern char g_audio_file_name[128];
extern int8_t g_seek_cmd_index;


uint8_t cli_app_unmount(void)
{
    sdcard_deinit();
	return 1;
}


uint8_t cli_app_listFileSDCard(void)
{
    sdcard_listFile("/");
	return 1;
}


uint8_t cli_app_deleteFileSDCard(void)
{
    char *file = cli_output_getArg_String(0);
    sdcard_deleteFile((const char *)file);
	return 1;
}


uint8_t cli_app_getFileSizeSDCard(void)
{
    char *file = cli_output_getArg_String(0);
    int size = sdcard_readFileSize((const char *)file);
    DBG_OUT("File size: %d\r\n", size);
	return 1;
}


uint8_t cli_app_startRecord(void)
{
    char *fileName = cli_output_getArg_String(0);
    memset(g_audio_file_name, 0, sizeof(g_audio_file_name));
    if (strlen(fileName) > 0) {
        strcpy(g_audio_file_name, fileName);
        g_start_record = true;
    } else {
        DBG_OUT_E("file name is null\r\n");
    }
	return 1;
}


uint8_t cli_app_startRecordSeek(void)
{
    char *fileName = cli_output_getArg_String(0);
    int seek_index = cli_output_getArg_Decimal(1);
    memset(g_audio_file_name, 0, sizeof(g_audio_file_name));
    if (strlen(fileName) > 0) {
        strcpy(g_audio_file_name, fileName);
        if ((seek_index >= ENUM_SEEK_0_INDEX) && (seek_index < ENUM_SEEK_MAX_INDEX)) {
            g_seek_cmd_index = seek_index;
            g_start_record = true;
        } else {
            DBG_OUT_E("seek index doesn't existed\r\n");
        }
    } else {
        DBG_OUT_E("file name is null\r\n");
    }
	return 1;
}


uint8_t cli_app_recordWavToFile(void)
{
    char *fileName = cli_output_getArg_String(0);
    memset(g_audio_file_name, 0, sizeof(g_audio_file_name));
    if (strlen(fileName) > 0) {
        strcpy(g_audio_file_name, fileName);
        g_start_record_wav = true;
    } else {
        DBG_OUT_E("file name is null\r\n");
    }
	return 1;
}


uint8_t cli_app_readFile(void)
{
    char *fileName = cli_output_getArg_String(0);
    if (strlen(fileName) > 0) {
        File file;
        if (sdcard_openToRead(&file, (const char *)fileName) == true) {
            char buf[512] = {0};
            sdcard_readStream(file, (uint8_t *)buf, sizeof(buf) - 1);
            DBG_OUT_I("%s", buf);
        }
        sdcard_closeFile(file);
    } else {
        DBG_OUT_E("file name is null\r\n");
    }
	return 1;
}

#endif




























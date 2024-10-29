
#include "tx_task.h"
#include "driver/recordWav.h"
#include "driver/recordRaw.h"
#include "driver/sdcard.h"
#include "driver/appI2S.h"
#include "seek/seekTable.h"
#include "lora/lora_sx1262.h"
#include "common/dateTime.h"

bool g_start_record_wav = false;
char g_audio_file_name[128] = {0};
bool g_start_record = false;

/* seek index */
int8_t g_seek_cmd_index = -1;
uint32_t g_seek_result_num = 0;

#if CONFIG_MODE_RX == false

CONFIG_LOG_TAG(TX_TASK)
/* bytes array to store audio data */
static uint8_t audio_buf[131072] = {0};
static uint32_t g_record_last_ms = 0;


/* fileName needs to start by '/' */
static void save_audio_toSDCard(char *fileName, uint8_t *data, uint32_t data_len);
/* fileName needs to start by '/' */
static void save_audioWav_toSDCard(char *fileName, uint8_t *data,
            uint32_t data_len, uint8_t *header, int header_len);
static void write_seekResult_toSdcard(uint16_t seek_index, dateTime_t dt);


void tx_task(void *pArg)
{
    DBG_OUT_I("app_task started\r\n");
    FUNC_DELAY_MS(1000);

    while (1) {
        FUNC_DELAY_MS(100);

        if ((FUNC_GET_TICK_MS() - g_record_last_ms) >= 60000) {
            g_start_record = true;
            g_record_last_ms = FUNC_GET_TICK_MS();
        }

        /* record raw file */
        if (g_start_record == true) {
            DBG_OUT_I("start to record audio to file \"%s\"\r\n", g_audio_file_name);
            /* start to record and saving to ram (audio_buf) */
            recordRaw_toRam_start(audio_buf, sizeof(audio_buf));
            DBG_OUT_I("Done to record audio to buffer\r\n");        

            int seek_index = CONFIG_SEEK_DEFAULT_INDEX;
            if ((g_seek_cmd_index >= ENUM_SEEK_0_INDEX) && (g_seek_cmd_index < ENUM_SEEK_MAX_INDEX)) {
                seek_index = g_seek_cmd_index;
            }

            /* implement seek */
            if ((seek_index >= 0) && (seek_index < ENUM_SEEK_MAX_INDEX)) {
                /* get current datetime */
                dateTime_t dt;
                g_dateTime.getDateTime(&dt);

                /* save to file .csv */
                char seekFilePath[64] = {0};
                snprintf(seekFilePath, sizeof(seekFilePath), "/seek_%d_audio_%02d%02d%04d_%02d%02d%02d.csv",
                                seek_index, dt.day, dt.month, dt.year, dt.hour, dt.minute, dt.second);
                DBG_OUT_I("save audio buffer to file \"%s\"\r\n", seekFilePath);
                save_audio_toSDCard(seekFilePath, audio_buf, sizeof(audio_buf));
                DBG_OUT_I("Finished\r\n");
                DBG_OUT_RAW_I("=====> Doing Seek%d\r\n", seek_index);

                /* implement seek do_calc function */
                if (g_seekTable[seek_index].func.func_do_calc != nullptr) {
                    g_seekTable[seek_index].func.func_do_calc(
                            &g_seekTable[seek_index].data, audio_buf, sizeof(audio_buf));
                    /* check warning/alarm */
                    if (g_seekTable[seek_index].func.func_warningAlarm != nullptr) {
                        g_seekTable[seek_index].func.func_warningAlarm(
                                &g_seekTable[seek_index].data);

                        /* get result and saving to sdcard */
                        write_seekResult_toSdcard(seek_index, dt);

                        /* if you want to call sending function without function of seek you can call it here */
                        uint8_t dataToLora[16] = {0};
                        dataToLora[0] = dt.day;
                        dataToLora[1] = dt.month;
                        dataToLora[2] = (uint8_t)((dt.year >> 8) & 0xFF);
                        dataToLora[3] = (uint8_t)((dt.year >> 0) & 0xFF);
                        dataToLora[4] = dt.hour;
                        dataToLora[5] = dt.minute;
                        dataToLora[6] = dt.second;
                        dataToLora[7] = seek_index; /* seek index */
                        memcpy(&dataToLora[8], &g_seek_result_num, sizeof(g_seek_result_num)); /* seek result num */
                        memcpy(&dataToLora[12], &g_seekTable[seek_index].data.magnitude,
                                sizeof(g_seekTable[seek_index].data.magnitude));
                        DBG_OUT("Lora TX: len: %d, data\r\n", sizeof(dataToLora));
                        for (uint16_t i = 0; i < sizeof(dataToLora); i++) {
                            DBG_OUT_RAW("%02X ", dataToLora[i]);
                        }
                        DBG_OUT_RAW("\r\n");
                        if (g_lora_sx1262.send(dataToLora, sizeof(dataToLora)) == false) {
                            DBG_OUT_E("failed to send data via lora\r\n");
                        } else {
                            DBG_OUT_I("send OK\r\n");
                        }

                        g_seek_result_num++;
                    }
                }
            } else {
                /* save to file .csv */
                DBG_OUT_I("save audio buffer to file \"%s\"\r\n", g_audio_file_name);
                save_audio_toSDCard(g_audio_file_name, audio_buf, sizeof(audio_buf));
                DBG_OUT_I("Finished\r\n");
            }
            /* reset seek index */
            if (seek_index == g_seek_cmd_index) {
                g_seek_cmd_index = -1;
            }

            g_start_record = false;
        }
        /* record .wav file */
        else if (g_start_record_wav == true) {
            DBG_OUT_I("start to record audio to file \"%s\"\r\n", g_audio_file_name);
            /* start to record and saving to ram (audio_buf) */
            recordRaw_toRam_start(audio_buf, sizeof(audio_buf)); 
            DBG_OUT_I("Done to record audio to buffer\r\n");
            /* create header for wav file */
            uint8_t wavHeader[64] = {0};
            int wavHeader_len =  recordWav_generateWAV_header(wavHeader, sizeof(audio_buf));
            /* write to wav file */
            save_audioWav_toSDCard(g_audio_file_name, audio_buf, sizeof(audio_buf), wavHeader, wavHeader_len);
            DBG_OUT_I("Finished\r\n");
            g_start_record_wav = false;
        }
    }
}


/* fileName needs to start by '/' */
static void save_audio_toSDCard(char *fileName, uint8_t *data, uint32_t data_len)
{
    File file;
    bool rc = sdcard_openToWrie(&file, (const char *)fileName);
    if (rc == false) {
        DBG_OUT_E("failed to open \"%s\" for writing\r\n", fileName);
        return;
    }

    uint32_t wrote_len = 0;
    char buf[32] = {0};
    do {
        int len = snprintf(buf, sizeof(buf), "%d\r\n", *((int16_t *)&data[wrote_len]));
        if (sdcard_writeStream(file, (uint8_t *)buf, len) != len) {
            DBG_OUT_E("wrote error at bytes %d\r\n", wrote_len);
        } else {
            // DBG_OUT("wrote: %s\r\n", buf);
            wrote_len += 2;
        }
    } while (wrote_len < data_len);

    DBG_OUT_I("Wrote audio data to file \"%s\"\r\n", fileName);

    sdcard_closeFile(file);
}


/* fileName needs to start by '/' */
static void save_audioWav_toSDCard(char *fileName, uint8_t *data,
            uint32_t data_len, uint8_t *header, int header_len)
{
    File file;
    bool rc = sdcard_openToWrie(&file, (const char *)fileName);
    if (rc == false) {
        DBG_OUT_E("failed to open \"%s\" for writing\r\n", fileName);
        return;
    }

    /* write header */
    sdcard_writeStream(file, header, header_len);

    uint32_t wrote_len = 0;
    do {
        int availen = ((data_len - wrote_len) >= CONFIG_RECORD_FRAME)?
                    CONFIG_RECORD_FRAME : (data_len - wrote_len);
        sdcard_writeStream(file, &data[wrote_len], availen);
        wrote_len += availen;
    } while (wrote_len < data_len);

    DBG_OUT_I("Wrote audio data to file \"%s\"\r\n", fileName);

    sdcard_closeFile(file);
}


static void write_seekResult_toSdcard(uint16_t seek_index, dateTime_t dt)
{
    seek_data_t seek_data = g_seekTable[seek_index].data;

    char buf[128] = {0};
    int buf_len = snprintf(buf, sizeof(buf), "%02d/%02d/%04d-%02d:%02d:%02d,%d,%f\r\n",
                    dt.day, dt.month, dt.year, dt.hour, dt.minute, dt.second,
                    g_seek_result_num, "Magnitude; ", seek_data.magnitude, "Crest Factor" , seek_data.peak);
    DBG_OUT("Magnitude: %f\r\n", seek_data.magnitude, " Crest Factor", seek_data.peak);
    char filePath[128] = {0};
    DBG_OUT_I("seek to file: %s\r\n", buf);
    snprintf(filePath, sizeof(filePath), "/seek_%d_tx_result.csv", seek_index);
    if (sdcard_writeFileAppend((const char *)filePath, (uint8_t *)buf, buf_len) == buf_len) {
        DBG_OUT_I("Wrote seek result to file \"%s\"\r\n", filePath);
    } else {
        DBG_OUT_E("failed to write seek result to file\r\n");
    }
}

#endif // #if CONFIG_MODE_RX == false
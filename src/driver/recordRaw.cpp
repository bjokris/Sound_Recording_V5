
#include "FS.h"
#include <LittleFS.h>
#include "recordRaw.h"
#include "appI2S.h"

CONFIG_LOG_TAG(RECORDRAW)


void recordRaw_toFlash_start(const char *filePath, int record_size)
{
    File file = LittleFS.open(filePath, FILE_WRITE, true);
    if (!file) {
        DBG_OUT_E("failed to open file for writing\r\n");
        return;
    }

    uint8_t mic_buf[CONFIG_RECORD_FRAME + 1] = {0};

    DBG_OUT_I("\r\n *** Recording Start *** \r\n");
    appI2S_stop();
    FUNC_DELAY_MS(10);
    int64_t ms = FUNC_GET_TICK_MS();
    appI2S_start();
    appI2S_clear_buff();
    appI2S_read_data((void *)mic_buf, CONFIG_RECORD_FRAME);

    int flash_wr_size = 0;
    while (flash_wr_size < record_size) {
        int availen = ((record_size - flash_wr_size) >= CONFIG_RECORD_FRAME)?
                        CONFIG_RECORD_FRAME : (record_size - flash_wr_size);
        int bytes_read = appI2S_read_data(mic_buf, availen);
        if (bytes_read > 0) {
            if (file.write(mic_buf, bytes_read) == bytes_read) {
                flash_wr_size += bytes_read;
                DBG_OUT("Sound recording: %d bytes - %u%%\r\n", flash_wr_size, flash_wr_size * 100 / record_size);
            }
        }
    }

    appI2S_stop();
    file.close();
    DBG_OUT_I("*** Recording Done %d bytes (%.1f seconds) in %lld ms *** \r\n",
                flash_wr_size,
                (float)record_size / CONFIG_MIC_SAMPLE_RATE / (CONFIG_MIC_SAMPLE_BIT / 8),
                FUNC_GET_TICK_MS() - ms);
}


void recordRaw_toRam_start(uint8_t *mic_buf, int record_size)
{
    uint8_t frame[CONFIG_RECORD_FRAME + 1] = {0};

    DBG_OUT_I("\r\n *** Recording Start *** \r\n");
    appI2S_stop();
    FUNC_DELAY_MS(10);
    int64_t ms = FUNC_GET_TICK_MS();
    appI2S_start();
    // appI2S_clear_buff();
    for (uint8_t i = 0; i < (CONFIG_RECORD_1_SECOND_SIZE / CONFIG_RECORD_FRAME); i++) {
        appI2S_read_data((void *)frame, sizeof(frame) - 1);
    }

    int flash_wr_size = 0;
    while (flash_wr_size < record_size) {
        int availen = ((record_size - flash_wr_size) >= CONFIG_RECORD_FRAME)?
                        CONFIG_RECORD_FRAME : (record_size - flash_wr_size);
        int bytes_read = appI2S_read_data(frame, availen);
        if (bytes_read > 0) {
            memcpy(&mic_buf[flash_wr_size], frame, bytes_read);
            flash_wr_size += bytes_read;
            DBG_OUT("Sound recording: %d bytes - %u%%\r\n", flash_wr_size, flash_wr_size * 100 / record_size);
        }
    }

    appI2S_stop();
    DBG_OUT_I("*** Recording Done %d bytes (%.1f seconds) in %lld ms *** \r\n",
                flash_wr_size,
                (float)record_size / CONFIG_MIC_SAMPLE_RATE / (CONFIG_MIC_SAMPLE_BIT / 8),
                FUNC_GET_TICK_MS() - ms);
}


bool recordRaw_writeFile(const char *filePath, uint8_t *mic_buf, int size)
{
    int64_t ms = FUNC_GET_TICK_MS();
    File file = LittleFS.open(filePath, FILE_WRITE, true);
    if (!file) {
        DBG_OUT_E("failed to open file for writing\r\n");
        return false;
    }

    int flash_wr_size = 0;
    while (flash_wr_size < size) {
        int availen = ((size - flash_wr_size) >= 4096)? 4096 : (size - flash_wr_size);
        if (file.write(&mic_buf[flash_wr_size], availen) == availen) {
            flash_wr_size += availen;
            DBG_OUT("Wrote to flash: %d bytes - %u%%\r\n", flash_wr_size, flash_wr_size * 100 / size);
        }
    }

    appI2S_stop();
    file.close();
    DBG_OUT_I("*** write Flash Done %d bytes (%.1f seconds) in %lld ms *** \r\n",
                flash_wr_size,
                (float)size / CONFIG_MIC_SAMPLE_RATE / (CONFIG_MIC_SAMPLE_BIT / 8),
                FUNC_GET_TICK_MS() - ms);
    return true;
}


int recordRaw_readFile(const char *filePath, uint8_t *buf, int size)
{
    int64_t ms = FUNC_GET_TICK_MS();
    File file = LittleFS.open(filePath, FILE_READ, false);
    if (!file || file.isDirectory()) {
        Serial.println("failed to open file for reading\r\n");
        return 0;
    }

    int len = 0;
    int total_len = file.available();
    while ((len < total_len) && (len < size)) {
        int availen = ((total_len - len) >= CONFIG_RECORD_FRAME)?
                CONFIG_RECORD_FRAME : (total_len - len);
        int got_len = file.read(&buf[len], availen);
        if (got_len > 0) {
            len += got_len;
        } else {
            break;
        }
    }
    file.close();
    DBG_OUT_I("*** read Flash Done %d bytes (%.1f seconds) in %lld ms *** \r\n",
                len,
                (float)(size / CONFIG_MIC_SAMPLE_RATE / (CONFIG_MIC_SAMPLE_BIT / 8)),
                FUNC_GET_TICK_MS() - ms);
    return len;
}

#include "rx_task.h"
#include "lora/lora_sx1262.h"
#include "driver/sdcard.h"
#include "common/dateTime.h"

#if CONFIG_MODE_RX == true

CONFIG_LOG_TAG(RX_TASK)


void rx_task(void *pArg)
{
    DBG_OUT_I("RX Task started\r\n");
    FUNC_DELAY_MS(1000);
    uint8_t lora_rx_buf[256] = {0};
    int lora_rx_buf_len = 0;
    uint32_t ms = FUNC_GET_TICK_MS();

    while (1) {
        FUNC_DELAY_MS(10);

        lora_rx_buf_len = g_lora_sx1262.getRxAvailable();
        if (lora_rx_buf_len > 0) {
            g_lora_sx1262.getRx(lora_rx_buf, lora_rx_buf_len);
            DBG_OUT("Lora RX: len: %d, data\r\n", lora_rx_buf_len);
            for (uint16_t i = 0; i < lora_rx_buf_len; i++) {
                DBG_OUT_RAW("%02X ", lora_rx_buf[i]);
            }
            DBG_OUT_RAW("\r\n");

            floatmagniude = 0;
            char buf[128] = {0};
            int buf_len = 0;
            uint8_t seek_index = 0;
            uint32_t seek_result_num = 0;
            dateTime_t dt;

            dt.day = lora_rx_buf[0];
            dt.month = lora_rx_buf[1];
            dt.year = ((uint16_t)lora_rx_buf[2] << 8) | lora_rx_buf[3];
            dt.hour = lora_rx_buf[4];
            dt.minute = lora_rx_buf[5];
            dt.second = lora_rx_buf[6];
            seek_index = lora_rx_buf[7];
            memcpy(&seek_result_num, &lora_rx_buf[8], sizeof(seek_result_num));
            memcpy(&magnitude, &lora_rx_buf[12], sizeof(magnitude));

            DBG_OUT_I("RXmagniude: %f\r\n",magniude);
            char filePath[128] = {0};
            snprintf(filePath, sizeof(filePath), "/seek_%d_rx_result.csv", seek_index);
            buf_len = snprintf(buf, sizeof(buf), "%02d/%02d/%04d-%02d:%02d:%02d,%d,%f\r\n",
                            dt.day, dt.month, dt.year, dt.hour, dt.minute, dt.second,
                            seek_result_num,magniude);

            if (sdcard_writeFileAppend((const char *)filePath, (uint8_t *)buf, buf_len) == buf_len) {
                DBG_OUT_I("Wrote seek result to file \"%s\"\r\n", filePath);
            } else {
                DBG_OUT_E("failed to write seek result to file\r\n");
            }
        }
        lora_rx_buf_len = 0;
    }
}

#endif // #if CONFIG_MODE_RX == true
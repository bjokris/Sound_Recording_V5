
#include <WiFi.h>
#include "net_task.h"
#include "common/dateTime.h"

CONFIG_LOG_TAG(NET_TASK)


void net_task(void *pArg)
{
    DBG_OUT_I("NET Task started\r\n");

    while (1) {
        FUNC_DELAY_MS(100);

        if (WiFi.isConnected() == false) {
            DBG_OUT_I("Connecting to %s\r\n", CONFIG_WIFI_SSID);
            WiFi.begin(CONFIG_WIFI_SSID, CONFIG_WIFI_PASSWORD);
            uint32_t ms = FUNC_GET_TICK_MS();
            while (WiFi.status() != WL_CONNECTED) {
                FUNC_DELAY_MS(500);
                if ((FUNC_GET_TICK_MS() - ms) >= 30000) {
                    break;
                }
            }

            if (WiFi.isConnected() == false) {
                DBG_OUT_E("can't connect to %s\r\n", CONFIG_WIFI_SSID);
            } else {
                DBG_OUT_I("WiFi connected. IP address: %s\r\n", WiFi.localIP());
                g_dateTime.syncNTP((char *)CONFIG_NTP_SERVER);
            }
        } else {
            // dateTime_t dt;
            // g_dateTime.getDateTime(&dt);
            // DBG_OUT_I("Date Time: %02d/%02d/%04d - %02d:%02d:%02d\r\n",
            //         dt.day, dt.month, dt.year, dt.hour, dt.minute, dt.second);

        }

        FUNC_DELAY_MS(5000);
    }
}
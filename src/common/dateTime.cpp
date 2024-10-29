
#include <WiFi.h>
#include "time.h"
#include "dateTime.h"

dateTime g_dateTime;


bool dateTime::syncNTP(char *server)
{
    daylightOffset_sec = CONFIG_TIMEZONE * 60 * 60;
    configTime(gmtOffset_sec, daylightOffset_sec, server);
    return true;
}


bool dateTime::getDateTime(dateTime_t *dt)
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return false;
    }

    dt->hour = timeinfo.tm_hour;
    dt->minute = timeinfo.tm_min;
    dt->second = timeinfo.tm_sec;
    dt->day = timeinfo.tm_mday;
    dt->month = timeinfo.tm_mon + 1;
    dt->year = timeinfo.tm_year + 1900;
    return true;
}
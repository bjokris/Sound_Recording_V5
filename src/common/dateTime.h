
#ifndef __DATETIME_H
#define __DATETIME_H

#include "app_cfg.h"

typedef struct {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t day;
    uint8_t month;
    uint16_t year;
} dateTime_t;

class dateTime {
public:
    bool syncNTP(char *server);

    bool getDateTime(dateTime_t *dt);

private:
    long  gmtOffset_sec = 0;
    int   daylightOffset_sec = 0;
};

extern dateTime g_dateTime;

#endif // __DATETIME_H
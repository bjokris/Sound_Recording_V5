
#ifndef __SEEKTABLE_H
#define __SEEKTABLE_H

#include "app_cfg.h"

typedef struct {
    bool warningAlarm_triggered;
    /* led's default state be can configured when initializing table or in do_calc funciton */
    bool ledRed_isOn;   /* configure State of led when warning/alarm is triggered */
    bool ledGreen_isOn; /* configure State of led when warning/alarm is triggered */
    bool ledYellow_isOn;/* configure State of led when warning/alarm is triggered */
    float magnitude;
    uint8_t dataToSend[64];
    uint16_t dataToSend_len;
    float peak;
} seek_data_t;

typedef struct {
    void (*func_do_calc) (seek_data_t *seek_data, uint8_t *array, uint32_t size);
    void (*func_warningAlarm) (seek_data_t *seek_data);
    bool (*func_sendToLora) (seek_data_t *seek_data);
} seek_func_t;

typedef struct {
    seek_data_t data;
    seek_func_t func;
} seekTable_t;

/* seek index. number of seek depends on seekn.cpp seekn.h files */
enum {
    ENUM_SEEK_0_INDEX = 0,
    ENUM_SEEK_1_INDEX,
    ENUM_SEEK_2_INDEX,
    ENUM_SEEK_MAX_INDEX,
};

void seekTable_init(void);

extern seekTable_t g_seekTable[ENUM_SEEK_MAX_INDEX];

#endif // __SEEKTABLE_H
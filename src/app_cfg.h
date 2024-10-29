
#ifndef __APP_CFG_H
#define __APP_CFG_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <esp_task_wdt.h>

#if CONFIG_USE_SERIAL_DEBUG == true
#define CONFIG_LOG_TAG(name)            static const char *const_TAG = #name ;
#define DBG_OUT(msg,...)			    CONFIG_DEBUG_PORT.printf("---> [%s-%d]: " msg, const_TAG, __LINE__, ##__VA_ARGS__)
#define DBG_OUT_RAW						CONFIG_DEBUG_PORT.printf
#define DBG_OUT_E(msg,...)			    CONFIG_DEBUG_PORT.printf("\033[0;31m---> [%s-%d]: " msg "\033[0m", const_TAG, __LINE__, ##__VA_ARGS__)
#define DBG_OUT_W(msg,...)              CONFIG_DEBUG_PORT.printf("\033[0;33m---> [%s-%d]: " msg "\033[0m", const_TAG, __LINE__, ##__VA_ARGS__)
#define DBG_OUT_I(msg,...)              CONFIG_DEBUG_PORT.printf("\033[0;32m---> [%s-%d]: " msg "\033[0m", const_TAG, __LINE__, ##__VA_ARGS__)
#define DBG_OUT_RAW_I(msg,...)          CONFIG_DEBUG_PORT.printf("\033[0;32m" msg "\033[0m", ##__VA_ARGS__)
#else
#define CONFIG_LOG_TAG(name)
#define DBG_OUT(...)
#define DBG_OUT_RAW(...)
#define DBG_OUT_E(...)
#define DBG_OUT_W(...)
#define DBG_OUT_I(...)
#define DBG_OUT_RAW_I(...)
#endif // #if CONFIG_USE_SERIAL_DEBUG == true

#define FUNC_GET_TICK_MS()              ((int64_t)xTaskGetTickCount() * portTICK_PERIOD_MS) //int64_t
#define FUNC_DELAY_MS(ms)               delay(ms)
#define FUNC_TASK_DELAY_MS(ms)          vTaskDelay(ms / portTICK_PERIOD_MS)

#endif // __APP_CFG_H
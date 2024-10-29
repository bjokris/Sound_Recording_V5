
#include <SPI.h>
#include "app_cfg.h"
#include "driver/sdcard.h"
#include "driver/appI2S.h"
#include "lora/lora_sx1262.h"
#include "common/dateTime.h"

#include "task/cli_task.h"
#include "task/net_task.h"
#include "task/rx_task.h"
#include "task/tx_task.h"

CONFIG_LOG_TAG(MAIN)

SPIClass *g_spi_1 = NULL;
SPIClass *g_spi_2 = NULL;


void setup() {
    CONFIG_DEBUG_PORT.begin(115200);
    FUNC_DELAY_MS(2000);
    DBG_OUT_RAW_I("\r\n********* Sound Recording v%s *********\r\n", CONFIG_FW_VERSION);
    DBG_OUT_RAW_I("Flash size: %dMB flash. \r\n\r\n\033[0m", ESP.getFlashChipSize() / (1024 * 1024));

    /* init SPI */
    g_spi_1 = new SPIClass(FSPI);
    g_spi_2 = new SPIClass(HSPI);
    g_spi_1->begin(CONFIG_SDCARD_CLK_PIN, CONFIG_SDCARD_MISO_PIN,
            CONFIG_SDCARD_MOSI_PIN);
    g_spi_2->begin(CONFIG_LORA_SPI_CLK_PIN, CONFIG_LORA_SPI_MISO_PIN,
            CONFIG_LORA_SPI_MOSI_PIN);

    /* init SD Card */
    sdcard_init(CONFIG_SDCARD_CS_PIN, *g_spi_1);

    /* init lora */
    g_lora_sx1262.init(*g_spi_2);

    /* init I2S-PDM mic */
    appI2S_init();
    appI2S_stop();

    /* create tasks */
    xTaskCreate(net_task    , "net_task"        , (8  * 1024)   , NULL  , 5, NULL);
#if CONFIG_MODE_RX == false
    xTaskCreate(tx_task     , "tx_task"         , (8  * 1024)   , NULL  , 5, NULL);
#else
    xTaskCreate(rx_task     , "rx_task"         , (8  * 1024)   , NULL  , 5, NULL);
#endif // #if CONFIG_MODE_RX == false
#if (CONFIG_CLI_ENABLED == 1)
    xTaskCreate(cliTask     , "cliTask"         , (4 * 1024)    , NULL  , 5, NULL);
#endif // CONFIG_CLI_ENABLED

    FUNC_DELAY_MS(1000);
    DBG_OUT("Total heap: %d\r\n", ESP.getHeapSize());
    DBG_OUT("Free heap: %d\r\n", ESP.getFreeHeap());
    DBG_OUT("Total PSRAM: %d\r\n", ESP.getPsramSize());
    DBG_OUT("Free PSRAM: %d\r\n", ESP.getFreePsram());
}


void loop() {
    vTaskDelete(NULL);
    FUNC_DELAY_MS(5000);
}


#include "lora_sx1262.h"
#include <RadioLib.h>

CONFIG_LOG_TAG(LORA_SX1262)
lora_sx1262 g_lora_sx1262;
static SX1262 *g_radio_sx1262 = nullptr;
static Module *g_radio_module = nullptr;


ICACHE_RAM_ATTR void lora_sx1262_setFlag(void)
{
    g_lora_sx1262.rx_isAvailable = true;
}


bool lora_sx1262::init(SPIClass &spi)
{
    g_radio_module = new Module(CONFIG_LORA_CS_PIN_PIN,
                                CONFIG_LORA_IRQ_D1_PIN,
                                CONFIG_LORA_RESET_PIN,
                                CONFIG_LORA_BUSY_PIN,
                                spi);
    g_radio_sx1262 = new SX1262(g_radio_module);
    int state = g_radio_sx1262->begin();
    if (state != RADIOLIB_ERR_NONE) {
        DBG_OUT_E("failed to init sx1262\r\n");
        return false;
    }

    // set carrier frequency
    if (g_radio_sx1262->setFrequency(CONFIG_LORA_CFG_FREQ_HZ) == RADIOLIB_ERR_INVALID_FREQUENCY) {
        DBG_OUT_E("Selected frequency is invalid for this module!\r\n");
        return false;
    }

    // set bandwidth
    if (g_radio_sx1262->setBandwidth(CONFIG_LORA_CFG_BANDWIDTH) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
        DBG_OUT_E("Selected bandwidth is invalid for this module!\r\n");
        return false;
    }

    // set spreading factor
    if (g_radio_sx1262->setSpreadingFactor(CONFIG_LORA_CFG_SPREADING_FACTOR)
        == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
        DBG_OUT_E("Selected spreading factor is invalid for this module!\r\n");
        return false;
    }

    // set coding rate
    if (g_radio_sx1262->setCodingRate(CONFIG_LORA_CFG_CODING_RATE) == RADIOLIB_ERR_INVALID_CODING_RATE) {
        DBG_OUT_E("Selected coding rate is invalid for this module!\r\n");
        return false;
    }

    // set output power (accepted range is -9 - 22 dBm)
    if (g_radio_sx1262->setOutputPower(CONFIG_LORA_CFG_TX_POWER) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
        DBG_OUT_E("Selected output power is invalid for this module!\r\n");
        return false;
    }

    g_radio_sx1262->setSyncWord(0xAB);
    g_radio_sx1262->setPreambleLength(8);
    g_radio_sx1262->setCRC(true);

    g_radio_sx1262->setPacketReceivedAction(lora_sx1262_setFlag);
    g_radio_sx1262->startReceive();
    return true;
}


/* return number of bytes received */
int lora_sx1262::getRxAvailable(void)
{
    if (g_lora_sx1262.rx_isAvailable == true) {
        int numBytes = g_radio_sx1262->getPacketLength();
        g_lora_sx1262.rx_isAvailable = false;
        return numBytes;
    }
    return 0;
}


/* return number of bytes received */
bool lora_sx1262::getRx(uint8_t *data, uint16_t data_size)
{
    // int len = 0;
    // int state = g_radio_sx1262->receive(data, data_size);
    // if (state == RADIOLIB_ERR_NONE) {
    //     len = g_radio_sx1262->getPacketLength();
    // } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    //     // DBG_OUT_E("rx timeout!\r\n");
    // } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
    //     DBG_OUT_E("RX CRC error!\r\n");
    // } else {
    //     DBG_OUT_E("RX failed, code %d\r\n", state);
    // }

    // return len;

    int state = g_radio_sx1262->readData(data, data_size);
    return (state == RADIOLIB_ERR_NONE);
}


bool lora_sx1262::send(uint8_t *data, uint16_t data_size)
{
    int ret = g_radio_sx1262->transmit(data, data_size);

    if (ret == RADIOLIB_ERR_NONE) {
        DBG_OUT_I("sent. dataRate: %f bps\r\n", g_radio_sx1262->getDataRate());
    } else if (ret == RADIOLIB_ERR_PACKET_TOO_LONG) {
        DBG_OUT_E("Package length is too long\r\n");
    } else if (ret == RADIOLIB_ERR_TX_TIMEOUT) {
        DBG_OUT_E("sending timeout\r\n");
    } else {
        DBG_OUT_E("sending got other error. %d\r\n", ret);
    }
    g_radio_sx1262->startReceive();
    return (ret == RADIOLIB_ERR_NONE);
}


int lora_sx1262::getRSSI(void)
{
    return g_radio_sx1262->getRSSI();
}


int lora_sx1262::getSNR(void)
{
    return g_radio_sx1262->getSNR();
}



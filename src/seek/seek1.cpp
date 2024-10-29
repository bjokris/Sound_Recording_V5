
#include "seek1.h"

CONFIG_LOG_TAG(SEEK1)

// Normalize the samples to mean 0 and std deviation 1
static void seek1_normalizeSamples(int16_t *samples, uint32_t samples_size,
        float *normalizedSamples, uint32_t normalizedSamples_len);
// Compute the crest factor of normalized samples
float seek1_computeCrestFactor(float *data, uint32_t data_len);


/* implement seek0 function to analysic array data */
void seek1_do_calc(seek_data_t *seek_data, uint8_t *array, uint32_t size)
{
    if (seek_data == nullptr) {
        DBG_OUT_I("Seek data is null\r\n");
        return;
    }

    seek_data->warningAlarm_triggered = false;

    uint32_t samples_size = size / sizeof(int16_t);
    float *normalizedSamples = (float *)ps_calloc(samples_size * sizeof(float), 1);
    if (normalizedSamples == nullptr) {
        DBG_OUT_E("Failed to allocate memory for Samples\r\n");
        return;
    }
    /* doing something here */
    seek1_normalizeSamples((int16_t *)array, samples_size,
                    normalizedSamples, samples_size);
    seek_data->magnitude = seek1_computeCrestFactor(normalizedSamples, samples_size);
    free(normalizedSamples);

    DBG_OUT_I("seek1 result: %f\r\n", seek_data->magnitude);
    DBG_OUT("doing something in seek1_do_calc function\r\n");

    if (seek_data->magnitude > 2.5) {
        seek_data->warningAlarm_triggered = true;
        DBG_OUT("Warning/Alarm triggered due to high crest factor.\r\n");
    } else {
        seek_data->warningAlarm_triggered = false;
    }
    /* if triggered warning/alrm. we can change default state of led or no need */
    seek_data->ledRed_isOn = true;
    seek_data->ledYellow_isOn = true;

    /* end */
}


/* check warning/alarm */
void seek1_checkWarningAlarm(seek_data_t *seek_data)
{
    if (seek_data == nullptr) {
        DBG_OUT_I("Seek data is null\r\n");
        return;
    }

    if (seek_data->warningAlarm_triggered == true) {
        /* doing something here if needed. */
        DBG_OUT("Warning/Alarm enabled. Set LED status");
        memset(seek_data->dataToSend, 0, sizeof(seek_data->dataToSend));
        uint8_t newData[] = {1, 2, 3, 4, 5, 6};
        size_t newDataLen = sizeof(newData);
        if (newDataLen <= sizeof(seek_data->dataToSend)) {
            memcpy(seek_data->dataToSend, newData, newDataLen);
            seek_data->dataToSend_len = newDataLen;
        } else {
            DBG_OUT_E("Not enough space to add data to dataToSend\r\n");
        }

        /* create data to send via lora. below is a example */

        /* doing something here if needed */
    }
    /* doing something here if needed */
}


/* send data via lora */
bool seek1_sendToLora(seek_data_t *seek_data)
{
    if (seek_data == nullptr) {
        DBG_OUT_I("Seek data is null\r\n");
        return false;
    }

    /* send to lora */
    /* call lora sending function here */
    /* example:
    
        if (lora_send(seek_data.dataToSend, seek_data.dataToSend_len) == true) {
            DBG_OUT("send data success\r\n");
            return true;
        }
        DBG_OUT_E("Failed to send data via lora\r\n");
    */
   return false;
}


// Normalize the samples to mean 0 and std deviation 1
static void seek1_normalizeSamples(int16_t *samples, uint32_t samples_size,
        float *normalizedSamples, uint32_t normalizedSamples_len)
{
    float sum = 0.0f;
    for (uint32_t i = 0; i < samples_size; i++) {
        sum += samples[i];
    }

    float mean = sum / samples_size;
    float sumSq = 0.0f;
    for (uint32_t i = 0; i < samples_size; i++) {
        sumSq += (samples[i] - mean) * (samples[i] - mean);
    }

    float stddev = std::sqrt(sumSq / samples_size);
    for (uint32_t i = 0; i < samples_size && i < normalizedSamples_len; i++) {
        normalizedSamples[i] = (samples[i] - mean) / stddev;
    }
}


// Compute the crest factor of normalized samples
float seek1_computeCrestFactor(float *data, uint32_t data_len)
{
    float peak = 0.0f;
    float sumSq = 0.0f;

    for (size_t i = 0; i < data_len; i++) {
        float absSample = std::fabs(data[i]);
        peak = std::max(peak, absSample);
        sumSq += data[i] * data[i];
    }
    return peak / std::sqrt(sumSq / data_len);
}

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include "seek0.h"

CONFIG_LOG_TAG(SEEK0)

static float seek0_computeMean(int16_t *samples, uint32_t samples_size);
float seek0_computeStdDev(int16_t *samples, uint32_t samples_size, float mean);
// Normalize the samples to mean 0 and std deviation 1
static void seek0_normalizeSamples(int16_t *samples, uint32_t samples_size,
        float *normalizedSamples, uint32_t normalizedSamples_len);
// Compute the crest factor of normalized samples
float seek0_computeCrestFactor(float *data, uint32_t data_len);


/* implement seek0 function to analysic array data */
void seek0_do_calc(seek_data_t *seek_data, uint8_t *array, uint32_t size)
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
    seek0_normalizeSamples((int16_t *)array, samples_size,
                    normalizedSamples, samples_size);
    seek_data->magnitude = seek0_computeCrestFactor(normalizedSamples, samples_size);
    free(normalizedSamples);

    DBG_OUT_I("seek0 result: %f\r\n", seek_data->magnitude);
    DBG_OUT("doing something in seek0_do_calc function\r\n");
    seek_data->warningAlarm_triggered = true;
    /* if triggered warning/alrm. we can change default state of led or no need */
    seek_data->ledGreen_isOn = true;

    /* end */
}


/* check warning/alarm */
void seek0_checkWarningAlarm(seek_data_t *seek_data)
{
    if (seek_data == nullptr) {
        DBG_OUT_I("Seek data is null\r\n");
        return;
    }

    if (seek_data->warningAlarm_triggered == true) {
        /* doing something here if needed. below is example */
        DBG_OUT("Warning/Alarm enabled. set led Status\r\n");

        /* doing something here if needed */
    }
    /* doing something here if needed */
}


/* send data via lora */
bool seek0_sendToLora(seek_data_t *seek_data)
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


static float seek0_computeMean(int16_t *samples, uint32_t samples_size)
{
    int32_t sum = 0;
    for (uint32_t i = 0; i < samples_size; i++) {
        sum += samples[i];
    }
    return ((float)sum / samples_size);
}


float seek0_computeStdDev(int16_t *samples, uint32_t samples_size, float mean)
{
    float sum = 0.0;
    for (uint32_t i = 0; i < samples_size; i++) {
        float diff = samples[i] - mean;
        sum += diff * diff;
    }
    return sqrt(sum / samples_size);
}


// Normalize the samples to mean 0 and std deviation 1
static void seek0_normalizeSamples(int16_t *samples, uint32_t samples_size,
        float *normalizedSamples, uint32_t normalizedSamples_len)
{
    float mean = seek0_computeMean(samples, samples_size);
    float stddev = seek0_computeStdDev(samples, samples_size, mean);

    for (uint32_t i = 0; i < samples_size && i < normalizedSamples_len; i++) {
        normalizedSamples[i] = (((float)samples[i] - mean) / stddev);
    }
}


// Compute the crest factor of normalized samples
float seek0_computeCrestFactor(float *data, uint32_t data_len)
{
    float peak = fabs(data[0]);
    float sumSq = data[0] * data[0];

    for (size_t i = 1; i < data_len; ++i) {
        float absData = fabs(data[i]);
        if (absData > peak) {
            peak = absData;
        }
        sumSq += data[i] * data[i];
    }

    float rms = sqrt(sumSq / data_len);
    return peak / rms;
}
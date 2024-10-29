
#include "recordWav.h"
#include "appI2S.h"

CONFIG_LOG_TAG(RECORDWAV)


void audio_output_task(void *pArg);


int recordWav_generateWAV_header(uint8_t *header, int wav_size)
{
    int header_len = 0;
    unsigned int fileSize = wav_size + CONFIG_WAV_HEADER_SIZE - 8;

    // chunk ID
    header[header_len++] = 'R';
    header[header_len++] = 'I';
    header[header_len++] = 'F';
    header[header_len++] = 'F';
    // Chunk size
    header[header_len++] = (byte)(fileSize & 0xFF);
    header[header_len++] = (byte)((fileSize >> 8) & 0xFF);
    header[header_len++] = (byte)((fileSize >> 16) & 0xFF);
    header[header_len++] = (byte)((fileSize >> 24) & 0xFF);
    // format
    header[header_len++] = 'W';
    header[header_len++] = 'A';
    header[header_len++] = 'V';
    header[header_len++] = 'E';
    // Subchunk ID
    header[header_len++] = 'f';
    header[header_len++] = 'm';
    header[header_len++] = 't';
    header[header_len++] = ' ';
    // chunk1 size
    header[header_len++] = 0x10;
    header[header_len++] = 0x00;
    header[header_len++] = 0x00;
    header[header_len++] = 0x00;
    // audio format: 1 for PCM
    header[header_len++] = 0x01;
    header[header_len++] = 0x00;
    // audio channel: 1
    header[header_len++] = 0x01;
    header[header_len++] = 0x00;
    // sample rate: 48000
    header[header_len++] = 0x80;
    header[header_len++] = 0xBB;
    header[header_len++] = 0x00;
    header[header_len++] = 0x00;
    // byte rate = sampel rate * num channel * bitPersample / 8
    header[header_len++] = 0x00;
    header[header_len++] = 0x77;
    header[header_len++] = 0x01;
    header[header_len++] = 0x00;
    // BlockAlign = NumChannels * BitsPerSample/8
    header[header_len++] = 0x02;
    header[header_len++] = 0x00;
    // BitsPerSample: 8 bits = 8, 16 bits = 16, etc.
    header[header_len++] = 0x10;
    header[header_len++] = 0x00;

    header[header_len++] = 'd';
    header[header_len++] = 'a';
    header[header_len++] = 't';
    header[header_len++] = 'a';
    header[header_len++] = (uint8_t)(wav_size & 0xFF);
    header[header_len++] = (uint8_t)((wav_size >> 8) & 0xFF);
    header[header_len++] = (uint8_t)((wav_size >> 16) & 0xFF);
    header[header_len++] = (uint8_t)((wav_size >> 24) & 0xFF);

    return header_len;
}


void recordWav_start(void)
{
    uint8_t header[CONFIG_WAV_HEADER_SIZE + 1] = {0};

    int64_t ms = FUNC_GET_TICK_MS();
    DBG_OUT_I("\r\n *** Recording Start *** \r\n");
    int record_size = CONFIG_RECORD_1_SECOND_SIZE * 10;
    recordWav_generateWAV_header(header, record_size);
    CONFIG_DEBUG_PORT.write(header, CONFIG_WAV_HEADER_SIZE);

    int flash_wr_size = 0;
    char *i2s_read_buff = (char*)calloc(CONFIG_RECORD_FRAME + 1, sizeof(char));
    uint8_t *flash_write_buff = (uint8_t*)calloc(CONFIG_RECORD_FRAME + 1, sizeof(uint8_t));

    appI2S_start();
    appI2S_clear_buff();
    appI2S_read_data((void *)i2s_read_buff, CONFIG_RECORD_FRAME);
    appI2S_read_data((void *)i2s_read_buff, CONFIG_RECORD_FRAME);

    while (flash_wr_size < record_size) {
        int bytes_read = appI2S_read_data(i2s_read_buff, CONFIG_RECORD_FRAME);
        if (bytes_read > 0) {
            CONFIG_DEBUG_PORT.write(i2s_read_buff, bytes_read);
            flash_wr_size += bytes_read;
            // DBG_OUT_I("Sound recording: size %d - %u%%\r\n", flash_wr_size, flash_wr_size * 100 / record_size);
        }
    }

    appI2S_stop();

    free(i2s_read_buff);
    i2s_read_buff = nullptr;
    free(flash_write_buff);
    flash_write_buff = nullptr;

    DBG_OUT_I("\r\n *** Recording Done in %lld*** \r\n", FUNC_GET_TICK_MS() - ms);
}
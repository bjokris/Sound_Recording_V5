
#ifndef __SDCARD_H
#define __SDCARD_H

#include "app_cfg.h"
#include <SD.h>

void sdcard_init(uint8_t cs_pin, SPIClass &spi);

void sdcard_deinit(void);

/* path: path to file to read. start by '/' */
bool sdcard_openToWrie(File *file, const char *filePath);

void sdcard_closeFile(File file);

int sdcard_writeStream(File file, uint8_t *data, uint16_t write_len);

/* path: path to file to read. start by '/' */
bool sdcard_openToRead(File *file, const char *filePath);

int sdcard_readStream(File file, uint8_t *data, uint16_t read_len);

/* dir: direction to folder to get files. start by '/' */
void sdcard_listFile(const char *dir);

/* path: path to file to read. start by '/'
    data: buffer to store data reading from file
    read_len: maximum number of bytes read from file

    @return: <= 0 if error
             other: length of data read from file
*/
int sdcard_readFile(const char *path, uint8_t *data, uint32_t read_len);

/* path: path to file to write. start by '/'
    data: data to write to file
    write_len: length of data
    @ return: size of data wrote to file
*/
int sdcard_writeFile(const char *path, uint8_t *data, uint32_t write_len);

/* path: path to file to write. start by '/'
    data: data to write to file
    write_len: length of data
    @ return: size of data wrote to file
*/
int sdcard_writeFileAppend(const char *path, uint8_t *data, uint32_t write_len);

/* path: path to file to be deleted */
bool sdcard_deleteFile(const char *path);

/* path: path to file */
int sdcard_readFileSize(const char *path);

#endif // __SDCARD_H
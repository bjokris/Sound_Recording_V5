
#include "sdcard.h"

CONFIG_LOG_TAG(SDCARD)


void sdcard_init(uint8_t cs_pin, SPIClass &spi)
{
    if (!SD.begin(cs_pin, spi, 16000000)) {
        DBG_OUT_E("Card Mount Failed\r\n");
        return;
    }
    
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE){
        DBG_OUT_E("No SD card attached\r\n");
        return;
    }

    DBG_OUT("SD Card Type: %sr\r\n",
            (cardType == CARD_MMC)? "MMC" :
            (cardType == CARD_SD)? "SDSC" :
            (cardType == CARD_SDHC)? "SDHC" : "UNKNOWN"
            );

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    DBG_OUT("SD Card Size: %lluMB\r\n", cardSize);
}


void sdcard_deinit(void)
{
    SD.end();
}


/* dir: direction to folder to get files. start by '/' */
void sdcard_listFile(const char *dir)
{
    File root = SD.open(dir);
    if(!root){
        DBG_OUT_E("Failed to open directory\r\n");
        return;
    }
    if (!root.isDirectory()) {
        DBG_OUT_E("Not a directory\r\n");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            DBG_OUT_RAW("  DIRECTION : %s\r\n", file.name());
        } else {
            DBG_OUT_RAW("  FILE      : %s, size: %d\r\n", file.name(), file.size());
        }
        file = root.openNextFile();
    }
}


/* path: path to file to read. start by '/' */
bool sdcard_openToWrie(File *file, const char *filePath)
{
    *file = SD.open(filePath, FILE_WRITE);
    if (!file) {
        DBG_OUT_E("Failed to open file for writing\r\n");
        return false;
    }
    return true;
}


/* path: path to file to read. start by '/' */
bool sdcard_openToRead(File *file, const char *filePath)
{
    *file = SD.open(filePath, FILE_READ);
    if (!file) {
        DBG_OUT_E("Failed to open file for reading\r\n");
        return false;
    }
    return true;
}


void sdcard_closeFile(File file)
{
    file.close();
}


int sdcard_writeStream(File file, uint8_t *data, uint16_t write_len)
{
    return file.write(data, write_len);
}


int sdcard_readStream(File file, uint8_t *data, uint16_t read_len)
{
    return file.read(data, read_len);
}


/* path: path to file to read. start by '/'
    data: buffer to store data reading from file
    read_len: maximum number of bytes read from file

    @return: <= 0 if error
             other: length of data read from file
*/
int sdcard_readFile(const char *path, uint8_t *data, uint32_t read_len)
{
    File file = SD.open(path, FILE_READ);
    if (!file) {
        DBG_OUT_E("Failed to open file for reading\r\n");
        return -1;
    }

    int len = file.size();
    if (len > read_len) {
        len = read_len;
    }
    
    int rx = file.read(data, len);
    file.close();
    return rx;
}


/* path: path to file to write. start by '/'
    data: data to write to file
    write_len: length of data
    @ return: size of data wrote to file
*/
int sdcard_writeFile(const char *path, uint8_t *data, uint32_t write_len)
{
    File file = SD.open(path, FILE_WRITE);
    if (!file) {
        DBG_OUT_E("Failed to open file for writing\r\n");
        return -1;
    }
    int len = file.write(data, write_len);
    file.close();
    return len;
}


/* path: path to file to write. start by '/'
    data: data to write to file
    write_len: length of data
    @ return: size of data wrote to file
*/
int sdcard_writeFileAppend(const char *path, uint8_t *data, uint32_t write_len)
{
    File file = SD.open(path, FILE_APPEND);
    if (!file) {
        DBG_OUT_E("Failed to open file for writing\r\n");
        return -1;
    }
    int len = file.write(data, write_len);
    file.close();
    return len;
}


/* path: path to file to be deleted */
bool sdcard_deleteFile(const char *path)
{
    if (SD.remove(path)) {
        DBG_OUT("File deleted\r\n");
        return true;
    }
    DBG_OUT_E("Delete failed\r\n");
    return false;
}


/* path: path to file */
int sdcard_readFileSize(const char *path)
{
    File file = SD.open(path, FILE_READ);
    return file.size();
}
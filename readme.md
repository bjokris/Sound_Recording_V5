
# Firmware ESP32 for sound recording

## Hardware
- ESP32-S3
- PDM Mic
- Micro SDCard

## Firmware
- PlatformIO IDE
- Arduino framework
- ESP32 framework version 6.4.0

## CLI

1. Disconnect SDCard
    - Command: sd-dis

2. List file in directory from SDCard
    - Command: sd-list

3. Delete a file in SDCard
    - Command: sd-del &lt;file name&gt;
    - Note: file name has to start by '/'
    - example: sd-del /data.txt

4. Get file size from SDCard
    - Command: sd-size &lt;file name&gt;
    - Note: file name has to start by '/'
    - example: sd-size /data.txt

5. Start to record audio to file
    - Command: record &lt;file name&gt;
    - Note: file name has to start by '/'
    - example: record /audio.csv

6. Start to record audio to file with seek index
    - Command: recordseek &lt;file name&gt; &lt;seek index&gt;
    - Note: file name has to start by '/'. 
            seek index is from 0 to max seek (ENUM_SEEK_MAX_INDEX)
    - example: record /audio.csv 0

7. Start to record audio to wav file
    - Command: recordWav &lt;file name&gt;
    - Note: file name has to start by '/'
    - example: recordWav /audio.wav

## Lora message format

|   byte 0   |    byte 1    |     byte 2-3     |   byte 4    |   byte 5      |    byte 6     |   byte 7   |    byte 8-11       | byte 12-15   |
|------------|--------------|------------------|-------------|---------------|---------------|------------|--------------------|--------------|
| Day (1-31) | Month (1-12) | Year (2000-2099) | Hour (0-23) | Minute (0-59) | Second (0-59) | Seek index | Seek result number | Crest factor |

Example: 16 bytes from lora: 0x070907E810141E000100000034C55F40

      07      09      07E8      10      14      1E      00    01000000    34C55F40
    byte 0  byte 1  byte 2-3  byte 4  byte 5  byte 6  byte 7  byte 8-11  byte 12-15

    - Byte 0    : 0x07      => Day = 7
    - Byte 1    : 0x09      => Month = 9
    - Byte 2-3  : 0x07E8    => Year = (0x07 << 8) | 0xE8 = 2024
    - Byte 4    : 0x10      => Hour = 16
    - Byte 5    : 0x14      => Minute = 20
    - Byte 6    : 0x1E      => Second = 30
    - Byte 7    : 0x00      => Seek Index = 0
    - Byte 8-11 : 0x01000000=> 0x00000001=> Seek Result number = 1
    - Byte 12-15: 0x34C55F40=> 0x405FC534=> Crest Factor = 3.49641

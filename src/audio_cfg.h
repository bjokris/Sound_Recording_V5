
#ifndef __AUDIO_CFG_H
#define __AUDIO_CFG_H

#define CONFIG_WAV_HEADER_SIZE              44
#define CONFIG_RECORD_1_SECOND_SIZE         (CONFIG_MIC_SAMPLE_RATE * (CONFIG_MIC_SAMPLE_BIT / 8))
#define CONFIG_RECORD_FRAME                 (CONFIG_MIC_SAMPLE_RATE / 10)

#endif // __AUDIO_CFG_H
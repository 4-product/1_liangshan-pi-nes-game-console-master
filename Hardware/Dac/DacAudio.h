#ifndef DacAudio_H
#define DacAudio_H


#include "gd32f4xx.h"

void DacAudio_Init(void);  

//typedef void (*callback_fun)(uint8_t *pBuff);


void DacAudio_Play(u32 samplingRate);
void DacAudio_Play2(const uint8_t audioSouce[], u32 len,u32 samplingRate);
void DacAudio_Stop(void);
//extern const uint8_t dacAudio_AliPay[11722];
//extern const uint8_t dacAudio_AliPay2[120004];

#endif

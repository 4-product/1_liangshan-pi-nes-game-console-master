#ifndef Dac_H
#define Dac_H

#include "gd32f4xx.h"


extern uint8_t wavbuff[50];
extern uint16_t wavnumber;



void Dac_Init(void);
void key_timeinit(u32 samplingRate);
void keywav_init(uint8_t M);
void key_wav(void);



void key_wav_motor(void);



#endif

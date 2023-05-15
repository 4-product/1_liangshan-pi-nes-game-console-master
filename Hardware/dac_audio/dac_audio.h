#ifndef _DAC_AUDIO_H
#define _DAC_AUDIO_H

#include "gd32f4xx.h"
#include "systick.h"


/* PA5  ADC1 */
#define DAC_AUDIO_RCU  		RCU_GPIOA
#define DAC_AUDIO_PORT  	GPIOA
#define DAC_AUDIO_PIN    	GPIO_PIN_5 


void dac_audio_init(void);
void dac_audio_set_value(uint16_t dat);
void dac_audio_play(uint16_t *pdat,size_t size);

#endif

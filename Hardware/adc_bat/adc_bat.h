#ifndef _ADC_BAT_H
#define _ADC_BAT_H

#include "gd32f4xx.h"
#include "systick.h"


/* PC5   */
#define ADC_BAT_RCU  RCU_GPIOB
#define ADC_BAT_PORT  GPIOB
#define ADC_BAT_PIN    GPIO_PIN_1




void adc_bat_init(void);
uint8_t adc_bat_get_value(void);


#endif

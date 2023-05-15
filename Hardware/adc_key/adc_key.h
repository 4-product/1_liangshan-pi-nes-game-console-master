#ifndef _ADC_KEY_H
#define _ADC_KEY_H

#include "gd32f4xx.h"
#include "systick.h"


/* PC4   */
#define ADC_RCU  RCU_GPIOC
#define ADC_PORT  GPIOC
#define ADC_PIN    GPIO_PIN_4

#define ADCX_RCU    RCU_GPIOC
#define ADCX_PORT   GPIOC
#define ADCX_PIN    GPIO_PIN_1

#define ADCY_RCU    RCU_GPIOA
#define ADCY_PORT   GPIOA
#define ADCY_PIN    GPIO_PIN_1


//left right up down
#define adckey_left_right   0xc0
#define adckey_left         0x40
#define adckey_right        0x80
#define adckey_up_down      0x30
#define adckey_up           0x10
#define adckey_down         0x20
#define adckey_null         0


//简易按键结构体
typedef struct  
{
    uint8_t     last_adckey;           //上次
    uint8_t     now_adckey;            //当前
}adckey_ui;
extern adckey_ui ui_adckey;




void adc_config(void);
uint16_t adc_channel_sample(uint8_t channel);
uint8_t five_way_key_scan(void);


#endif

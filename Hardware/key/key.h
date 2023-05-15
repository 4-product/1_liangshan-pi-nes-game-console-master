#ifndef _KEY_H
#define _KEY_H

#include "gd32f4xx.h"
#include "systick.h"

#define KEYL_RCU  RCU_GPIOA
#define KEYL_PORT  GPIOA
#define KEYL_PIN    GPIO_PIN_0

#define KEYR_RCU  RCU_GPIOB
#define KEYR_PORT  GPIOB
#define KEYR_PIN    GPIO_PIN_2

#define KEYA_RCU  RCU_GPIOG
#define KEYA_PORT  GPIOG
#define KEYA_PIN    GPIO_PIN_9

#define KEYB_RCU  RCU_GPIOB
#define KEYB_PORT  GPIOB
#define KEYB_PIN    GPIO_PIN_15



#define key_enter   0x03
#define key_exit    0x0c
#define key_null    0


//简易按键结构体
typedef struct  
{
    uint8_t     last_key;           //上次
    uint8_t     now_key;            //当前
}key_ui;
extern key_ui ui_key;


void key_init(void);
//void key_get_value(uint32_t port,uint32_t pin);
//extern one_key_enum get_key_val();
u8 get_key_val(void);


#endif


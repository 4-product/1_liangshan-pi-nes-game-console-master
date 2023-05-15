#ifndef _BEEP_H
#define _BEEP_H

#include "gd32f4xx.h"
#include "systick.h"


/* PA15  定时器1的通道0 */
#define BEEP_RCU  RCU_GPIOA
#define BEEP_PORT  GPIOA
#define BEEP_PIN    GPIO_PIN_15 

void beep_init(void);
void timer_config(void);


#endif

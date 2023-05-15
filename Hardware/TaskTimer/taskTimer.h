#ifndef _TASKTIMER_H
#define _TASKTIMER_H

#include "gd32f4xx.h"
#include "systick.h"



void taskTimer_config(uint32_t TIM_Period);
uint8_t  SysGetSignal_1ms(uint16_t nms,uint8_t index);


#endif

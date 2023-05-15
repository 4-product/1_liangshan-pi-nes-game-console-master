#ifndef __RTC_H
#define __RTC_H

#include "gd32f4xx.h"

#define RTC_CLOCK_SOURCE_LXTAL
#define BKP_VALUE    0x32F0
#define EVAL_COM0    USART0

extern rtc_parameter_struct   rtc_initpara;


uint8_t usart_input_threshold(uint32_t value);

void rtc_setup(void);
void rtc_show_time(void);
void rtc_show_alarm(void);

void rtc_pre_config(void);

void rtctime_init(void);


#endif

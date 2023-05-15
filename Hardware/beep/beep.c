#include "beep.h"

void beep_init(void)
{
		/* enable the clock */
    rcu_periph_clock_enable(BEEP_RCU);
    /* configure GPIO port */ 
    gpio_mode_set(BEEP_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE,BEEP_PIN);
    gpio_output_options_set(BEEP_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,BEEP_PIN);
		/* configure GPIO AF	*/ 
	  gpio_af_set(BEEP_PORT, GPIO_AF_1, BEEP_PIN); 
}


void timer_config(void)
{
	/*
	  TIMER1CLK = SystemCoreClock / 200 = 1MHz
	  TIMER1 channel0 duty cycle = (8000/ 16000)* 100  = 50%

	eg:
		T  = prescaler * period / 200M  = 199 * 15999 / 200 000ms = 16ms
		f = 1 / 16 = 0.0625 khz = 62.5hz
	
		T  = prescaler * period / 200M  = 19 * 15999 / 200 000ms = 1.6ms
		f = 1 / 1.6 = 0.625 khz = 625hz
	
	
	 蜂鸣器工作频率1.5khz - 2.5khz  取2khz = 0.5ms  定时器需500us
	*/
	  timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
	
	  rcu_periph_clock_enable(RCU_TIMER1);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(TIMER1);
	
	 /* TIMER1 configuration */
    timer_initpara.prescaler         = 199;  // 1MHZ
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 499;  // 500 * 1us = 0.5ms   = 2khz
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1,&timer_initpara);

		/* CH0 configuration in PWM mode */
		timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
		
		timer_channel_output_config(TIMER1,TIMER_CH_0,&timer_ocintpara);
	
	 /* CH0 configuration in PWM mode1,
				duty cycle 0%      0
				duty cycle 25% 		125
				duty cycle 50%    249
				duty cycle 75% 		375
				duty cycle 100% 	499			
	 */
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,0);  // 
    timer_channel_output_mode_config(TIMER1,TIMER_CH_0,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER1,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);
		
	  /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER1);
    /* auto-reload preload enable */
    timer_enable(TIMER1);
}

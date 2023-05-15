
#include "DacAudio.h"
#include "cw_queue.h"

extern cw_cycle_queue t_queue;

void DacAudio_Init()
{

    nvic_irq_enable(TIMER4_IRQn, 1, 2);
    rcu_periph_clock_enable(RCU_TIMER4);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_deinit(TIMER4);

    /* TIMER4 configuration */
    timer_parameter_struct timer_initpara;
    timer_initpara.prescaler = 400 - 1;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = (40000) - 1;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    // timer_initpara.repetitioncounter = 0;
    timer_init(TIMER4, &timer_initpara);

    timer_interrupt_enable(TIMER4, TIMER_INT_UP);
    timer_enable(TIMER4);
}

#define time_Frequency 400000000 
#define time_Prescaler 4 

u32 dacAudio_Index = 0;
u32 dacAudio_Len = 0 ;
uint8_t *dacAudioSouce ;
u8 dacAudio_Volume=255;

//static callback_fun finish_call_fun = NULL;
void DacAudio_Play(u32 samplingRate)
{
    nvic_irq_enable(TIMER4_IRQn, 1, 2);
    rcu_periph_clock_enable(RCU_TIMER4);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_deinit(TIMER4);
    
    
	printf("samplingRate:%d\r\n",samplingRate);
    /* TIMER4 configuration */ 
    timer_parameter_struct timer_initpara;
    timer_initpara.prescaler = time_Prescaler - 1;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = (time_Frequency/time_Prescaler/samplingRate/2) - 1;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    // timer_initpara.repetitioncounter = 0;
    
    timer_init(TIMER4, &timer_initpara);
    timer_interrupt_enable(TIMER4, TIMER_INT_UP);
	timer_enable(TIMER4);
	
}
void DacAudio_Play2(const uint8_t audioSouce[], u32 len,u32 samplingRate)
{

    
    /* TIMER4 configuration */ 
    timer_parameter_struct timer_initpara;
    timer_initpara.prescaler = time_Prescaler - 1;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = (time_Frequency/time_Prescaler/samplingRate) - 1;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    // timer_initpara.repetitioncounter = 0;
    timer_init(TIMER4, &timer_initpara);
    
    //dacAudioSouce = audioSouce;
    dacAudio_Len=len;
    dacAudio_Index=0;
	timer_enable(TIMER4);
	
}
void DacAudio_Stop()
{
    dacAudioSouce=NULL;
}

void TIMER4_IRQHandler(void)
{
	int ret = 0;
	uint8_t data = 0;
	ret = cw_queue_read(&t_queue,&data,1,0);
	if(ret)
	{
		dac_data_set(DAC1, DAC_ALIGN_8B_R,data);
        dac_software_trigger_enable(DAC1);
	}
	//printf("irq:%d\r\n",ret);
    /* clear TIMER interrupt flag */
    timer_interrupt_flag_clear(TIMER4, TIMER_INT_FLAG_UP);
}

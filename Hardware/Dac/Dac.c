#include "Dac.h"
#include "bsp_pwm.h"

//extern uint8_t setmake[7];

/////按键音 半波取模
const uint8_t wavdata[25]={0 ,16 ,32 ,47 ,62 ,75 ,88 ,99 ,108 ,116 ,122 ,126 ,128 ,128 ,126 ,122 ,116 ,108 ,99 ,88 ,75 ,62 ,47 ,32 ,16 };
uint8_t wavbuff[50];

uint16_t wavnumber;

/* PA5  ADC1 */
#define DAC_AUDIO_RCU  		RCU_GPIOA
#define DAC_AUDIO_PORT  	GPIOA
#define DAC_AUDIO_PIN    	GPIO_PIN_5 


void Dac_Init()
{ /* enable the clock of peripherals */
    rcu_periph_clock_enable(DAC_AUDIO_RCU);
    rcu_periph_clock_enable(RCU_DAC);

    /* once enabled the DAC, the corresponding GPIO pin is connected to the DAC converter automatically */
    gpio_mode_set(DAC_AUDIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, DAC_AUDIO_PIN);

    dac_deinit();                                          // 复位DAC
                                                           /* configure the DAC1 */
                                                           // dac_trigger_source_config(DAC1, DAC_TRIGGER_T3_TRGO); // 使用定时器3作为触发源
    dac_trigger_source_config(DAC1, DAC_TRIGGER_SOFTWARE); // 使用定时器3作为触发源
    dac_trigger_enable(DAC1);

    dac_wave_mode_config(DAC1, DAC_WAVE_DISABLE); // 不使能波形
                                                  //	dac_wave_mode_config(DAC1, DAC_WAVE_MODE_LFSR); //
                                                  // dac_lfsr_noise_config(DAC1, DAC_LFSR_BITS11_0);
    dac_output_buffer_enable(DAC1);

    /* enable DAC1 and set data */
    dac_enable(DAC1); // 使能DAC

    dac_data_set(DAC1, DAC_ALIGN_8B_R, 0x80); // 8位数据右对齐
    dac_software_trigger_enable(DAC1);     // 软件触发使能 输出数据

    
}


void key_timeinit(u32 samplingRate)
{
    nvic_irq_enable(TIMER3_IRQn, 1, 2);
    rcu_periph_clock_enable(RCU_TIMER3);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_deinit(TIMER3);
    
    /* TIMER4 configuration */ 
    timer_parameter_struct timer_initpara;
    timer_initpara.prescaler = 3;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = (100000000/samplingRate/2) - 1;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    // timer_initpara.repetitioncounter = 0;
    
    timer_init(TIMER3, &timer_initpara);
    timer_interrupt_enable(TIMER3, TIMER_INT_UP);
	timer_enable(TIMER3);
}


void Delay_Nop(u32 N)
{
    while(N--) __nop();
}


void keywav_init(uint8_t M)
{
    uint8_t i;
    
    if(M)
    {
        M = 6-M;
        for(i=0;i<25;i++) wavbuff[i] = 0x80+(wavdata[i]>>M);
        for(i=0;i<25;i++) wavbuff[25+i] = 0x80-(wavdata[i]>>M);
    }
    else
    {
        for(i=0;i<50;i++) wavbuff[i] = 0x80;
    }
}

void key_wav(void)
{
    uint8_t i,j;
    
    for(j=0;j<25;j++)
    {
        for(i=0;i<50;i++)
        {
            dac_data_set(DAC1, DAC_ALIGN_8B_R, wavbuff[i]);
            dac_software_trigger_enable(DAC1);
            Delay_Nop(1200);
        }
    }
    dac_data_set(DAC1, DAC_ALIGN_8B_R, 0x80);
    dac_software_trigger_enable(DAC1);
}



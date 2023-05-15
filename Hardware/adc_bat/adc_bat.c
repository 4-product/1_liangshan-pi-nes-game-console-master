#include "adc_bat.h"
#include "adc_key.h"
#include "stdio.h"

//#define DEBUG   // 打开这个将会使能adc_key下面的打印信息

//电压等级 01234
const uint16_t BATDATA[6] = {2600,3000,3400,3600,3800,4000};


/* ADC的附加功能 优先于普通GPIO  */
void adc_bat_init(void)
{
//		/* enable the clock */
    rcu_periph_clock_enable(ADC_BAT_RCU);
	
    /* configure GPIO port 附加功能需要配置为 GPIO_MODE_ANALOG */ 
    gpio_mode_set(ADC_BAT_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADC_BAT_PIN);
    
}


/* 获取值函数  默认3.3V基准稳定  */
uint8_t adc_bat_get_value(void)
{
    uint8_t i;
	uint16_t adcValue=0;
    
    for(i=0;i<16;i++)
    {
        adcValue += adc_channel_sample(ADC_CHANNEL_9);  // 采样15通道 
    }
    adcValue = (adcValue>>4)*3300/2048;  //分压电阻/2
    printf("adc_photosensitive value is :%d \r\n",adcValue);
    
    for(i=0;i<5;i++)
    {
        if(BATDATA[i] > adcValue) break;
    }
    return i;
    
//#ifdef DEBUG
	
//#endif
}

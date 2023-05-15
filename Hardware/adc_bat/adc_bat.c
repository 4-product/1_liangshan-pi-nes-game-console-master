#include "adc_bat.h"
#include "adc_key.h"
#include "stdio.h"

//#define DEBUG   // ���������ʹ��adc_key����Ĵ�ӡ��Ϣ

//��ѹ�ȼ� 01234
const uint16_t BATDATA[6] = {2600,3000,3400,3600,3800,4000};


/* ADC�ĸ��ӹ��� ��������ͨGPIO  */
void adc_bat_init(void)
{
//		/* enable the clock */
    rcu_periph_clock_enable(ADC_BAT_RCU);
	
    /* configure GPIO port ���ӹ�����Ҫ����Ϊ GPIO_MODE_ANALOG */ 
    gpio_mode_set(ADC_BAT_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADC_BAT_PIN);
    
}


/* ��ȡֵ����  Ĭ��3.3V��׼�ȶ�  */
uint8_t adc_bat_get_value(void)
{
    uint8_t i;
	uint16_t adcValue=0;
    
    for(i=0;i<16;i++)
    {
        adcValue += adc_channel_sample(ADC_CHANNEL_9);  // ����15ͨ�� 
    }
    adcValue = (adcValue>>4)*3300/2048;  //��ѹ����/2
    printf("adc_photosensitive value is :%d \r\n",adcValue);
    
    for(i=0;i<5;i++)
    {
        if(BATDATA[i] > adcValue) break;
    }
    return i;
    
//#ifdef DEBUG
	
//#endif
}

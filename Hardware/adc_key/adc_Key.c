#include "adc_key.h"
#include "stdio.h"

#define DEBUG // 打开这个将会使能adc_key下面的打印信息


adckey_ui ui_adckey;




/* 五向开关的值  ADC12位 最大值为4095
默认值 																							 8
A:100R-10K  		10K*(100R+10K)*4095 = 4054.5   实际  4028   可操作范围  4000-4095
B:10K-10K     	10K*(10K+10K)*4095 = 2047.5					 2033								2000-2050
C:100K-10K     	10K*(100K+10K)*4095 = 372.3					 372								320-400
D:43K-10K       10K*(43K+10K)*4095 = 772.6					 770								730-820
center:560K-10K 10K*(560K+10K)*4095 = 71.8           80								  50-120
*/

/* ADC的附加功能 优先于普通GPIO  */
static void adc_init(void)
{

    /* enable the clock */
    rcu_periph_clock_enable(ADCX_RCU);
    /* configure GPIO port 附加功能需要配置为 GPIO_MODE_ANALOG */ 
    gpio_mode_set(ADCX_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADCX_PIN);
    /* enable the clock */
    rcu_periph_clock_enable(ADCY_RCU);
    /* configure GPIO port 附加功能需要配置为 GPIO_MODE_ANALOG */ 
    gpio_mode_set(ADCY_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADCY_PIN);
    
    
    
}

void adc_config(void)
{
  
        /* enable ADC0 clock */
    rcu_periph_clock_enable(RCU_ADC0);
        /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8);
        /* reset ADC */
        adc_deinit();
      /* configure the ADC  mode */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT); // 所有ADC都工作在独立模式

    /* ADC contineous function disable */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE); // 关闭连续模式
    /* ADC scan mode disable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE); // 关闭扫描模式

        /* ADC data alignment config */
    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT); // LSB对齐，低位对齐

    /* ADC channel length config */
    adc_channel_length_config(ADC0,ADC_REGULAR_CHANNEL,1U); //  ADC规则通道 长度为1
    /* enable ADC interface */
    adc_enable(ADC0);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);  // ADC校准
    /* wait for ADC stability */
    delay_1ms(10);
    
    adc_init();
}

/*!
    \brief      ADC channel sample  ADC通道采样
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint16_t adc_channel_sample(uint8_t channel)
{
  /* ADC regular channel config */
  adc_regular_channel_config(ADC0, 0U, channel, ADC_SAMPLETIME_15); // 15个采样周期
  /* ADC software trigger enable */
  adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL); // ADC软件触发使能

  /* wait the end of conversion flag */
  while (!adc_flag_get(ADC0, ADC_FLAG_EOC))
    ;
  /* clear the end of conversion flag */
  adc_flag_clear(ADC0, ADC_FLAG_EOC);
  /* return regular channel sample value */
  return (adc_regular_data_read(ADC0));
}

/* 五向按键扫描函数 */
uint8_t five_way_key_scan(void)
{
    uint16_t 	adcXValue,adcYValue;
//	//右128 左64 下32 上16 Start 8 Select 4 B 2 A0
//	uint16_t five_key_Val[]={0,16,32,64,128};
    u8 keyd=0;
    
    adcXValue = adc_channel_sample(ADC_CHANNEL_1);  // 采样
    adcYValue = adc_channel_sample(ADC_CHANNEL_11);  // 采样
    
    if (adcYValue >= 3000 )
    { // 
        keyd |= 0x20;
    }
    else if ( adcYValue <= 1000)
    { // 
        keyd |= 0x10;
    }
    else;
    if (adcXValue >= 3000 )
    { // 
        keyd |= 0x40;
    }
    else if ( adcXValue <= 1000)
    { // 
        keyd |= 0x80;
    }
    else;
    return keyd;
}


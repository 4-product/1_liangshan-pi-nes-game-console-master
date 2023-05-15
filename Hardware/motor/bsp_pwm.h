#ifndef _BSP_PWM_H
#define _BSP_PWM_H


#include "gd32f4xx.h"
#include "systick.h"

#define BSP_PWM_RCU     RCU_GPIOA               // PWM�˿�ʱ��
#define BSP_PWM_PORT    GPIOA 									// PWM�˿�
#define BSP_PWM_PIN     GPIO_PIN_2              // PWM����
#define BSP_PWM_AF      GPIO_AF_1								// PWM���Ÿ���


#define BSP_PWM_TIMER_RCU  				RCU_TIMER1		// ��ʱ��ʱ��
#define BSP_PWM_TIMER      				TIMER1        // ��ʱ��
#define BSP_PWM_CHANNEL  					TIMER_CH_2    // ��ʱ��ͨ��


#define motor_off()  gpio_bit_reset(BSP_PWM_PORT,BSP_PWM_PIN)//RST  PF10
#define motor_on()  gpio_bit_set(BSP_PWM_PORT,BSP_PWM_PIN)



extern uint8_t pwm_motor_status; // ������״̬  0 �ر� 1��ʼ 

void pwm_config(uint16_t pre,uint16_t per);			// PWM���� 
void pwmMotorSetValue(uint16_t value);          // ����PWM��ֵ

void motro_gpio_config(void);

#endif  /* BSP_PWM_H */

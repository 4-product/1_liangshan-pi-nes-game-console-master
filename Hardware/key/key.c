#include "key.h"

key_ui ui_key;




void key_init(void)
{
	rcu_periph_clock_enable(KEYL_RCU);
	/* configure led GPIO port */
	gpio_mode_set(KEYL_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, KEYL_PIN);
	//gpio_output_options_set(KEYL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, KEYL_PIN);

	rcu_periph_clock_enable(KEYR_RCU);
	/* configure led GPIO port */
	gpio_mode_set(KEYR_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, KEYR_PIN);
	//gpio_output_options_set(KEYR_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, KEYR_PIN);


	/* enable the led clock */
	rcu_periph_clock_enable(KEYA_RCU);
	/* configure led GPIO port */
	gpio_mode_set(KEYA_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, KEYA_PIN);
	//gpio_output_options_set(KEYA_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, KEYA_PIN);

	/* enable the led clock */
	rcu_periph_clock_enable(KEYB_RCU);
	/* configure led GPIO port */
	gpio_mode_set(KEYB_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, KEYB_PIN);
	//gpio_output_options_set(KEYB_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, KEYB_PIN);
}

//void key_get_value(uint32_t port, uint32_t pin)
//{
//	if (SET == gpio_input_bit_get(port, pin))
//	{
//		delay_1ms(10); // 延迟消抖
//		if (SET == gpio_input_bit_get(port, pin))
//		{
//			/* 执行按键按下的动作 */
//			printf("currect port is %d,pin is %d\n", port, pin);
//			while (SET == gpio_input_bit_get(port, pin))
//				; // 等待按键松开
//		}
//	};
//}

u8 get_key_val(void)
{
    u8 key=0;
    //Start 8 Select 4 B 2 A0
    
    if (gpio_input_bit_get(KEYL_PORT, KEYL_PIN) == SET) key |= 0x08;
    if (gpio_input_bit_get(KEYR_PORT, KEYR_PIN) == SET) key |= 0x04;
    if (gpio_input_bit_get(KEYB_PORT, KEYB_PIN) == SET) key |= 0x02;
    if (gpio_input_bit_get(KEYA_PORT, KEYA_PIN) == SET) key |= 0x01;
    
    return key;
}


#include "taskTimer.h"
#include "stdio.h"



#define TASK_NUM  5 // 要计数的任务
uint16_t DelayTable[TASK_NUM] = {0}; // 


/* 定时器配置 
T=（TIM_Period+1）*（TIM_Prescaler+1）/TIMxCLK
		TIM_Period * 20000 / 200MHZ  
*/
void taskTimer_config(uint32_t TIM_Period)
{
		uint8_t i;
    timer_parameter_struct timer_initpara;
	
		/* 中断配置 */
	  nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3); // 设置优先级组
    nvic_irq_enable(TIMER2_IRQn, 0, 1); // 设置中断优先级

	  rcu_periph_clock_enable(RCU_TIMER2);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(TIMER2);  // 定时器复位
	  /* TIMER2 configuration */
    timer_initpara.prescaler         = TIM_Period -1;  // 预分频
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE; // 对齐模式
    timer_initpara.counterdirection  = TIMER_COUNTER_UP; // 计数方向
    timer_initpara.period            = 20000 -1; // 周期
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1; // 时钟分频
    timer_initpara.repetitioncounter = 0; // 重复计数器
    timer_init(TIMER2,&timer_initpara);
	  /* TIMER2 interrupt enable */
    timer_interrupt_enable(TIMER2,TIMER_INT_UP);  // 中断使能
		
	  timer_enable(TIMER2);
		
		/* 清空时间表格 */
		for (i=0;i<TASK_NUM;i++)
				DelayTable[i] = 0;//清零
}

/*!
    \brief      this function handles TIMER0 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER2_IRQHandler(void)
{
	uint8_t i;
    /* clear TIMER interrupt flag */
    timer_interrupt_flag_clear(TIMER2,TIMER_INT_FLAG_UP);
	
	//	printf("taskTimer handler!\n");
		for (i=0;i<TASK_NUM;i++)
			DelayTable[i]++; //数组中每个元素都加1
}

uint8_t  SysGetSignal_1ms(uint16_t nms,uint8_t index)
{
	if (index < TASK_NUM)
	{
		if (DelayTable[index] >= nms) //计数大于定时值，说明时间到
		{
			DelayTable[index] = 0;
			return 1;
		}
	}
	return 0;
}

//复位一个定时信号
//index 定时数组的索引
void SysResetSignal(uint8_t index)
{
	if (index < TASK_NUM)
		DelayTable[index] = 0;
}
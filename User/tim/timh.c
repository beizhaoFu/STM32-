#include "timh.h"

/***********************************************************************************************
*函数名称：void TIM_Mode_Configuration(void)
*函数功能：对定时器1的模式进行设置
*输入参数：无
*输出参数：无
************************************************************************************************/
static void TIM_Mode_Configuration(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//使能或者失能APB2外设时钟

		TIM_TimeBaseStructure.TIM_Prescaler = 3599;//预分频值
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
		TIM_TimeBaseStructure.TIM_Period = 80;//在下一个更新事件装入活动的自动重装载寄存器周期的值
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;//重复计数值
		TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化
		TIM_Cmd(TIM1, ENABLE);
		TIM_ITConfig(TIM1, TIM_IT_Update,ENABLE);//使能或者失能指定的TIM中断
}

/****************************************************************************************
*函数名称：void TIM_NVIC_Configuration(void)
*函数功能：设置TIM的中断优先级
*输入参数：无
*输出参数：无
****************************************************************************************/
static void TIM_NVIC_Configuration(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		/* Enable the WAKEUP_BUTTON_EXTI_IRQn Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************************
*函数名称：void TIM1_Configuration(void)
*函数功能：对STM32的定时器一进行所有需要的配置
*输入参数：无
*输出参数：无
****************************************************************************************/
void TIM1_Configuration(void)
{
		TIM_Mode_Configuration();
		TIM_NVIC_Configuration();
}


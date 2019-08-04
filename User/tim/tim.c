#include "tim.h"
/******************************************************************
*函数名称：void TIM3_Config(void)
*函数功能：启动TIM3时钟，并且初始化TIM3
*输入：无
*输出：无
*******************************************************************/
static void TIM3_Config(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能定时器儿的时钟		
		TIM_TimeBaseStructure.TIM_Prescaler = 3599;//设置预分频值，切记要减1，以为它会自动加1
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;//向上计数模式
		TIM_TimeBaseStructure.TIM_Period = 200;//预装载为20000,1s溢出一次
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//不进行时钟分割，对于时钟分割，暂时还不理解
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//不进行重复计数
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//对结构体进行初始化
		/*需要养成一个好习惯，在使能一个中断的时候先将其清空*/
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);//清空更新标志位
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//清除数据更新中断
		TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);//使能数据更新中断
		TIM_Cmd(TIM3, ENABLE);//使能定时器外设，在用到每一个外设时都需要使能
}

/******************************************************************
*函数名称：void TIM3_NVIC_Config(void)
*函数功能：配置定时器3的中断优先级
*输入：无
*输出：无
*备注：中断优先级组有五种选择：第0组：0位抢占优先级，4位响应优先级
															 第1组：1位抢占优先级，3位响应优先级
															 第2组：2位抢占优先级，2位响应优先级
															 第3组：3位抢占优先级，1位响应优先级
															 第4组：4位抢占优先级，0位响应优先级
*******************************************************************/
static void TIM3_NVIC_Config(void)
{
		NVIC_InitTypeDef  NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

}
/******************************************************************
*函数名称：TIM_CompositeInit(void)
*函数功能：对TIM3进行所有的初始化操作
*输入：无
*输出：无
*******************************************************************/
void TIM3_Init(void)
{
		TIM3_Config();
		TIM3_NVIC_Config();
}

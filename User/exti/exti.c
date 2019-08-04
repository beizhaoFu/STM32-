#include "exti.h"
/*******************************************************************
*函数名称：void EXTI_GPIO_Config(void)
*函数功能：配置外部中断时钟和IO口及中断优先级
*输入：无
*输出：无
*备注：在用到外部中断时记得打开复用时钟
*******************************************************************/
void EXTI_GPIO_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能管脚复用功能
		/*配置IO口*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//
		GPIO_Init(GPIOA, &GPIO_InitStructure);//
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);//使能PA0用作外部中断线路
		/*配置中断模式*/
		EXTI_InitStructure.EXTI_Line = EXTI_Line4;//配置中断线
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能中断线
		EXTI_Init(&EXTI_InitStructure);//
		/*配置中断优先级*/
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断管理模式
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

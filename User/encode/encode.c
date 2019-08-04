#include "encode.h"

/***********************************************************************************************
*函数名称：void TIM2_GPIO_Configuration(void)
*函数功能：对定时器二的IO口模式进行配置
*输入参数：无
*输出参数：无
***********************************************************************************************/
static void TIM2_GPIO_Configuration(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能或者失能APB2外设时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//使能或者失能APB1外设时钟
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化外设GPIOx寄存器
}

/***********************************************************************************************
*函数名称：void TIM4_GPIO_Configuration(void)
*函数功能：对定时器4的IO口模式进行配置
*输入参数：无
*输出参数：无
***********************************************************************************************/
static void TIM4_GPIO_Configuration(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能或者失能APB2外设时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//使能或者失能APB1外设时钟
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化外设GPIOx寄存器
}

/***********************************************************************************************
*函数名称：void TIM2_Mode_Configuration(void)
*函数功能：对定时器二的模式进行配置
*输入参数：无
*输出参数：无
***********************************************************************************************/
static void TIM2_Mode_Configuration(void)
{	
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_ICInitTypeDef TIM_ICInitStructure;
	
		TIM_TimeBaseStructure.TIM_Prescaler = 0;//预分频值
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
		TIM_TimeBaseStructure.TIM_Period = 60000;//在下一个更新事件装入活动的自动重装载寄存器周期的值
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
		TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//初始化
		TIM_SetCounter(TIM2,30000);
		TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//设置TIMx编码界面	
		TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_ICFilter = 10;//输入捕获滤波器
		TIM_ICInit(TIM2,&TIM_ICInitStructure);//初始化
		TIM_Cmd(TIM2,ENABLE);
}

/***********************************************************************************************
*函数名称：void TIM4_Mode_Configuration(void)
*函数功能：对定时器4的模式进行配置
*输入参数：无
*输出参数：无
***********************************************************************************************/
static void TIM4_Mode_Configuration(void)
{	
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_ICInitTypeDef TIM_ICInitStructure;
	
		TIM_TimeBaseStructure.TIM_Prescaler = 0;//预分频值
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
		TIM_TimeBaseStructure.TIM_Period = 60000;//在下一个更新事件装入活动的自动重装载寄存器周期的值
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
		TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化
		TIM_SetCounter(TIM4,30000);
		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//设置TIMx编码界面	
		TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_ICFilter = 10;//输入捕获滤波器
		TIM_ICInit(TIM4,&TIM_ICInitStructure);//初始化
		TIM_Cmd(TIM4,ENABLE);
}

/***********************************************************************************************
*函数名称：void TIM2_Code_Configuration(void)
*函数功能：对定时器二的编码器模式进行配置
*输入参数：无
*输出参数：无
***********************************************************************************************/	
void TIM2_Code_Configuration(void)
{
		TIM2_GPIO_Configuration();
		TIM2_Mode_Configuration();
}

/***********************************************************************************************
*函数名称：void TIM4_Code_Configuration(void)
*函数功能：对定时器二的编码器模式进行配置
*输入参数：无
*输出参数：无
***********************************************************************************************/	
void TIM4_Code_Configuration(void)
{
		TIM4_GPIO_Configuration();
		TIM4_Mode_Configuration();
}
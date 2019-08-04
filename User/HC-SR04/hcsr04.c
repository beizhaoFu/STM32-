#include "hcsr04.h"

/**********************************************************************************************
*函数名称：void HCSR04_GPIO_Configuration(void)
*函数功能：超声波模块的IO口配置
*输入参数：无
*输出参数：无
**********************************************************************************************/
static void HCSR04_GPIO_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(HCSR_GPIO_CLK,ENABLE);
		RCC_APB1PeriphClockCmd(HCSR_TIM_CLK,ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = TRIG_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
		GPIO_Init(TRIG_GPIO, &GPIO_InitStructure);//初始化外设GPIOx寄存器
	
		GPIO_InitStructure.GPIO_Pin = ECHO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(ECHO_GPIO, &GPIO_InitStructure);//初始化外设GPIOx寄存器
}
/**********************************************************************************************
*函数名称：void HCSR04_TIM_Configuration(void)
*函数功能：超声波模块定时器的模式配置
*输入参数：无
*输出参数：无
**********************************************************************************************/
static void HCSR04_TIM_Configuration(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_TimeBaseStructure.TIM_Period = 65000; 
		TIM_TimeBaseStructure.TIM_Prescaler = 71; 
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
		TIM_TimeBaseInit(HCSR_TIM, & TIM_TimeBaseStructure);
		TIM_SetCounter(HCSR_TIM,0);
		TIM_Cmd(HCSR_TIM,DISABLE);		
}

/**********************************************************************************************
*函数名称：void HCSR04_Init(void)
*函数功能：超声波模块初始化
*输入参数：无
*输出参数：无
**********************************************************************************************/
void HCSR04_Init(void)
{
		HCSR04_GPIO_Configuration();
		HCSR04_TIM_Configuration();
}
/**********************************************************************************************
*函数名称：void HCSR04_Startup(void)
*函数功能：超声波模块启动
*输入参数：无
*输出参数：无
**********************************************************************************************/
static void HCSR04_Startup(void)
{
		TRIG_HIGH();
		delay_us(10);
		TRIG_LOW();
}

/**********************************************************************************************
*函数名称：uint16_t HCSR04_WaitToConvert(void)
*函数功能：超声波模块转换
*输入参数：无
*输出参数：测得的长度
**********************************************************************************************/
float HCSR04_WaitToConvert(void)
{
		uint16_t nCount=0;
		uint16_t i=0;
		float Distance=0;
		HCSR04_Startup();
		while(ECHO_READ() == Bit_RESET)
		{
				i++;
				if(i>=50000) break;
		}
		TIM_Cmd(HCSR_TIM,ENABLE);
		while(ECHO_READ() == Bit_SET);
		TIM_Cmd(HCSR_TIM,DISABLE);
		nCount = TIM_GetCounter(HCSR_TIM);
		TIM_SetCounter(HCSR_TIM,0);
		Distance = 0.017f*nCount;
		return Distance;
}









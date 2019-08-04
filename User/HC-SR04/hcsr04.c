#include "hcsr04.h"

/**********************************************************************************************
*�������ƣ�void HCSR04_GPIO_Configuration(void)
*�������ܣ�������ģ���IO������
*�����������
*�����������
**********************************************************************************************/
static void HCSR04_GPIO_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(HCSR_GPIO_CLK,ENABLE);
		RCC_APB1PeriphClockCmd(HCSR_TIM_CLK,ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = TRIG_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
		GPIO_Init(TRIG_GPIO, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���
	
		GPIO_InitStructure.GPIO_Pin = ECHO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(ECHO_GPIO, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���
}
/**********************************************************************************************
*�������ƣ�void HCSR04_TIM_Configuration(void)
*�������ܣ�������ģ�鶨ʱ����ģʽ����
*�����������
*�����������
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
*�������ƣ�void HCSR04_Init(void)
*�������ܣ�������ģ���ʼ��
*�����������
*�����������
**********************************************************************************************/
void HCSR04_Init(void)
{
		HCSR04_GPIO_Configuration();
		HCSR04_TIM_Configuration();
}
/**********************************************************************************************
*�������ƣ�void HCSR04_Startup(void)
*�������ܣ�������ģ������
*�����������
*�����������
**********************************************************************************************/
static void HCSR04_Startup(void)
{
		TRIG_HIGH();
		delay_us(10);
		TRIG_LOW();
}

/**********************************************************************************************
*�������ƣ�uint16_t HCSR04_WaitToConvert(void)
*�������ܣ�������ģ��ת��
*�����������
*�����������õĳ���
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









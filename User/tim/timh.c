#include "timh.h"

/***********************************************************************************************
*�������ƣ�void TIM_Mode_Configuration(void)
*�������ܣ��Զ�ʱ��1��ģʽ��������
*�����������
*�����������
************************************************************************************************/
static void TIM_Mode_Configuration(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��

		TIM_TimeBaseStructure.TIM_Prescaler = 3599;//Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_Period = 80;//����һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;//�ظ�����ֵ
		TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//��ʼ��
		TIM_Cmd(TIM1, ENABLE);
		TIM_ITConfig(TIM1, TIM_IT_Update,ENABLE);//ʹ�ܻ���ʧ��ָ����TIM�ж�
}

/****************************************************************************************
*�������ƣ�void TIM_NVIC_Configuration(void)
*�������ܣ�����TIM���ж����ȼ�
*�����������
*�����������
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
*�������ƣ�void TIM1_Configuration(void)
*�������ܣ���STM32�Ķ�ʱ��һ����������Ҫ������
*�����������
*�����������
****************************************************************************************/
void TIM1_Configuration(void)
{
		TIM_Mode_Configuration();
		TIM_NVIC_Configuration();
}


#include "exti.h"
/*******************************************************************
*�������ƣ�void EXTI_GPIO_Config(void)
*�������ܣ������ⲿ�ж�ʱ�Ӻ�IO�ڼ��ж����ȼ�
*���룺��
*�������
*��ע�����õ��ⲿ�ж�ʱ�ǵô򿪸���ʱ��
*******************************************************************/
void EXTI_GPIO_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//ʹ�ܹܽŸ��ù���
		/*����IO��*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//
		GPIO_Init(GPIOA, &GPIO_InitStructure);//
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);//ʹ��PA0�����ⲿ�ж���·
		/*�����ж�ģʽ*/
		EXTI_InitStructure.EXTI_Line = EXTI_Line4;//�����ж���
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ���ж���
		EXTI_Init(&EXTI_InitStructure);//
		/*�����ж����ȼ�*/
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϹ���ģʽ
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

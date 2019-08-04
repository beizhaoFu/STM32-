#include "tim.h"
/******************************************************************
*�������ƣ�void TIM3_Config(void)
*�������ܣ�����TIM3ʱ�ӣ����ҳ�ʼ��TIM3
*���룺��
*�������
*******************************************************************/
static void TIM3_Config(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//ʹ�ܶ�ʱ������ʱ��		
		TIM_TimeBaseStructure.TIM_Prescaler = 3599;//����Ԥ��Ƶֵ���м�Ҫ��1����Ϊ�����Զ���1
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;//���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_Period = 200;//Ԥװ��Ϊ20000,1s���һ��
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//������ʱ�ӷָ����ʱ�ӷָ��ʱ�������
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//�������ظ�����
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//�Խṹ����г�ʼ��
		/*��Ҫ����һ����ϰ�ߣ���ʹ��һ���жϵ�ʱ���Ƚ������*/
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);//��ո��±�־λ
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//������ݸ����ж�
		TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);//ʹ�����ݸ����ж�
		TIM_Cmd(TIM3, ENABLE);//ʹ�ܶ�ʱ�����裬���õ�ÿһ������ʱ����Ҫʹ��
}

/******************************************************************
*�������ƣ�void TIM3_NVIC_Config(void)
*�������ܣ����ö�ʱ��3���ж����ȼ�
*���룺��
*�������
*��ע���ж����ȼ���������ѡ�񣺵�0�飺0λ��ռ���ȼ���4λ��Ӧ���ȼ�
															 ��1�飺1λ��ռ���ȼ���3λ��Ӧ���ȼ�
															 ��2�飺2λ��ռ���ȼ���2λ��Ӧ���ȼ�
															 ��3�飺3λ��ռ���ȼ���1λ��Ӧ���ȼ�
															 ��4�飺4λ��ռ���ȼ���0λ��Ӧ���ȼ�
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
*�������ƣ�TIM_CompositeInit(void)
*�������ܣ���TIM3�������еĳ�ʼ������
*���룺��
*�������
*******************************************************************/
void TIM3_Init(void)
{
		TIM3_Config();
		TIM3_NVIC_Config();
}

#include "encode.h"

/***********************************************************************************************
*�������ƣ�void TIM2_GPIO_Configuration(void)
*�������ܣ��Զ�ʱ������IO��ģʽ��������
*�����������
*�����������
***********************************************************************************************/
static void TIM2_GPIO_Configuration(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���
}

/***********************************************************************************************
*�������ƣ�void TIM4_GPIO_Configuration(void)
*�������ܣ��Զ�ʱ��4��IO��ģʽ��������
*�����������
*�����������
***********************************************************************************************/
static void TIM4_GPIO_Configuration(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���
}

/***********************************************************************************************
*�������ƣ�void TIM2_Mode_Configuration(void)
*�������ܣ��Զ�ʱ������ģʽ��������
*�����������
*�����������
***********************************************************************************************/
static void TIM2_Mode_Configuration(void)
{	
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_ICInitTypeDef TIM_ICInitStructure;
	
		TIM_TimeBaseStructure.TIM_Prescaler = 0;//Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_Period = 60000;//����һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
		TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//��ʼ��
		TIM_SetCounter(TIM2,30000);
		TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//����TIMx�������	
		TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_ICFilter = 10;//���벶���˲���
		TIM_ICInit(TIM2,&TIM_ICInitStructure);//��ʼ��
		TIM_Cmd(TIM2,ENABLE);
}

/***********************************************************************************************
*�������ƣ�void TIM4_Mode_Configuration(void)
*�������ܣ��Զ�ʱ��4��ģʽ��������
*�����������
*�����������
***********************************************************************************************/
static void TIM4_Mode_Configuration(void)
{	
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_ICInitTypeDef TIM_ICInitStructure;
	
		TIM_TimeBaseStructure.TIM_Prescaler = 0;//Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_Period = 60000;//����һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
		TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ��
		TIM_SetCounter(TIM4,30000);
		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//����TIMx�������	
		TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_ICFilter = 10;//���벶���˲���
		TIM_ICInit(TIM4,&TIM_ICInitStructure);//��ʼ��
		TIM_Cmd(TIM4,ENABLE);
}

/***********************************************************************************************
*�������ƣ�void TIM2_Code_Configuration(void)
*�������ܣ��Զ�ʱ�����ı�����ģʽ��������
*�����������
*�����������
***********************************************************************************************/	
void TIM2_Code_Configuration(void)
{
		TIM2_GPIO_Configuration();
		TIM2_Mode_Configuration();
}

/***********************************************************************************************
*�������ƣ�void TIM4_Code_Configuration(void)
*�������ܣ��Զ�ʱ�����ı�����ģʽ��������
*�����������
*�����������
***********************************************************************************************/	
void TIM4_Code_Configuration(void)
{
		TIM4_GPIO_Configuration();
		TIM4_Mode_Configuration();
}
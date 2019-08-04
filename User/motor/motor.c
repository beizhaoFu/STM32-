#include "motor.h"
#include "app.h"

uint16_t ARR_Val;
/***********************************************************************************************
*�������ƣ�void TIM3_GPIO_configuration(void)
*�������ܣ���ʱ��5��ʱ�ӻ�׼�����Լ�ʱ������
*�����������
*�����������
***********************************************************************************************/
static void TIM3_GPIO_configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���

}
/***********************************************************************************************
*�������ƣ�void TIM3_Mode_configuration(void)
*�������ܣ���ʱ��3��ʱ�ӻ�׼�����Լ�ʱ������
*�����������
*�����������
***********************************************************************************************/
static void TIM3_Mode_configuration(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��
		
		TIM_TimeBaseStructure.TIM_Prescaler = 0;//Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_Period = 7200;//����һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x01;//�ظ�����ֵ
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ��
		TIM_ARRPreloadConfig(TIM3, ENABLE);//ʹ�ܻ���ʧ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
		TIM_Cmd(TIM3, ENABLE);//ʹ�ܻ���ʧ��TIMx����
}

/***********************************************************************************************
*�������ƣ�void TIM3_PWM_configuration(void)
*�������ܣ���ʱ��3��PWM�������
*�����������
*�����������
***********************************************************************************************/
static void TIM3_PWM_configuration(void)
{
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;	//0-7200֮��仯
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
}


/***********************************************************************************************
*�������ƣ�void DIRECT_GPIO_Config(void)
*�������ܣ����Ƶ��ת�������IO������
*�����������
*�����������
***********************************************************************************************/
static void DIRECT_GPIO_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
		GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���
}

/***********************************************************************************************
*�������ƣ�void MOTOR_configuration(void)
*�������ܣ������ʼ������
*�����������
*�����������
***********************************************************************************************/
void MOTOR_configuration(void)
{
		TIM3_GPIO_configuration();
		DIRECT_GPIO_Config();
		TIM3_Mode_configuration();
		TIM3_PWM_configuration();
}


/***********************************************************************************************
*�������ƣ�void MOTOR_SetFrenquency(void)
*�������ܣ�PWMƵ�ʵ���
*���������Fren:��Ҫ��Ƶ��
*�����������
***********************************************************************************************/
void MOTOR_SetFrenquency(uint32_t Fren)
{		
		ARR_Val = 800000/Fren;
		TIM_SetAutoreload(TIM3, ARR_Val);	
}

/***********************************************************************************************
*�������ƣ�void MOTOR2_Rotation(int pwm)
*�������ܣ��趨�����ת�ٺͷ���
*���������pwn:ת������
*�����������
***********************************************************************************************/
void MOTOR2_Rotation(int pwm)
{
		if(pwm>=0)	//�����趨
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_10);
			GPIO_SetBits(GPIOB,GPIO_Pin_11);
		}
		else	
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_11);
			GPIO_SetBits(GPIOB,GPIO_Pin_10);
		}
		
		TIM_SetCompare3(TIM3, myabs(pwm));//����TIM4����Ƚ�1�Ĵ���ֵ 
}

/***********************************************************************************************
*�������ƣ�void MOTOR4_Rotation(int pwm)
*�������ܣ��趨�����ת�ٺͷ���
*���������pwn:ת������
*�����������
***********************************************************************************************/
void MOTOR4_Rotation(int pwm)
{
		if(pwm>=0)	//�����趨
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);
			GPIO_SetBits(GPIOC,GPIO_Pin_4);
		}
		else	
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
			GPIO_SetBits(GPIOC,GPIO_Pin_5);
		}
		
		TIM_SetCompare4(TIM3, myabs(pwm));//����TIM4����Ƚ�1�Ĵ���ֵ 
}

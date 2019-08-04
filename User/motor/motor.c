#include "motor.h"
#include "app.h"

uint16_t ARR_Val;
/***********************************************************************************************
*函数名称：void TIM3_GPIO_configuration(void)
*函数功能：定时器5的时钟基准配置以及时钟设置
*输入参数：无
*输出参数：无
***********************************************************************************************/
static void TIM3_GPIO_configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能或者失能APB1外设时钟
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化外设GPIOx寄存器

}
/***********************************************************************************************
*函数名称：void TIM3_Mode_configuration(void)
*函数功能：定时器3的时钟基准配置以及时钟设置
*输入参数：无
*输出参数：无
***********************************************************************************************/
static void TIM3_Mode_configuration(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能或者失能APB1外设时钟
		
		TIM_TimeBaseStructure.TIM_Prescaler = 0;//预分频值
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
		TIM_TimeBaseStructure.TIM_Period = 7200;//在下一个更新事件装入活动的自动重装载寄存器周期的值
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x01;//重复计数值
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化
		TIM_ARRPreloadConfig(TIM3, ENABLE);//使能或者失能TIMx在ARR上的预装载寄存器
		TIM_Cmd(TIM3, ENABLE);//使能或者失能TIMx外设
}

/***********************************************************************************************
*函数名称：void TIM3_PWM_configuration(void)
*函数功能：定时器3的PWM输出配置
*输入参数：无
*输出参数：无
***********************************************************************************************/
static void TIM3_PWM_configuration(void)
{
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;	//0-7200之间变化
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
*函数名称：void DIRECT_GPIO_Config(void)
*函数功能：控制电机转动方向的IO口配置
*输入参数：无
*输出参数：无
***********************************************************************************************/
static void DIRECT_GPIO_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能或者失能APB1外设时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能或者失能APB1外设时钟
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化外设GPIOx寄存器
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
		GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化外设GPIOx寄存器
}

/***********************************************************************************************
*函数名称：void MOTOR_configuration(void)
*函数功能：电机初始化配置
*输入参数：无
*输出参数：无
***********************************************************************************************/
void MOTOR_configuration(void)
{
		TIM3_GPIO_configuration();
		DIRECT_GPIO_Config();
		TIM3_Mode_configuration();
		TIM3_PWM_configuration();
}


/***********************************************************************************************
*函数名称：void MOTOR_SetFrenquency(void)
*函数功能：PWM频率调节
*输入参数：Fren:想要的频率
*输出参数：无
***********************************************************************************************/
void MOTOR_SetFrenquency(uint32_t Fren)
{		
		ARR_Val = 800000/Fren;
		TIM_SetAutoreload(TIM3, ARR_Val);	
}

/***********************************************************************************************
*函数名称：void MOTOR2_Rotation(int pwm)
*函数功能：设定电机的转速和方向
*输入参数：pwn:转速设置
*输出参数：无
***********************************************************************************************/
void MOTOR2_Rotation(int pwm)
{
		if(pwm>=0)	//方向设定
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_10);
			GPIO_SetBits(GPIOB,GPIO_Pin_11);
		}
		else	
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_11);
			GPIO_SetBits(GPIOB,GPIO_Pin_10);
		}
		
		TIM_SetCompare3(TIM3, myabs(pwm));//设置TIM4捕获比较1寄存器值 
}

/***********************************************************************************************
*函数名称：void MOTOR4_Rotation(int pwm)
*函数功能：设定电机的转速和方向
*输入参数：pwn:转速设置
*输出参数：无
***********************************************************************************************/
void MOTOR4_Rotation(int pwm)
{
		if(pwm>=0)	//方向设定
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);
			GPIO_SetBits(GPIOC,GPIO_Pin_4);
		}
		else	
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
			GPIO_SetBits(GPIOC,GPIO_Pin_5);
		}
		
		TIM_SetCompare4(TIM3, myabs(pwm));//设置TIM4捕获比较1寄存器值 
}

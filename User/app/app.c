#include "app.h"

/**************************************************************************************************
*函数名称:int MOTOR2_GetSpeed(void)
*函数功能：获取电机2的速度
*输入参数：无
*输出参数：速度
**************************************************************************************************/
int MOTOR2_GetSpeed(void)
{
		int speed;
		
		speed = TIM_GetCounter(TIM2)-30000;	//计算速度
		TIM_SetCounter(TIM2,30000);
		return speed;
}

/**************************************************************************************************
*函数名称:int MOTOR4_GetSpeed(void)
*函数功能：获取电机4的速度
*输入参数：无
*输出参数：速度
**************************************************************************************************/
int MOTOR4_GetSpeed(void)
{
		int speed;
		
		speed = TIM_GetCounter(TIM4)-30000;	//计算速度
		TIM_SetCounter(TIM4,30000);
		return speed;
}



/**************************************************************************************************
*函数名称:void SpeedPID_Init(SpeedPid *pid)
*函数功能：速度环PID初始化
*输入参数：速度环PID结构体
*输出参数：无
**************************************************************************************************/
void SpeedPID_Init(SpeedPid *pid)
{
		pid->SetSpeed=0;
		pid->ActualSpeed=0;
		pid->err=0;
		pid->lastErr=0;
		pid->oldOutputPWM=0;
		pid->newOutputPWM=0;
		pid->integral=0;
		pid->Kp=15;
		pid->Ki=1.0;
		pid->Kd=0;
}

/**************************************************************************************************
*函数名称:void SpeedPID_Realize(SpeedPid *pid,int actualSpeed)
*函数功能：速度环PID处理
*输入参数：速度环PID结构体和实际速度
*输出参数：无
**************************************************************************************************/
int Movement=0;
void SpeedPID_Realize(SpeedPid *pid,int actualSpeed)
{
		pid->ActualSpeed = actualSpeed;
		pid->err = pid->SetSpeed - pid->ActualSpeed;
		pid->integral = pid->integral + pid->err;
		pid->integral = pid->integral - Movement;
		pid->oldOutputPWM = pid->newOutputPWM;
		pid->newOutputPWM = pid->Kp*pid->err + pid->Ki*pid->integral;
}

/**************************************************************************************************
*函数名称:int SpeedControlOutput(SpeedPid *pid,uint8_t period)
*函数功能：速度环输出函数
*输入参数：速度环PID结构体和周期
*输出参数：实际应该输出的PWM
**************************************************************************************************/
int SpeedControlOutput(SpeedPid *pid,uint8_t period)
{
		int output,diff;
		
		diff = pid->newOutputPWM - pid->oldOutputPWM;
		output = pid->oldOutputPWM + diff*(period+1)/SPEED_CONTROL_PERIOD;
		return output;
		
}
/**************************************************************************************************
*函数名称:void AnglePID_Init(AnglePid *pid)
*函数功能：角度环PID初始化
*输入参数：角度环结构体
*输出参数：实际应该输出的速度
**************************************************************************************************/
void AnglePID_Init(AnglePid *pid)
{
		pid->SetAngle = 0;
		pid->ActualAngle = 0;
		pid->err = 0;
		pid->index = 0;
		pid->integral = 0;
		pid->Kp = 625;
		pid->Ki = 0;
		pid->Kd = -0.85;
		pid->lastErr = 0;
		pid->outputSpeed = 0;
}

/**************************************************************************************************
*函数名称:int AnglePID_Realize(AnglePid *pid,float actualAngle)
*函数功能：角度环PID处理
*输入参数：角度环结构体和实际角度
*输出参数：实际应该输出的速度
**************************************************************************************************/
int AnglePID_Realize(AnglePid *pid,float actualAngle,short Gyroy)
{	
				pid->ActualAngle = actualAngle;
				pid->err = pid->SetAngle - pid->ActualAngle;
				pid->outputSpeed = pid->Kp*pid->err + pid->Kd*Gyroy;
				return pid->outputSpeed;
}

/**************************************************************************************************
*函数名称:void TurnPID_Init(TurnPid *pid)
*函数功能：转向环初始化
*输入参数：转向环结构体
*输出参数：无
**************************************************************************************************/
void TurnPID_Init(TurnPid *pid)
{
		pid->SetSpeedSum = 0;
		pid->Kp = -4.5;
		pid->Kd = 0.8;
		pid->ActualSpeedSum = 0;
		pid->err = 0;
		pid->oldOutputPWM = 0;
		pid->newOutputPWM = 0;
}

/**************************************************************************************************
*函数名称:void TurnPID_Realize(TurnPid *pid,int diffSpeedSum,short gz)
*函数功能：转向环实现函数
*输入参数：转向环结构体和转向的速度
*输出参数：无
**************************************************************************************************/
void TurnPID_Realize(TurnPid *pid,int diffSpeedSum,short gz)
{
		pid->err = pid->SetSpeedSum - diffSpeedSum;
		pid->oldOutputPWM = pid->newOutputPWM;
		pid->newOutputPWM = pid->Kp*pid->err + pid->Kd*gz;
}

/**************************************************************************************************
*函数名称:int TurnControlOutput(TurnPid *pid,uint8_t period)
*函数功能：转向环输出控制
*输入参数：转向环结构体和周期
*输出参数：无
**************************************************************************************************/
int TurnControlOutput(TurnPid *pid,uint8_t period)
{
		int output,diff;
		
		diff = pid->newOutputPWM - pid->oldOutputPWM;
		output = pid->oldOutputPWM + diff*(period+1)/TURN_CONTROL_PERIOD;
		return output;
}

/**************************************************************************************************
*函数名称:void MotorOutput(int output2,int output4)
*函数功能：实现电机的PWM输出和死区控制
*输入参数：两个电机的PWM
*输出参数：无
**************************************************************************************************/
int DeathArea=0;
void MotorOutput(int output2,int output4)
{
		if(output2>=0)	output2 = output2+DeathArea;	//为小车加上死区电压，防止小车静止时左右移动
		else						output2 = output2-DeathArea;	
	
		if(output4>=0)	output4 = output4+DeathArea;
		else						output4 = output4-DeathArea;	
	
	
		if(output2>7200) output2=7200;								//为小车的PWM输出限制幅值
		else if(output2<-7200) output2=-7200;
	
		if(output4>7200) output4=7200;
		else if(output4<-7200) output4=-7200;
	
		MOTOR2_Rotation(output2);
		MOTOR4_Rotation(output4);
}

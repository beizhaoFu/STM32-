#include "app.h"

/**************************************************************************************************
*��������:int MOTOR2_GetSpeed(void)
*�������ܣ���ȡ���2���ٶ�
*�����������
*����������ٶ�
**************************************************************************************************/
int MOTOR2_GetSpeed(void)
{
		int speed;
		
		speed = TIM_GetCounter(TIM2)-30000;	//�����ٶ�
		TIM_SetCounter(TIM2,30000);
		return speed;
}

/**************************************************************************************************
*��������:int MOTOR4_GetSpeed(void)
*�������ܣ���ȡ���4���ٶ�
*�����������
*����������ٶ�
**************************************************************************************************/
int MOTOR4_GetSpeed(void)
{
		int speed;
		
		speed = TIM_GetCounter(TIM4)-30000;	//�����ٶ�
		TIM_SetCounter(TIM4,30000);
		return speed;
}



/**************************************************************************************************
*��������:void SpeedPID_Init(SpeedPid *pid)
*�������ܣ��ٶȻ�PID��ʼ��
*����������ٶȻ�PID�ṹ��
*�����������
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
*��������:void SpeedPID_Realize(SpeedPid *pid,int actualSpeed)
*�������ܣ��ٶȻ�PID����
*����������ٶȻ�PID�ṹ���ʵ���ٶ�
*�����������
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
*��������:int SpeedControlOutput(SpeedPid *pid,uint8_t period)
*�������ܣ��ٶȻ��������
*����������ٶȻ�PID�ṹ�������
*���������ʵ��Ӧ�������PWM
**************************************************************************************************/
int SpeedControlOutput(SpeedPid *pid,uint8_t period)
{
		int output,diff;
		
		diff = pid->newOutputPWM - pid->oldOutputPWM;
		output = pid->oldOutputPWM + diff*(period+1)/SPEED_CONTROL_PERIOD;
		return output;
		
}
/**************************************************************************************************
*��������:void AnglePID_Init(AnglePid *pid)
*�������ܣ��ǶȻ�PID��ʼ��
*����������ǶȻ��ṹ��
*���������ʵ��Ӧ��������ٶ�
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
*��������:int AnglePID_Realize(AnglePid *pid,float actualAngle)
*�������ܣ��ǶȻ�PID����
*����������ǶȻ��ṹ���ʵ�ʽǶ�
*���������ʵ��Ӧ��������ٶ�
**************************************************************************************************/
int AnglePID_Realize(AnglePid *pid,float actualAngle,short Gyroy)
{	
				pid->ActualAngle = actualAngle;
				pid->err = pid->SetAngle - pid->ActualAngle;
				pid->outputSpeed = pid->Kp*pid->err + pid->Kd*Gyroy;
				return pid->outputSpeed;
}

/**************************************************************************************************
*��������:void TurnPID_Init(TurnPid *pid)
*�������ܣ�ת�򻷳�ʼ��
*���������ת�򻷽ṹ��
*�����������
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
*��������:void TurnPID_Realize(TurnPid *pid,int diffSpeedSum,short gz)
*�������ܣ�ת��ʵ�ֺ���
*���������ת�򻷽ṹ���ת����ٶ�
*�����������
**************************************************************************************************/
void TurnPID_Realize(TurnPid *pid,int diffSpeedSum,short gz)
{
		pid->err = pid->SetSpeedSum - diffSpeedSum;
		pid->oldOutputPWM = pid->newOutputPWM;
		pid->newOutputPWM = pid->Kp*pid->err + pid->Kd*gz;
}

/**************************************************************************************************
*��������:int TurnControlOutput(TurnPid *pid,uint8_t period)
*�������ܣ�ת���������
*���������ת�򻷽ṹ�������
*�����������
**************************************************************************************************/
int TurnControlOutput(TurnPid *pid,uint8_t period)
{
		int output,diff;
		
		diff = pid->newOutputPWM - pid->oldOutputPWM;
		output = pid->oldOutputPWM + diff*(period+1)/TURN_CONTROL_PERIOD;
		return output;
}

/**************************************************************************************************
*��������:void MotorOutput(int output2,int output4)
*�������ܣ�ʵ�ֵ����PWM�������������
*������������������PWM
*�����������
**************************************************************************************************/
int DeathArea=0;
void MotorOutput(int output2,int output4)
{
		if(output2>=0)	output2 = output2+DeathArea;	//ΪС������������ѹ����ֹС����ֹʱ�����ƶ�
		else						output2 = output2-DeathArea;	
	
		if(output4>=0)	output4 = output4+DeathArea;
		else						output4 = output4-DeathArea;	
	
	
		if(output2>7200) output2=7200;								//ΪС����PWM������Ʒ�ֵ
		else if(output2<-7200) output2=-7200;
	
		if(output4>7200) output4=7200;
		else if(output4<-7200) output4=-7200;
	
		MOTOR2_Rotation(output2);
		MOTOR4_Rotation(output4);
}

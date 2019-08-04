#ifndef APP_H
#define APP_H

#include "stm32f10x.h"
#include "oled.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"
#include "tim.h"
#include "systick.h"
#include "usart.h"
#include "tim.h"
#include "encode.h"
#include "motor.h"
#include "timh.h"
#include "app.h"

#define myabs(x)   ((x>=0)?x:(-1*x))
#define SPEED_CONTROL_PERIOD	20
#define	TURN_CONTROL_PERIOD		20

typedef struct __SpeedPid
{
		int SetSpeed;
		int ActualSpeed;
		int err;
		int lastErr;
		float Kp,Ki,Kd;
		int oldOutputPWM;
		int newOutputPWM;
		int integral;
}SpeedPid;

typedef struct __AnglePid
{
		float SetAngle;
		float ActualAngle;
		float err;
		float lastErr;
		float Kp,Ki,Kd;
		int outputSpeed;
		float integral;
		int index;
}AnglePid;

typedef struct __TurnPid
{
		float SetSpeedSum;
		float ActualSpeedSum;
		float err;
		float Kp,Kd;
		int oldOutputPWM;
		int newOutputPWM;
}TurnPid;

typedef struct PID_Val
{
		float kpA;
		float kiA;
		float kdA;
	
		float kpB;
		float kiB;
		float kdB;
}PID_Val;


int MOTOR2_GetSpeed(void);
int MOTOR4_GetSpeed(void);
void OLED_DispPolarity(u8 x,u8 y,int num);
void SpeedPID_Init(SpeedPid *pid);
void SpeedPID_Realize(SpeedPid *pid,int actualSpeed);
int SpeedControlOutput(SpeedPid *pid,uint8_t period);
void AnglePID_Init(AnglePid *pid);
int AnglePID_Realize(AnglePid *pid,float actualAngle,short Gyroy);
void TurnPID_Init(TurnPid *pid);
void TurnPID_Realize(TurnPid *pid,int diffSpeedSum,short gz);
int TurnControlOutput(TurnPid *pid,uint8_t period);
void MotorOutput(int output2,int output4);

#endif

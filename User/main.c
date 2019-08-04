/*********************************************************************************************
*程序功能：用STM32驱动OLED显示
*书写时间：2015年5月24日
*调试结果：
*********************************************************************************************/
  
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
#include "remote.h"
#include "exti.h"
#include "hcsr04.h"

float distance;
AnglePid AnglePidA;
SpeedPid SpeedPidA;
TurnPid  TurnPidA;
extern MPU6050_DMP_PACKET mpu6050_info;
extern int speed2,speed4;
extern int Movement;
extern RC_Ctl_t RC_Ctl;
extern uint16_t diffcount;

void MPU6050_DMP_Init(void);

//main函数
int main(void)
{
		int num=0;
		uint8_t flag=0;
		AnglePID_Init(&AnglePidA);
		SpeedPID_Init(&SpeedPidA);
		TurnPID_Init(&TurnPidA);
		OLED_Init();		//OLED初始化
		OLED_ShowString(0,0,"hardware init...");
		TIM2_Code_Configuration();
		TIM4_Code_Configuration();
		USARTInit();		//串口1初始化
		MOTOR_configuration();
		MPU6050_DMP_Init();//MPU6050之DMP初始化
		OLED_ShowString(0,0,"                ");
		OLED_ShowString(36,0,"content");
		OLED_ShowString(0,16,"M2:     M4:");	//界面初始化
		OLED_ShowString(0,32,"MPU:");
		OLED_ShowString(0,48,"DIS:");
		OLED_ShowString(85,48,"cm");
		Remote_Init();
		HCSR04_Init();
		TIM1_Configuration();
  	while(1)
		{
				
				OLED_DispPolarity(24,16,speed2);
				OLED_DispPolarity(88,16,speed4);
				OLED_DispFloat(35,32,mpu6050_info.pitch);//
				OLED_DispInt(87,32,mpu6050_info.gyro[1]);
				distance = HCSR04_WaitToConvert();
				if(RC_Ctl.rc.s1==1)
				{
					if(flag==0)
					{
						OLED_ShowString(32,48,"           ");
						OLED_ShowString(85,48,"cm");flag++;
					}
					OLED_DispFloat(35,48,distance);//
				}
				else
				{
						flag=0;
						OLED_ShowString(32,48,"DISABLE  ");
				}
		}
	  
}

void MPU6050_DMP_Init(void)
{
		u8 res;
		CT_IIC_Init();//初始化模拟IIC接口
		while(IIC_CheckDevice(MPU6050_Add)==1)	//检测MPU6050硬件是否存在
		{
				delay_ms(100);
				OLED_ShowString(1,0,"can't find mpu6050!");
		}
		MPU6050_Init();			//MPU6050初始化
		delay_ms(50);
		while(res = mpu_dmp_init())	//ＤＭＰ初始化
		{
			OLED_ShowString(1,0,"dmplib init failed!");
			MPU6050_Init();			//MPU6050初始化
			delay_ms(50);
		}
}


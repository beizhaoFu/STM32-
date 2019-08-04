/*********************************************************************************************
*�����ܣ���STM32����OLED��ʾ
*��дʱ�䣺2015��5��24��
*���Խ����
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

//main����
int main(void)
{
		int num=0;
		uint8_t flag=0;
		AnglePID_Init(&AnglePidA);
		SpeedPID_Init(&SpeedPidA);
		TurnPID_Init(&TurnPidA);
		OLED_Init();		//OLED��ʼ��
		OLED_ShowString(0,0,"hardware init...");
		TIM2_Code_Configuration();
		TIM4_Code_Configuration();
		USARTInit();		//����1��ʼ��
		MOTOR_configuration();
		MPU6050_DMP_Init();//MPU6050֮DMP��ʼ��
		OLED_ShowString(0,0,"                ");
		OLED_ShowString(36,0,"content");
		OLED_ShowString(0,16,"M2:     M4:");	//�����ʼ��
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
		CT_IIC_Init();//��ʼ��ģ��IIC�ӿ�
		while(IIC_CheckDevice(MPU6050_Add)==1)	//���MPU6050Ӳ���Ƿ����
		{
				delay_ms(100);
				OLED_ShowString(1,0,"can't find mpu6050!");
		}
		MPU6050_Init();			//MPU6050��ʼ��
		delay_ms(50);
		while(res = mpu_dmp_init())	//�ģͣг�ʼ��
		{
			OLED_ShowString(1,0,"dmplib init failed!");
			MPU6050_Init();			//MPU6050��ʼ��
			delay_ms(50);
		}
}


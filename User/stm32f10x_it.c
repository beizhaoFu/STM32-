/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"
#include "usart.h"
#include "oled.h"
#include "app.h"
#include "usart.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/


//---------串口接收ＤＭＡ中断---------------
extern uint8_t parameter[48];
union dataConvert PAValue;
union dataConvert IAValue;
union dataConvert DAValue;
union dataConvert PBValue;
union dataConvert IBValue;
union dataConvert DBValue;
extern AnglePid AnglePidA;
extern SpeedPid SpeedPidA;
extern TurnPid  TurnPidA;
extern int Movement;
extern int DeathArea;
void DMA1_Channel5_IRQHandler(void)  //
{
		uint8_t i;
		//清除标志位  
    DMA_ClearFlag(DMA1_FLAG_TC5); 	
		DMA_ClearITPendingBit(DMA1_IT_TC5);
		for(i=0;i<8;i++)
		{
				PAValue.table[i] = parameter[i];
		}
		for(i=0;i<8;i++)
		{
				IAValue.table[i] = parameter[8+i];
		}
		for(i=0;i<8;i++)
		{
				DAValue.table[i] = parameter[i+16];
		}
		for(i=0;i<8;i++)
		{
				PBValue.table[i] = parameter[i+24];
		}
		for(i=0;i<8;i++)
		{
				IBValue.table[i] = parameter[32+i];
		}
		for(i=0;i<8;i++)
		{
				DBValue.table[i] = parameter[i+40];
		}
//		AnglePidA.Kp = PAValue.num;AnglePidA.Ki = IAValue.num;AnglePidA.Kd = DAValue.num;
//		SpeedPidA.Kp = PBValue.num;SpeedPidA.Ki = IBValue.num;SpeedPidA.Kd = DBValue.num;
		TurnPidA.Kp = PAValue.num;Movement = IAValue.num;TurnPidA.Kd=DAValue.num;
		TurnPidA.SetSpeedSum = PBValue.num;
//			DeathArea = PAValue.num*10;
}

MPU6050_DMP_PACKET mpu6050_info;
int speed2,speed4;
void EXTI4_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line4)==SET)
		{
				EXTI_ClearITPendingBit(EXTI_Line4);  
		}
}


void TIM1_UP_IRQHandler(void)
{
		int balance=0;
		int speedout=0;
		int turn=0;
		int outputpwm2,outputpwm4;
		int diffSpeedSum;
		static uint8_t SpeedControlPeriod=0;
		static uint8_t TurnControlPeriod=0;
		static int Speed2Sum=0,Speed4Sum=0;
		
		if(TIM_GetFlagStatus(TIM1, TIM_FLAG_Update)==SET)
		{
				mpu_dmp_get_data(&mpu6050_info);
				if(myabs(mpu6050_info.pitch)<55)	//满足条件才进行PID控制
				{
						/*-------------------计算各个控制量---------------------*/
						SpeedControlPeriod++;	
						speedout = SpeedControlOutput(&SpeedPidA,SpeedControlPeriod);
						TurnControlPeriod++;
						turn =  TurnControlOutput(&TurnPidA,TurnControlPeriod);
						balance = AnglePID_Realize(&AnglePidA,mpu6050_info.pitch,mpu6050_info.gyro[1]);	//角度控制，周期5ms
					
						
						if(SpeedControlPeriod >= SPEED_CONTROL_PERIOD)	//速度和转向控制，周期100ms
						{
								SpeedControlPeriod = 0;
								TurnControlPeriod = 0;
								speed2 = MOTOR2_GetSpeed();	//获取数据
								speed4 = MOTOR4_GetSpeed();
								Speed2Sum += speed2;   Speed4Sum += speed4;
								diffSpeedSum = Speed2Sum - Speed4Sum;
								TurnPID_Realize(&TurnPidA,diffSpeedSum,mpu6050_info.gyro[2]);
								SpeedPID_Realize(&SpeedPidA,speed2+speed4);
						}		
							
						
						outputpwm2 = balance + speedout + turn;
						outputpwm4 = balance + speedout - turn;
						MotorOutput(outputpwm2,outputpwm4);
				}
				else
				{
						MotorOutput(0,0);
				}

				
				TIM_ClearITPendingBit(TIM1, TIM_IT_Update);//
		}
}
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H
#include "systick.h"    
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//电容触摸屏-IIC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/7
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//********************************************************************************
//修改说明
//V1.1 20140721
//1,修改CT_IIC_Read_Byte函数,读数据更快.
//2,修改CT_IIC_Wait_Ack函数,以支持MDK的-O2优化.
////////////////////////////////////////////////////////////////////////////////// 	


#define GPIO_PORT_I2C	GPIOC			/* GPIO端口 */
#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define I2C_SCL_PIN		GPIO_Pin_1			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN		GPIO_Pin_0			/* 连接到SDA数据线的GPIO */	
	


/* 定义读写SCL和SDA的宏 */
#define I2C_SCL_1()  GPIO_SetBits( GPIO_PORT_I2C,I2C_SCL_PIN) 					/* SCL = 1 */
#define I2C_SCL_0()  GPIO_ResetBits( GPIO_PORT_I2C,I2C_SCL_PIN) 				/* SCL = 0 */

#define I2C_SDA_1()  GPIO_SetBits( GPIO_PORT_I2C,I2C_SDA_PIN)				/* SDA = 1 */
#define I2C_SDA_0() GPIO_ResetBits( GPIO_PORT_I2C,I2C_SDA_PIN) 			/* SDA = 0 */

#define I2C_SDA_READ()  ((GPIO_ReadInputDataBit(GPIO_PORT_I2C,I2C_SDA_PIN) != 0))	/* 读SDA口线状态 */
#define I2C_SCL_READ()  ((GPIO_ReadInputDataBit(GPIO_PORT_I2C,I2C_SCL_PIN)) != 0)	/* 读SCL口线状态 */
	
#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

void CT_IIC_Init(void);
void CT_IIC_Start(void);
void CT_IIC_Stop(void);
void CT_IIC_Send_Byte(uint8_t _ucByte);
uint8_t CT_IIC_Read_Byte(unsigned char ack);
uint8_t CT_IIC_Wait_Ack(void);
void CT_IIC_Ack(void);
void CT_IIC_NAck(void);


uint8_t IIC_CheckDevice(uint8_t _Address);
u8 IIC_WriteByte(u8 add,u8 reg,u8 dat);
u8 IIC_ReadByte(u8 add,u8 reg);

#endif








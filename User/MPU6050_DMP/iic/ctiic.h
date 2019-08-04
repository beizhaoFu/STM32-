#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H
#include "systick.h"    
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//���ݴ�����-IIC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/7
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//********************************************************************************
//�޸�˵��
//V1.1 20140721
//1,�޸�CT_IIC_Read_Byte����,�����ݸ���.
//2,�޸�CT_IIC_Wait_Ack����,��֧��MDK��-O2�Ż�.
////////////////////////////////////////////////////////////////////////////////// 	


#define GPIO_PORT_I2C	GPIOC			/* GPIO�˿� */
#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define I2C_SCL_PIN		GPIO_Pin_1			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN		GPIO_Pin_0			/* ���ӵ�SDA�����ߵ�GPIO */	
	


/* �����дSCL��SDA�ĺ� */
#define I2C_SCL_1()  GPIO_SetBits( GPIO_PORT_I2C,I2C_SCL_PIN) 					/* SCL = 1 */
#define I2C_SCL_0()  GPIO_ResetBits( GPIO_PORT_I2C,I2C_SCL_PIN) 				/* SCL = 0 */

#define I2C_SDA_1()  GPIO_SetBits( GPIO_PORT_I2C,I2C_SDA_PIN)				/* SDA = 1 */
#define I2C_SDA_0() GPIO_ResetBits( GPIO_PORT_I2C,I2C_SDA_PIN) 			/* SDA = 0 */

#define I2C_SDA_READ()  ((GPIO_ReadInputDataBit(GPIO_PORT_I2C,I2C_SDA_PIN) != 0))	/* ��SDA����״̬ */
#define I2C_SCL_READ()  ((GPIO_ReadInputDataBit(GPIO_PORT_I2C,I2C_SCL_PIN)) != 0)	/* ��SCL����״̬ */
	
#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

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








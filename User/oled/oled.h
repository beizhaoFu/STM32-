#ifndef __OLED_H
#define __OLED_H			  	 
 

#include "stm32f10x.h"

//OLED模式设置
//0:4线串行模式

#define OLED_MODE 0
		    						  
//-----------------OLED端口定义----------------  					   

#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_12)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_12)


#define OLED_RS_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_6)//A0
#define OLED_RS_Set() GPIO_SetBits(GPIOC,GPIO_Pin_6)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void SPI2_Init(void);
unsigned char SPI2_ReadWriteByte(unsigned char TxData);
//OLED控制用函数
void OLED_WR_Byte(unsigned char  dat,unsigned char  cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(unsigned char  x,unsigned char  y,unsigned char  t);
void OLED_Fill(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2,unsigned char  dot);
void OLED_ShowCHinese(unsigned char  x,unsigned char  y,unsigned char  *p,unsigned char  mode);
void OLED_ShowChar(unsigned char  x,unsigned char  y,unsigned char  chr,unsigned char  size,unsigned char  mode);
void OLED_ShowNum(unsigned char  x,unsigned char  y,unsigned int num,unsigned char  len,unsigned char  size);
void OLED_ShowString(unsigned char  x,unsigned char  y,const unsigned char  *p);	 
void LCD_Set_Pos(unsigned char  x, unsigned char  y) ;
void OLED_DispPolarity(u8 x,u8 y,int num);
void OLED_DispInt(u8 x,u8 y,int num);
void OLED_DispFloat(u8 x,u8 y,float num);

#endif

	


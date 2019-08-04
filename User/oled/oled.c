#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include <stdio.h>
#include <string.h>
#include "app.h"
/***********************************************************************************************
*函数名称：void SPI2_Init(void)
*函数功能：对SPI2实现初始化操作
*输入参数：无
*输出参数：无
***********************************************************************************************/
void SPI2_Init(void)           //硬件SPI2口初始化
{
		SPI_InitTypeDef  SPI_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE) ;//使能SPI2的时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能IO口时钟


		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_ResetBits(GPIOB, GPIO_Pin_12);//片选信号为低

		/* SPI1 configuration */ 
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为双线双向全双工
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 //设置为主SPI
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//SPI发送接收8位帧结构
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; //时钟悬空高
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//数据捕获于第二个时钟沿
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	//内部NSS信号有SSI位控制
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//波特率预分频值为2
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输从MSB位开始
		SPI_InitStructure.SPI_CRCPolynomial = 7; //SPI_CRCPolynomial定义了用于CRC值计算的多项式
		SPI_Init(SPI2, &SPI_InitStructure);

		SPI_Cmd(SPI2, ENABLE);  
}

/***********************************************************************************************
*函数名称：unsigned char SPI2_ReadWriteByte(unsigned char TxData)
*函数功能：对SPI2进行读写操作
*输入参数：TxData：写入的数据
*输出参数：读到的数据
***********************************************************************************************/
unsigned char SPI2_ReadWriteByte(unsigned char TxData)
{		
		unsigned int retry=0;				 
		while((SPI2->SR&1<<1)==0)//等待发送区空	
		{
			retry++;
			if(retry>2000)return 0;
		}			  
		SPI2->DR=TxData;	 	  //发送一个byte 
		retry=0;
		while((SPI2->SR&1<<0)==0) //等待接收完一个byte  
		{
			retry++;
			if(retry>2000)return 0;
		}	  						    
		return SPI2->DR;          //返回收到的数据				    
}

/***********************************************************************************************
*函数名称：void OLED_WR_Byte(unsigned char  dat,unsigned char  cmd)
*函数功能：对OLED进行写入命令或数据
*输入参数：dat:写入的数据 cmd:数据命令选择（0,表示命令;1,表示数据;）
*输出参数：无
***********************************************************************************************/
void OLED_WR_Byte(unsigned char  dat,unsigned char  cmd)
{			  
	if(cmd)//命令模式或者数据模式	  
	  OLED_RS_Set();
	else 
	  OLED_RS_Clr();
	  OLED_CS_Clr();//片选
	//添加硬件读写SPI函数
	SPI2_ReadWriteByte(dat); 		  
	OLED_CS_Set();
	OLED_RS_Set();   	  
} 

/***********************************************************************************************
*函数名称：void OLED_Refresh_Gram(void)
*函数功能：对OLED写入显示数据
*输入参数：无
*输出参数：无
***********************************************************************************************/
unsigned char  OLED_GRAM[128][8];	  
void OLED_Refresh_Gram(void)
{
	unsigned char  i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x02,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

/***********************************************************************************************
*函数名称：void OLED_Display_On(void)
*函数功能：开启OLED的显示功能
*输入参数：无
*输出参数：无
***********************************************************************************************/  
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
/***********************************************************************************************
*函数名称：void OLED_Display_Off(void)
*函数功能：关闭OLED的显示
*输入参数：无
*输出参数：无
***********************************************************************************************/   
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 

/***********************************************************************************************
*函数名称：void OLED_Clear(void)  
*函数功能：OLED清屏函数
*输入参数：无
*输出参数：无
***********************************************************************************************/   
void OLED_Clear(void)  
{  
	unsigned char  i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//更新显示
}

/***********************************************************************************************
*函数名称：void LCD_Set_Pos(unsigned char  x, unsigned char  y) 
*函数功能：设置OLED的坐标
*输入参数：无
*输出参数：无
***********************************************************************************************/   
void LCD_Set_Pos(unsigned char  x, unsigned char  y) 
{ 
		OLED_WR_Byte(0xb0+(7-y/8),OLED_CMD);
		OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
		OLED_WR_Byte((x&0x0f),OLED_CMD); 
} 

/***********************************************************************************************
*函数名称：void OLED_DrawPoint(unsigned char  x,unsigned char  y,unsigned char  t)
*函数功能：在显示屏上面画一个点
*输入参数：x:0~127 y:0~63 t:1 填充 0,清空	
*输出参数：无
***********************************************************************************************/   
void OLED_DrawPoint(unsigned char  x,unsigned char  y,unsigned char  t)
{
	unsigned char  pos,bx,temp=0;
		LCD_Set_Pos(x, y);  
		if(x>127||y>64)return;//超出范围了.
		pos=7-y/8;
		bx=y%8;
		temp=1<<(7-bx);
		if(t)OLED_GRAM[x][pos]|=temp;
		else OLED_GRAM[x][pos]&=~temp;	
	  
     //设置显示位置—列高地址   
	  OLED_WR_Byte(OLED_GRAM[x][pos],OLED_DATA); 
}

/***********************************************************************************************
*函数名称：void OLED_Fill(unsigned char  x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char dot);
*函数功能：在显示屏上面画一个矩形
*输入参数：x1,y1,x2,y2 填充区域的对角坐标   dot:0,清空;1,填充	  
*输出参数：无
***********************************************************************************************/   
void OLED_Fill(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2,unsigned char  dot)  
{  
	unsigned char  x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	//OLED_Refresh_Gram();//更新显示(此处不必要)
}

/***********************************************************************************************
*函数名称：void OLED_ShowChar(unsigned char  x,unsigned char  y,unsigned char  chr,unsigned char  size,unsigned char  mode)
*函数功能：在显示屏上面显示一个字符
*输入参数：x,y:显示的起始位置 x:0~127 y:0~63  chr:显示的字符 size:显示字符的大小16/12 
						mode:显示的模式	  mode:0,反白显示;1,正常显示
*输出参数：无
***********************************************************************************************/   
void OLED_ShowChar(unsigned char  x,unsigned char  y,unsigned char  chr,unsigned char  size,unsigned char  mode)
{      			    
	unsigned char  temp,t,t1;
	unsigned char  y0=y;
	chr=chr-' ';//得到偏移后的值				   
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=oled_asc2_1206[chr][t];  //调用1206字体
		else temp=oled_asc2_1608[chr][t];		 //调用1608字体 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

/***********************************************************************************************
*函数名称：void OLED_ShowCHinese(unsigned char  x,unsigned char  y,unsigned char  *p,unsigned char  mode)
*函数功能：在显示屏上面显示中文
*输入参数：x,y:显示的起始位置 x:0~127 y:0~63  *p:显示的字符串 mode:显示的模式	  mode:0,反白显示;1,正常显示  
*输出参数：无
***********************************************************************************************/   
void OLED_ShowCHinese(unsigned char  x,unsigned char  y,unsigned char  *p,unsigned char  mode)//显示中文
{      			    
		unsigned char  temp,t,t1;
		unsigned char  y0=y;
		unsigned char i=0;
		while(*p != '\0')
		{
				while(i<COUNT)
				{
						if((p[0]==CHINESE[i].Index[0])&&(p[1]==CHINESE[i].Index[1]))
							break;
						++i;
				}
				for(t=0;t<32;t++)
				{   	
						temp= CHINESE[i].Msk[t];//调用1608字体 	                          
						 for(t1=0;t1<8;t1++)
						{
							if(temp&0x80)OLED_DrawPoint(x,y,mode);
							else OLED_DrawPoint(x,y,!mode);
							temp<<=1;
							y++;
							if((y-y0)==16)
							{
								y=y0;
								x++;
								break;
							}
						}  	 
				 }
				p = p+2;
	 }		 
} 


//m^n函数
unsigned int oled_pow(unsigned char  m,unsigned char  n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(unsigned char  x,unsigned char  y,unsigned int num,unsigned char  len,unsigned char  size)
{         	
	unsigned char  t,temp;
	unsigned char  enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void OLED_ShowString(unsigned char  x,unsigned char  y,const unsigned char  *p)
{
#define MAX_CHAR_POSX 127
#define MAX_CHAR_POSY 63         
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,1);	 
        x+=8;
        p++;
    }  
}	

/**************************************************************************************************
*函数名称:void OLED_DispPolarity(u8 x,u8 y,int num)
*函数功能：OLED显示正负数
*输入参数：要显示的数字和坐标
*输出参数：无
**************************************************************************************************/
void OLED_DispPolarity(u8 x,u8 y,int num)
{	
		u8 i=0;
		u16 temp;
	
		OLED_ShowString(x,y,"     ");
	
		if(num>0)											//显示符号
			OLED_ShowString(x,y,"+");
		else if(num==0)
		{
			OLED_ShowString(x,y,"0");
		}
		else
			OLED_ShowString(x,y,"-");
		
		temp = myabs(num);
		while(temp)
		{
				temp = temp/10;
				++i;
		}
		OLED_ShowNum(x+8,y,myabs(num),i,16);
}

/**************************************************************************************************
*函数名称:void OLED_DispInt(u8 x,u8 y,int num)
*函数功能：OLED显示整型的数
*输入参数：要显示的数字和坐标
*输出参数：无
**************************************************************************************************/
void OLED_DispInt(u8 x,u8 y,int num)
{
		uint8_t table[10];
	
		sprintf((char*)table,"%5d",num);
		OLED_ShowString(x,y,table);
}

/**************************************************************************************************
*函数名称:void OLED_DispFloat(u8 x,u8 y,float num)
*函数功能：OLED显示浮点数
*输入参数：要显示的数字和坐标
*输出参数：无
**************************************************************************************************/
void OLED_DispFloat(u8 x,u8 y,float num)
{
		uint8_t table[10];
	
		sprintf((char*)table,"%6.2f",num);
		OLED_ShowString(x,y,table);
}
//初始化SSD1306					    
void OLED_Init(void)
{ 	
 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC,D,G端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	 //A0脚接到OLED的RST脚 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //A0脚接到OLED的RST脚 

  SPI2_Init();							  
					  
	OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	OLED_WR_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
	OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
	OLED_WR_Byte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
	OLED_WR_Byte(0X00,OLED_CMD); //默认为0

	OLED_WR_Byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD); //bit2，开启/关闭
	OLED_WR_Byte(0x20,OLED_CMD); //设置内存地址模式
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WR_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WR_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]配置
		 
	OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WR_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	 
	OLED_Clear();
}  


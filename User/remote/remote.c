#include "remote.h"
#include "app.h"

RC_Ctl_t RC_Ctl;
uint8_t sbus_rx_buffer[25];
/********************************************************************************************
*函数名称：void Remote_Init(void)
*函数功能：遥控器初始化
*输入参数：无
*输出参数：无
********************************************************************************************/
void Remote_Init(void)
{
		//GPIO端口设置
		GPIO_InitTypeDef	GPIO_InitStructure;
		USART_InitTypeDef	USART_InitStructure;
		NVIC_InitTypeDef	NVIC_InitStructure;
		DMA_InitTypeDef		DMA_InitStructure;

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 ,ENABLE);		//使能DMA1
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART1，GPIOA时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA时钟
		//USART1_TX   PA.2
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2

		//USART1_RX	  PA.3
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

		//Usart1 NVIC 配置

		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		//USART 初始化设置
		USART_DeInit(USART2);  //复位串口1
		USART_InitStructure.USART_BaudRate = 100000;//一般设置为9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_Even;//偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx ;	//收发模式
		USART_Init(USART2, &USART_InitStructure); //初始化串口	
		USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);	 //开启USART1接收DMA
		USART_Cmd(USART2, ENABLE);                    //使能串口 

		DMA_DeInit(DMA1_Channel6);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)sbus_rx_buffer;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  
		DMA_InitStructure.DMA_BufferSize = 18;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			
		DMA_Init(DMA1_Channel6,&DMA_InitStructure);
		DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);	  //DMA通道1传输完成中断
		DMA_Cmd(DMA1_Channel6,ENABLE);
}


/********************************************************************************************
*函数名称：void DMA1_Channel5_IRQHandler(void)
*函数功能：串口DMA接收中断
*输入参数：无
*输出参数：无
********************************************************************************************/
extern float distance;
extern int Movement;
extern TurnPid  TurnPidA;
extern SpeedPid SpeedPidA;
void DMA1_Channel6_IRQHandler(void)
{
		if(DMA_GetITStatus(DMA1_IT_TC6))
		{
				DMA_ClearFlag(DMA1_IT_TC6);
				DMA_ClearITPendingBit(DMA1_IT_GL6); //清除全部中断标志
				RC_Ctl.rc.ch0 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff; //!< Channel 0
				RC_Ctl.rc.ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff; //!< Channel 1
				RC_Ctl.rc.ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | //!< Channel 2
				(sbus_rx_buffer[4] << 10)) & 0x07ff;
				RC_Ctl.rc.ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff; //!< Channel 3
				RC_Ctl.rc.s1 = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2; //!< Switch left
				RC_Ctl.rc.s2 = ((sbus_rx_buffer[5] >> 4)& 0x0003); //!< Switch right
				RC_Ctl.mouse.x = sbus_rx_buffer[6] | (sbus_rx_buffer[7] << 8); //!< Mouse X axis
				RC_Ctl.mouse.y = sbus_rx_buffer[8] | (sbus_rx_buffer[9] << 8); //!< Mouse Y axis
				RC_Ctl.mouse.z = sbus_rx_buffer[10] | (sbus_rx_buffer[11] << 8); //!< Mouse Z axis
				RC_Ctl.mouse.press_l = sbus_rx_buffer[12]; //!< Mouse Left Is Press ?
				RC_Ctl.mouse.press_r = sbus_rx_buffer[13]; //!< Mouse Right Is Press ?
				RC_Ctl.key.v = sbus_rx_buffer[14] | (sbus_rx_buffer[15] << 8); //!< KeyBoard value
			
				Movement = (RC_Ctl.rc.ch1 - 1024)*1.5;
				if(RC_Ctl.rc.s1==1)	//加入超声波
				{
						if(Movement>=0 && distance <100.0f)
								Movement=-600;
				}
				
				
				if(RC_Ctl.rc.ch0-1024>30 || RC_Ctl.rc.ch0-1024<-30)
						TurnPidA.SetSpeedSum -= (RC_Ctl.rc.ch0-1024)/14.67;
		}	  
}

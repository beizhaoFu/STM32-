#include "usart.h"
uint8_t parameter[48];
/***************************************************************
*函数名称：USART_GPIOConfiguration(void)
*函数功能：STM32串口通信IO口及时钟配置
*输入参数：无
*输出参数：无
*备注：该函数是static类型的函数。
***************************************************************/
static void GPIO_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		/*使能GPIOA和UASRT1*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//使能DMA1时钟
		/*配置USART中的Tx*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/*配置USART中的Rx*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*************************************************************
*函数名称：USART_NVICConfig(void)
*函数功能：确定USART中断的优先级
*输入：无
*输出：无
*备注：无
*************************************************************/
static void USART_NVICConfig(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}

/*************************************************************
*函数名称：USART_Config(void)
*函数功能：初始化USART
*输入：无
*输出：无
*备注：无
*************************************************************/
static void USART_Config(void)
{
		USART_InitTypeDef USART_InitStructure;
		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART1,&USART_InitStructure);
		USART_Cmd(USART1, ENABLE);
		/*运行此命令立即执行DMA传输*/
		USART_DMACmd(USART1, USART_DMAReq_Rx,ENABLE);
}
/*************************************************************
*函数名称：USART_DMAConfig()
*函数功能：配置DMA
*输入：无
*输出：无
*************************************************************/
static void USART_DMAConfig(void)
{
		DMA_InitTypeDef DMA_InitStructure;
		DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_Base;//将USART_DR作为外设基地址
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)parameter;//设置内存地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//以外设作为数据的来源
		DMA_InitStructure.DMA_BufferSize = 48;//设置数据传输的长度
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不变
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址自增
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//DMA传输数据大小
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//DMA传输数据大小
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA优先级高
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//禁止内存到内存的传输
		DMA_Init(DMA1_Channel5, &DMA_InitStructure);//对DMA初始化
		DMA_ITConfig(DMA1_Channel5,DMA_IT_TC, ENABLE);//使能或者失能指定的通道x中断(DMA1 通道2)
		DMA_Cmd(DMA1_Channel5,ENABLE);
}
/*************************************************************
*函数名称：USART_Init(void)
*函数功能：初始化USART
*输入：无
*输出：无
*调用函数：void GPIO_Configuration(void)void USART_Config(void)
*************************************************************/
void USARTInit(void)
{
		GPIO_Configuration();
		USART_Config();
		USART_DMAConfig();
		USART_NVICConfig();
}
//串口输出函数
int fputc(int ch,FILE *f)
{
		USART_SendData(USART1, (uint8_t) ch);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
		return ch;
}
//串口接受函数
int fgetc(FILE *f)
{
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET);
		return (int)USART_ReceiveData(USART1);
}

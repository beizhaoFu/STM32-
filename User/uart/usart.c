#include "usart.h"
uint8_t parameter[48];
/***************************************************************
*�������ƣ�USART_GPIOConfiguration(void)
*�������ܣ�STM32����ͨ��IO�ڼ�ʱ������
*�����������
*�����������
*��ע���ú�����static���͵ĺ�����
***************************************************************/
static void GPIO_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		/*ʹ��GPIOA��UASRT1*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//ʹ��DMA1ʱ��
		/*����USART�е�Tx*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/*����USART�е�Rx*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*************************************************************
*�������ƣ�USART_NVICConfig(void)
*�������ܣ�ȷ��USART�жϵ����ȼ�
*���룺��
*�������
*��ע����
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
*�������ƣ�USART_Config(void)
*�������ܣ���ʼ��USART
*���룺��
*�������
*��ע����
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
		/*���д���������ִ��DMA����*/
		USART_DMACmd(USART1, USART_DMAReq_Rx,ENABLE);
}
/*************************************************************
*�������ƣ�USART_DMAConfig()
*�������ܣ�����DMA
*���룺��
*�������
*************************************************************/
static void USART_DMAConfig(void)
{
		DMA_InitTypeDef DMA_InitStructure;
		DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_Base;//��USART_DR��Ϊ�������ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)parameter;//�����ڴ��ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//��������Ϊ���ݵ���Դ
		DMA_InitStructure.DMA_BufferSize = 48;//�������ݴ���ĳ���
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ����
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ����
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//DMA�������ݴ�С
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//DMA�������ݴ�С
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA���ȼ���
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//��ֹ�ڴ浽�ڴ�Ĵ���
		DMA_Init(DMA1_Channel5, &DMA_InitStructure);//��DMA��ʼ��
		DMA_ITConfig(DMA1_Channel5,DMA_IT_TC, ENABLE);//ʹ�ܻ���ʧ��ָ����ͨ��x�ж�(DMA1 ͨ��2)
		DMA_Cmd(DMA1_Channel5,ENABLE);
}
/*************************************************************
*�������ƣ�USART_Init(void)
*�������ܣ���ʼ��USART
*���룺��
*�������
*���ú�����void GPIO_Configuration(void)�vvoid USART_Config(void)
*************************************************************/
void USARTInit(void)
{
		GPIO_Configuration();
		USART_Config();
		USART_DMAConfig();
		USART_NVICConfig();
}
//�����������
int fputc(int ch,FILE *f)
{
		USART_SendData(USART1, (uint8_t) ch);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
		return ch;
}
//���ڽ��ܺ���
int fgetc(FILE *f)
{
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET);
		return (int)USART_ReceiveData(USART1);
}

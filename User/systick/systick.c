#include "systick.h"

/******************************************************************
*��������:void RCC_HSE_Config(void)
*�������ܣ��޸�ϵͳʱ��
*���룺��
*�������
******************************************************************/
#ifdef REVISE_SYSTICK
void RCC_HSE_Config(void)
{
		RCC_DeInit();//��RCC�Ĵ�������Ϊȱʡֵ
		RCC_HSEConfig(RCC_HSE_ON);//��HSE
		if(RCC_WaitForHSEStartUp()==SUCCESS)//���HSE�Ƿ��ȶ�
		{
				RCC_HCLKConfig(RCC_SYSCLK_Div1);//��HCLKʱ�ӵ���ϵͳʱ��
				RCC_PCLK1Config(RCC_HCLK_Div1);//��PCLK1ʱ�ӵ���HCLK
				RCC_PCLK2Config(RCC_HCLK_Div2);//PCLK2ʱ�ӵ���HCLK��һ��
				RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);//���໷��HSE����9��Ƶ
				RCC_PLLCmd(ENABLE);//ʹ��PLL
				while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);//���PLL�Ƿ�׼������			
				RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//��PLLʱ����Ϊϵͳʱ��
				while(RCC_GetSYSCLKSource()!=0x08);//��һ������Ƿ��Ѿ���PLL��Ϊϵͳʱ�ӣ�������ǽ�������ѭ��
		}

}
#endif
/******************************************************************
*��������:void delay_us(uint16_t uns)
*�������ܣ�ϵͳʵ�־�ȷ��ʱ1us
*���룺uns
*�������
******************************************************************/
void delay_us(uint16_t uns)
{
		uint32_t temp;
		SysTick->LOAD=9*uns;//���Զ�ת�ؼĴ�����װ��ֵ
		SysTick->VAL=0x00;//��ռ����Ĵ���������ռĴ�����ʱ��LOAD�е�ֵ���Զ�װ�ص�VAL��
		SysTick->CTRL=0x01;//��ϵͳ��ʱ��������ʱ��ΪHCLK/8
		do
		{
				temp=SysTick->CTRL;//��ȡCTRL�е�ֵ��ɨ���־λ�Ƿ���λ
		}while((temp&0x01)&&(!(temp&(1<<16))));
		SysTick->VAL=0x00;//���VAL�Ĵ���
		SysTick->CTRL=0x00;//�رն�ʱ��
}

/******************************************************************
*��������:void delay_ms(uint16_t ums)
*�������ܣ�ϵͳʵ�־�ȷ��ʱ1ms
*���룺uns
*�������
******************************************************************/
void delay_ms(uint16_t ums)
{
		uint32_t temp;
		SysTick->LOAD=9000*ums;//���Զ�ת�ؼĴ�����װ��ֵ
		SysTick->VAL=0x00;//��ռ����Ĵ���������ռĴ�����ʱ��LOAD�е�ֵ���Զ�װ�ص�VAL��
		SysTick->CTRL=0x01;//��ϵͳ��ʱ��������ʱ��ΪHCLK/8
		do
		{
				temp=SysTick->CTRL;//��ȡCTRL�е�ֵ��ɨ���־λ�Ƿ���λ
		}while((temp&0x01)&&(!(temp&(1<<16))));
		SysTick->VAL=0x00;//���VAL�Ĵ���
		SysTick->CTRL=0x00;//�رն�ʱ��
}

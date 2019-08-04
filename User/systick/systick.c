#include "systick.h"

/******************************************************************
*函数名称:void RCC_HSE_Config(void)
*函数功能：修改系统时钟
*输入：无
*输出：无
******************************************************************/
#ifdef REVISE_SYSTICK
void RCC_HSE_Config(void)
{
		RCC_DeInit();//将RCC寄存器设置为缺省值
		RCC_HSEConfig(RCC_HSE_ON);//打开HSE
		if(RCC_WaitForHSEStartUp()==SUCCESS)//检查HSE是否稳定
		{
				RCC_HCLKConfig(RCC_SYSCLK_Div1);//将HCLK时钟等于系统时钟
				RCC_PCLK1Config(RCC_HCLK_Div1);//将PCLK1时钟等于HCLK
				RCC_PCLK2Config(RCC_HCLK_Div2);//PCLK2时钟等于HCLK的一半
				RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);//锁相环将HSE进行9倍频
				RCC_PLLCmd(ENABLE);//使能PLL
				while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);//检查PLL是否准备就绪			
				RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//将PLL时钟作为系统时钟
				while(RCC_GetSYSCLKSource()!=0x08);//进一步检查是否已经将PLL作为系统时钟，如果不是将进入死循环
		}

}
#endif
/******************************************************************
*函数名称:void delay_us(uint16_t uns)
*函数功能：系统实现精确延时1us
*输入：uns
*输出：无
******************************************************************/
void delay_us(uint16_t uns)
{
		uint32_t temp;
		SysTick->LOAD=9*uns;//向自动转载寄存器中装载值
		SysTick->VAL=0x00;//清空计数寄存器，在清空寄存器的时候LOAD中的值会自动装载到VAL中
		SysTick->CTRL=0x01;//打开系统定时器，并且时钟为HCLK/8
		do
		{
				temp=SysTick->CTRL;//读取CTRL中的值，扫描标志位是否置位
		}while((temp&0x01)&&(!(temp&(1<<16))));
		SysTick->VAL=0x00;//清空VAL寄存器
		SysTick->CTRL=0x00;//关闭定时器
}

/******************************************************************
*函数名称:void delay_ms(uint16_t ums)
*函数功能：系统实现精确延时1ms
*输入：uns
*输出：无
******************************************************************/
void delay_ms(uint16_t ums)
{
		uint32_t temp;
		SysTick->LOAD=9000*ums;//向自动转载寄存器中装载值
		SysTick->VAL=0x00;//清空计数寄存器，在清空寄存器的时候LOAD中的值会自动装载到VAL中
		SysTick->CTRL=0x01;//打开系统定时器，并且时钟为HCLK/8
		do
		{
				temp=SysTick->CTRL;//读取CTRL中的值，扫描标志位是否置位
		}while((temp&0x01)&&(!(temp&(1<<16))));
		SysTick->VAL=0x00;//清空VAL寄存器
		SysTick->CTRL=0x00;//关闭定时器
}

#ifndef SYSTICK_H
#define SYSTICK_H

#include <stm32f10x.h>
//#define REVISE_SYSTICK
/*������޸�ϵͳʱ����ȡ�������ע��*/
void RCC_HSE_Config(void);
void delay_us(uint16_t uns);
void delay_ms(uint16_t ums);

#endif

#ifndef USART_H
#define USART_H

#include "stm32f10x.h"
#include <stdio.h>
#define USART_DR_Base 0x40013804

union dataConvert
{
		uint8_t table[8];
		double num;
};

void USARTInit(void);
int fputc(int ch,FILE *f);
int fgetc(FILE *f);

#endif

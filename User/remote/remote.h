#ifndef REMOTE_H
#define REMOTE_H

#include "stm32f10x.h"
#include <stdio.h>
//------------定义结构体----------------
typedef struct
{  
	struct
	{
			uint16_t ch0;
			uint16_t ch1;
			uint16_t ch2;
			uint16_t ch3;
			uint8_t s1;
			uint8_t s2;
	}rc;
	struct
	{
			int16_t x;
			int16_t y;
			int16_t z;
			uint8_t press_l;
			uint8_t press_r;
	}mouse;
  struct
	{
			uint16_t v;
	}key;
	
}RC_Ctl_t;
//-----------函数定义--------------
void Remote_Init(void);
#endif

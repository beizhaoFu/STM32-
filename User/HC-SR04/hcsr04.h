#ifndef HC_SR04_H
#define HC_SR04_H

#include "stm32f10x.h"
#include "systick.h"

#define HCSR_GPIO_CLK    	RCC_APB2Periph_GPIOA
#define TRIG_GPIO     GPIOA
#define TRIG_PIN			GPIO_Pin_4
#define TRIG_HIGH()		GPIO_SetBits(TRIG_GPIO, TRIG_PIN)
#define TRIG_LOW()		GPIO_ResetBits(TRIG_GPIO, TRIG_PIN)

#define ECHO_GPIO     GPIOA
#define ECHO_PIN			GPIO_Pin_5
#define ECHO_READ()   GPIO_ReadInputDataBit(ECHO_GPIO, ECHO_PIN)

#define HCSR_TIM_CLK	RCC_APB1Periph_TIM5
#define HCSR_TIM			TIM5

void HCSR04_Init(void);
float HCSR04_WaitToConvert(void);

#endif

#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f10x.h"

void MOTOR_configuration(void);
void MOTOR_SetFrenquency(uint32_t Fren);
void MOTOR2_Rotation(int pwm);
void MOTOR4_Rotation(int pwm);

#endif

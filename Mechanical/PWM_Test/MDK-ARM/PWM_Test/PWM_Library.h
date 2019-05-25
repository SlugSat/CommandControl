#ifndef PWM_LIBRARY_H
#define PWM_LIBRARY_H

#include "main.h"

#define CLOCK_FREQ 1000000 // in Hz

TIM_HandleTypeDef htim4;

void Set_Duty_Cycle(uint16_t duty_cycle);
void Set_Frequency(uint16_t freq);
void Set_Counter_Period(uint16_t per);
void Set_Pulse(uint16_t pulse);

#endif
#ifndef PWM_LIBRARY_H
#define PWM_LIBRARY_H

#include "main.h"

#define CLOCK_FREQ 1000000 // in Hz

void Set_Duty_Cycle(TIM_HandleTypeDef *htim4, uint16_t duty_cycle);
void Set_Frequency(TIM_HandleTypeDef *htim4, uint16_t freq);
void Set_Counter_Period(TIM_HandleTypeDef *htim4, uint16_t per);
void Set_Pulse(TIM_HandleTypeDef *htim4, uint16_t pulse);

#endif

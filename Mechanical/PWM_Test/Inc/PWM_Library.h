#ifndef PWM_LIBRARY_H
#define PWM_LIBRARY_H

#include "main.h"

#define CLOCK_FREQ 1000000 // Frequency of Timer 4


//======================================================================
// Sets duty cycle for PWM signal assuming clock frequency is 1 MHz
// (note: the duty cycle is in percent so the range is 0-100
//======================================================================
void PWM_Set_Duty_Cycle(TIM_HandleTypeDef *htim, uint16_t duty_cycle);

//======================================================================
// Sets frequency for the PWM signal. frequency is in Hz
//======================================================================
void PWM_Set_Frequency(TIM_HandleTypeDef *htim, uint16_t freq);

//======================================================================
// Sets counter period value based on clock frequency of 1MHz
//======================================================================
void PWM_Set_Counter_Period(TIM_HandleTypeDef *htim, uint16_t per);

//======================================================================
// Sets compare value that decides duty cycle of PWM signal
//======================================================================
void PWM_Set_Pulse(TIM_HandleTypeDef *htim, uint16_t pulse);


#endif

#ifndef ACTUATOR_LIB_H
#define ACTUATOR_LIB_H

#include "main.h"

#define TIM_RPM_CLK 500000.0

void rw_init(TIM_HandleTypeDef *htim_pwm, 	// PWM timer
						 TIM_HandleTypeDef *htim_rpm);	// RPM timer

void rw_start_wheel (TIM_HandleTypeDef *htim_rpm);

void rw_get_speed (TIM_HandleTypeDef *htim_rpm, float *speed);

void rw_set_speed (TIM_HandleTypeDef *htim_rpm, float percent_speed);


#endif

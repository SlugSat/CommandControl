#include "Actuator_Lib.h"
#include "PWM_Library.h"

static TIM_HandleTypeDef *htim;

void rw_init(TIM_HandleTypeDef *htim_pwm, TIM_HandleTypeDef *htim_rpm)
{
	// start timer for PWM
	HAL_TIM_Base_Start(htim_pwm);
	
	// set PWM signal frequency
	PWM_Set_Frequency(htim_pwm, 100);
	
	// starts PWM signal generation
	if (HAL_TIM_PWM_Start(htim_pwm, TIM_CHANNEL_1) != HAL_OK) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
	
	// start timer for RPM calculations
	htim_rpm->Instance->CCER |= 1;
	HAL_TIM_Base_Start(htim_rpm);
}

void rwget_speed (TIM_HandleTypeDef *htim_rpm, float *speed)
{
	
	// local variable to store pulse time
	float xor_pulse_time;
	
	// time for rotation (us) assuming pulses are equidistant
	xor_pulse_time = 6.0 * htim_rpm->Instance->CCR1; 	// time to rotate in us
	xor_pulse_time = xor_pulse_time 			// (us/rev)
									 * (1.0/TIM_RPM_CLK) 	// (s/us)
									 * (1.0/60);					// (min/s)
		
	// calculate rpm
	*speed = (1.0/xor_pulse_time)*3.0;
}

void set_speed (TIM_HandleTypeDef *htim_rpm, float percent_speed)
{
	// set duty cycle accordingly
	PWM_Set_Duty_Cycle(htim_rpm, percent_speed, TIM_CHANNEL_1);
}

#include "PWM_Library.h"

void PWM_Set_Duty_Cycle(TIM_HandleTypeDef *htim, uint16_t duty_cycle)
{
	if ((htim->Instance->CCR1 = (uint32_t)(((float)duty_cycle/100.0)*(float)htim->Init.Period)) == 0)
	{
		htim->Instance->CCR1 = 1;
	}
}

void PWM_Set_Frequency(TIM_HandleTypeDef *htim, uint16_t freq)
{
	PWM_Set_Counter_Period(htim, CLOCK_FREQ/freq);
}

void PWM_Set_Counter_Period(TIM_HandleTypeDef *htim, uint16_t per)
{
	PWM_Set_Pulse(htim, per);
	
	htim->Init.Period = per;
	if (HAL_TIM_Base_Init(htim) != HAL_OK)
  {
    Error_Handler();
  }
}

void PWM_Set_Pulse(TIM_HandleTypeDef *htim, uint16_t new_per)
{
	htim->Instance->CCR1 = (uint32_t)(((float)htim->Instance->CCR1/(float)htim->Init.Period)*(float)new_per);
}

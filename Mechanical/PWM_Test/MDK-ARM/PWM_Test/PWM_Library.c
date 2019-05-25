#include "../Src/PWM_Library.h"

void Set_Duty_Cycle(uint16_t duty_cycle)
{
	htim4.Instance->CCR1 = (uint32_t)(((float)duty_cycle/100.0)*(float)htim4.Init.Period);
}

void Set_Frequency(uint16_t freq)
{
	Set_Counter_Period(CLOCK_FREQ/freq);
}

void Set_Counter_Period(uint16_t per)
{
	Set_Pulse(per);
	
	htim4.Init.Period = per;
	if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
	//htim4.Instance->CNT = per;
}

void Set_Pulse(uint16_t new_per)
{
	htim4.Instance->CCR1 = (uint32_t)(((float)htim4.Instance->CCR1/(float)htim4.Init.Period)*(float)new_per);
		//htim4.Instance->CCR1 = 250;

}
#include "Temp_Mon.h"

//Returns the current output of the ADC
uint32_t Poll_Temp(ADC_HandleTypeDef hadc1){
	uint32_t ADC = 0;
	HAL_ADC_Start(&hadc1);
	if(HAL_ADC_PollForConversion(&hadc1, 5) == HAL_OK){
		ADC = HAL_ADC_GetValue(&hadc1);
	}
	return ADC;
}

//Returns the rolling average of the last N (ROLLING_AVERAGE) readings of the ADC
//As such this requires the first N returns be ignored
uint32_t Smooth_Poll_Temp(ADC_HandleTypeDef hadc1){
	static uint32_t ADC[ROLLING_AVERAGE] = {0};
	uint32_t ADV_Av = 0;
	
	for(int i=ROLLING_AVERAGE-1; i>0; i--){
		ADC[i] = ADC[i-1];
	}
	
	HAL_ADC_Start(&hadc1);
	if(HAL_ADC_PollForConversion(&hadc1, 5) == HAL_OK){
		ADC[0] = HAL_ADC_GetValue(&hadc1);
	}
	
	ADV_Av = 0;
	for(int i=0; i<ROLLING_AVERAGE; i++){
		ADV_Av += ADC[i];
	}
	
	return ADV_Av/ROLLING_AVERAGE;
}

float ADC_To_C(uint32_t ADC_Val){
	return (ADC_Val - OFFSET) * SCALE;
}

float ADC_To_F(uint32_t ADC_Val){
	return ((ADC_Val - OFFSET) * SCALE * 9/5) + 32;
}

float ADC_To_K(uint32_t ADC_Val){
	return ((ADC_Val - OFFSET) * SCALE) + 273.15;
}

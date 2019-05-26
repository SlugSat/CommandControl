#include "main.h"

//Calculated for 3.3v reference on ADC
//Not well calibrated
#define OFFSET 3415
#define SCALE .074

#define ROLLING_AVERAGE 5

uint32_t Poll_Temp(ADC_HandleTypeDef hadc1);

uint32_t Smooth_Poll_Temp(ADC_HandleTypeDef hadc1);

float ADC_To_C(uint32_t ADC_Val);

float ADC_To_F(uint32_t ADC_Val);

float ADC_To_K(uint32_t ADC_Val);
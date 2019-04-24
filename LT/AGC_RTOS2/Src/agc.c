/*
 * agc.c
 *
 *  Created on: Mar 17, 2019
 *      Author: thomas
 */

#include "main.h"
#include "dac.h"
#include "adc.h"
#include "agc.h"

typedef enum {
	STATE_IDLE,
	STATE_INCREASE_VGA,
	STATE_DECREASE_VGA,
	STATE_INCREASE_VA,
} AgcState_t;

static AgcState_t CurrentState = STATE_IDLE;

/* These are based on Excel calculations */
volatile float InputV = 0;
static  float AGC_DetectorToRSSI(uint32_t DetectorValue) {
	InputV = (3.3 * DetectorValue / 4095.);
	return (52.4748346 * InputV) - 34.33916738;
	//return (12.4 * InputV) + 0.117;
}

static uint32_t AGC_VGAToDAC(float VgaValue) {
	float val = (0.011479857 * (VgaValue)) + 1.09158902; // diode drop of transistor included
	//float val = (0.89924 * VgaValue) - 0.474323;
	return (4096 * (val + 0.7) / 3.3);
}

static uint32_t AGC_VAToDAC(float VaValue) {
	float val = (0.029660982 * VaValue) + 2.543671697; // -50dBm at 1.8Vcc, diode drop of transistor
	return (4096 * (val + 0.7) / 3.3);
}

#define AGC_SETPOINT (-30.0) // dBm
#define VGA_MAX (AGC_VGAToDAC(50 /* gain */))
#define VGA_MIN (AGC_VGAToDAC(-20 /* gain */))
#define VA_MAX (AGC_VAToDAC(-3.5 /* dB */))
#define VA_MIN (AGC_VAToDAC(-20 /* dB */))

#define AGC_DELAY_VGA 1000 // ms
#define AGC_DELAY_VA 1000 // ms

#define min(a, b) ((a) > (b) ? (b) : (a))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(x) ((x) > 0 ? (x) : -(x))

static volatile uint32_t VgaValue, VaValue, DetectorValue;

static void AGC_SetOutputs(void) {
	//while(HAL_ADC_PollForConversion(&hadc, 1000) != HAL_OK);
	HAL_DAC_SetValue(&hdac, DAC_CH_VA, DAC_ALIGN_12B_R, VaValue);
	HAL_DAC_SetValue(&hdac, DAC_CH_VGA, DAC_ALIGN_12B_R, VgaValue);
}

void AGC_Init(void) {
	// Set VA to min
	VaValue = VA_MIN;

	// Set VGA to max
	VgaValue = VGA_MAX;

	// Output the AGC initial values
	AGC_SetOutputs();
	
	// Init the ADC correctly
	HAL_ADC_Start(&hadc);
	DAC->CR |= DAC_CR_EN1;
	DAC->CR |= DAC_CR_EN2;
}

static float AGC_MeasureRSSI(void) {
	HAL_ADC_Start(&hadc);
	DetectorValue = HAL_ADC_GetValue(&hadc);
	return AGC_DetectorToRSSI(DetectorValue);
}

void AGC_DoEvent(void) {
	float rssi = AGC_MeasureRSSI();
	volatile float NeededGain = 0; // = abs(AGC_SETPOINT - rssi);

	switch (CurrentState) {
	case STATE_IDLE:
		my_printf("[%d] RSSI: %f, Needed Gain: %f\nDetector: %lu     VGA:%lu VA:%lu\n\n",
				CurrentState, rssi, NeededGain,
				DetectorValue, VgaValue, VaValue);
		if (rssi < AGC_SETPOINT && VgaValue < VGA_MAX) {
			CurrentState = STATE_INCREASE_VGA;
		} else if (rssi > AGC_SETPOINT && VgaValue > VGA_MIN) {
			CurrentState = STATE_DECREASE_VGA;
		} else if (rssi > AGC_SETPOINT && VgaValue <= VGA_MIN) {
			CurrentState = STATE_INCREASE_VA;
		}
		break;
	case STATE_INCREASE_VGA:
		NeededGain = AGC_SETPOINT - rssi;
		VgaValue = AGC_VGAToDAC(NeededGain); //max(min(, VGA_MAX), VGA_MIN);
		my_printf("[%d] RSSI: %f, Needed Gain: %f\nDetector: %lu     VGA:%lu VA:%lu\n\n",
				CurrentState, rssi, NeededGain,
				DetectorValue, VgaValue, VaValue);
		AGC_SetOutputs();

		HAL_Delay(AGC_DELAY_VGA);

		my_printf("[%d After Delay] RSSI: %f, Needed Gain: %f\nDetector: %lu\tVGA:%lu VA:%lu\n\n",
			CurrentState, rssi, NeededGain,
			DetectorValue, VgaValue, VaValue);

		rssi = AGC_MeasureRSSI();
		if (rssi >= AGC_SETPOINT || VgaValue >= VGA_MAX) {
			CurrentState = STATE_IDLE;
		}
		break;
	case STATE_DECREASE_VGA:
		NeededGain = AGC_SETPOINT - rssi;
		VgaValue = AGC_VGAToDAC(NeededGain); //max(min(, VGA_MAX), VGA_MIN);
		my_printf("[%d] RSSI: %f, Needed Gain: %f\nDetector: %lu     VGA:%lu VA:%lu\n\n",
				CurrentState, rssi, NeededGain,
				DetectorValue, VgaValue, VaValue);
		AGC_SetOutputs();

		HAL_Delay(AGC_DELAY_VGA);

		rssi = AGC_MeasureRSSI();
		if (rssi <= AGC_SETPOINT) {
			CurrentState = STATE_IDLE;
		} else if (rssi >= AGC_SETPOINT && VgaValue <= VGA_MIN) {
			CurrentState = STATE_INCREASE_VA;
		}
		break;
	case STATE_INCREASE_VA:
		NeededGain = AGC_SETPOINT - rssi;
		VaValue = AGC_VAToDAC(NeededGain); //max(min(, VA_MAX), VA_MIN);
		my_printf("[%d] RSSI: %f, Needed Gain: %f\nDetector: %lu     VGA:%lu VA:%lu\n\n",
				CurrentState, rssi, NeededGain,
				DetectorValue, VgaValue, VaValue);
		AGC_SetOutputs();

		HAL_Delay(AGC_DELAY_VA);
		my_printf("[%d After Delay] RSSI: %f, Needed Gain: %f\nDetector: %lu\tVGA:%lu VA:%lu\n\n",
			CurrentState, rssi, NeededGain,
			DetectorValue, VgaValue, VaValue);

		rssi = AGC_MeasureRSSI();
		if (rssi <= AGC_SETPOINT || VaValue >= VA_MAX) {
			CurrentState = STATE_IDLE;
		}
		break;
	default:
		Error_Handler(); // shouldn't happen
		break;
	}
}

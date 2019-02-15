#include "PowerModes.h"


#ifndef GLOBALS
#define GLOBALS
/* Global variables controlling events and signals */
uint8_t stable = 0;
uint8_t die = 0;
uint8_t batteryLevel = 0;
uint8_t scienceEvent = 0;
uint8_t solarVector = 0; 
#endif

/* State transition for Detumble */
uint8_t Detumble_Transition(void)
{
	while(1)
	{
		if (die == FALSE)
		{
			if (stable == TRUE)
			{
				if ((solarVector == OFF) && (batteryLevel > ULTRA_LOW_BATT))
				{
					return Eclipse;
				}
				else if ((solarVector == ON) && (batteryLevel > LOW_BATT))
				{
					return Normal;
				}
				else if ((solarVector == ON) && (batteryLevel < LOW_BATT) && (batteryLevel > ULTRA_LOW_BATT))
				{
					return LowPower;
				}
			}
		}			
	}
	
}

/* State transition for Eclipse */
uint8_t Eclipse_Transition(void)
{
	
	
}

/* State transition for Normal */
uint8_t Normal_Transition(void)
{
	
	
}

/* State transition for LowPower */
uint8_t LowPower_Transition(void)
{
	
	
}

/* State transition for UltraLowPower */
uint8_t UltraLowPower_Transition(void)
{

	
}

/* State transition for ScienceOnly */
uint8_t ScienceOnly_Transition(void)
{
	
	
}
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


uint8_t set_stable(uint8_t change)
{
	stable = change;
}
uint8_t set_die(uint8_t change)
{
	die = change;
}
uint8_t set_science(uint8_t change)
{
	batteryLevel = change;
}
uint8_t set_solar(uint8_t change)
{
	scienceEvent = change;
}
uint8_t set_power(uint8_t change)
{
	solarVector = change;
}


/* State transition for Detumble */
uint8_t Detumble_Transition(void)
{
	while(1)
	{
		// Grab mutex
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
				else if ((solarVector == ON) && (batteryLevel < ULTRA_LOW_BATT))
				{
					return UltraLowPower;
				}
			}
		}
		else
		{
			return Kill;
		}
		// Release mutex
	}
	return 255; //Error
}

/* State transition for Eclipse */
uint8_t Eclipse_Transition(void)
{
	while(1)
	{
		// Grab mutex
		if (die == FALSE)
		{
			if (scienceEvent == FALSE)
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
				else if ((solarVector == ON) && (batteryLevel < ULTRA_LOW_BATT))
				{
					return UltraLowPower;
				}
			}
			else
			{
				return ScienceOnly;
			}
		}
		else
		{
			return Kill;
		}
		// Release mutex
	}
	return 255; //Error
	
}

/* State transition for Normal */
uint8_t Normal_Transition(void)
{
	while(1)
	{
		// Grab mutex
		if (die == FALSE)
		{
			if (scienceEvent == FALSE)
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
				else if ((solarVector == ON) && (batteryLevel < ULTRA_LOW_BATT))
				{
					return UltraLowPower;
				}
			}
			else
			{
				return ScienceOnly;
			}
		}
		else
		{
			return Kill;
		}
		// Release mutex
	}
	return 255; //Error
	
}

/* State transition for LowPower */
uint8_t LowPower_Transition(void)
{
	while(1)
	{
		// Grab mutex
		if (die == FALSE)
		{
			if (scienceEvent == FALSE)
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
				else if ((solarVector == ON) && (batteryLevel < ULTRA_LOW_BATT))
				{
					return UltraLowPower;
				}
			}
			else
			{
				return ScienceOnly;
			}
		}
		else
		{
			return Kill;
		}
		// Release mutex
	}
	return 255; //Error
	
}

/* State transition for UltraLowPower */
uint8_t UltraLowPower_Transition(void)
{
	while(1)
	{
		// Grab mutex
		if (die == FALSE)
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
			else if ((solarVector == ON) && (batteryLevel < ULTRA_LOW_BATT))
			{
				return UltraLowPower;
			}
		}
		else
		{
			return Kill;
		}		
		// Release mutex
	}
	return 255; //Error
	
}

/* State transition for ScienceOnly */
uint8_t ScienceOnly_Transition(void)
{
	while(1)
	{
		// Grab mutex
		if (die == FALSE)
		{
			if (batteryLevel < ULTRA_LOW_BATT)
			{
				return UltraLowPower;
			}
			
			if (scienceEvent == FALSE)
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
				else if ((solarVector == ON) && (batteryLevel < ULTRA_LOW_BATT))
				{
					return UltraLowPower;
				}
			}
		}
		else
		{
			return Kill;
		}
		// Release mutex
	}
	return 255; //Error
	
}
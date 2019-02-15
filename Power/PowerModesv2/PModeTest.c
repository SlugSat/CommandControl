#include "PowerModes.h"

#define DEBUG (1)

int main(int argc, char **argv)
{
	/* Initialize Variables */
	States state;
	system_function functions;

	/* Initialize the starting state of each of the systems that need power */
	Initialize_Functions(&functions);
	
	/* Set Initial state */
	state = Detumble;
	
	/* Enter the state machine */
	while(1)
	{
		switch (state)
		{
			/* In Detumble mode */
			case (Detumble): 
				if (DEBUG) printf("\nEntering Detumble mode\n");
				Set_PowerModes(&functions, Detumble);
				Print_System_Settings(&functions);	// Used for testing and debugging
				// Set the pins high for each rail that should be one here //
				state = Transition(Detumble);
				break;
			/* In Kill mode */
			case (Kill): 
				if (DEBUG) printf("\nEntering Kill mode\n");
				Set_PowerModes(&functions, Kill);
				Print_System_Settings(&functions);	// Used for testing and debugging
				// Set the pins high for each rail that should be one here //
				state = Transition(Kill);
				break;
			/* In Normal mode */
			case (Normal): 
				if (DEBUG) printf("\nEntering Normal mode\n");
				Set_PowerModes(&functions, Normal);
				Print_System_Settings(&functions);	// Used for testing and debugging
				// Set the pins high for each rail that should be one here //
				state = Transition(Normal);
				break;
			/* In UltraLowPower mode */	
			case (UltraLowPower): 
				if (DEBUG) printf("\nEntering UltraLowPower mode\n");
				Set_PowerModes(&functions, UltraLowPower);
				Print_System_Settings(&functions);	// Used for testing and debugging
				// Set the pins high for each rail that should be one here //
				state = Transition(UltraLowPower);
				break;
			/* In LowPower mode */
			case (LowPower): 
				if (DEBUG) printf("\nEntering LowPower mode\n");
				Set_PowerModes(&functions, LowPower);
				Print_System_Settings(&functions);	// Used for testing and debugging
				// Set the pins high for each rail that should be one here //
				state = Transition(LowPower);
				break;
			/* In Eclipse mode */
			case (Eclipse): 
				if (DEBUG) printf("\nEntering Eclipse mode\n");
				Set_PowerModes(&functions, Eclipse);
				Print_System_Settings(&functions);	// Used for testing and debugging
				// Set the pins high for each rail that should be one here //
				state = Transition(Eclipse);
				break;
			/* In ScienceOnly mode */
			case (ScienceOnly): 
				if (DEBUG) printf("\nEntering ScienceOnly mode\n");
				Set_PowerModes(&functions, ScienceOnly);
				Print_System_Settings(&functions);	// Used for testing and debugging
				// Set the pins high for each rail that should be one here //
				state = Transition(ScienceOnly);
				break;
			/* An error occurred */
			default:
				fprintf(stderr, "\nInvalid state in the state machine reached...\nExiting\n");
				return -1;
		}
	}
	return 0;
}

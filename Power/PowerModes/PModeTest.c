#include "PowerModes.h"

int main(int argc, char **argv)
{
	/* Initialize Variables */
	enum State state;
	system_function functions;

	/* Initialize the starting state of each of the systems that need power */
	Initialize_Functions(&functions);
	
	/* Set Initial state */
	state = StartUp;
	
	/* Enter the state machine */
	while(1)
	{
		switch (state)
		{
			/* In StartUp mode */
			case (StartUp): 
				printf("\nEntering StartUp mode\n");
				Set_Functions(&functions, StartUp);
				Print_System_Settings(&functions);	// Used for testing and debugging
				state = Transition(StartUp);
				break;
			/* In Shutdown mode */
			case (Shutdown): 
				printf("\nEntering Shutdown mode\n");
				Set_Functions(&functions, Shutdown);
				Print_System_Settings(&functions);	// Used for testing and debugging
				state = Transition(Shutdown);
				break;
			/* In Normal mode */
			case (Normal): 
				printf("\nEntering Normal mode\n");
				Set_Functions(&functions, Normal);
				Print_System_Settings(&functions);	// Used for testing and debugging
				state = Transition(Normal);
				break;
			/* In PowerSave mode */	
			case (PowerSave): 
				printf("\nEntering PowerSave mode\n");
				Set_Functions(&functions, PowerSave);
				Print_System_Settings(&functions);	// Used for testing and debugging
				state = Transition(PowerSave);
				break;
			/* In TelemetryOnly mode */
			case (TelemetryOnly): 
				printf("\nEntering TelemetryOnly mode\n");
				Set_Functions(&functions, TelemetryOnly);
				Print_System_Settings(&functions);	// Used for testing and debugging
				state = Transition(TelemetryOnly);
				break;
			/* In Eclipse mode */
			case (Eclipse): 
				printf("\nEntering Eclipse mode\n");
				Set_Functions(&functions, Eclipse);
				Print_System_Settings(&functions);	// Used for testing and debugging
				state = Transition(Eclipse);
				break;
			/* An error occurred */
			default:
				fprintf(stderr, "\nInvalid state in the state machine reached...\nExiting\n");
				return -1;
		}
	}
	return 0;
}

#include "PowerModes.h"

#define DEBUG (1)


void *StateMachine(void *hi)
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
		}
	}
	return NULL;
}
	
int main(int argc, char **argv)
{
	
	pthread_t thread_id[2];
	
	if (pthread_create(&thread_id[0], NULL , &StateMachine, NULL) != 0)
	{
		fprintf(stderr, "Thread not able to be created\n");
		exit(1);
	}
	
	if (pthread_create(&thread_id[1], NULL , &change_variables, NULL) != 0)
	{
		fprintf(stderr, "Thread not able to be created\n");
		exit(1);
	}
	
	// Wait for all of threads to finish processing before continuing
	for (int i = 0; i < 2; i++)
	{
		char *val;
		if (pthread_join(thread_id[i], (void **)&val) != 0)
		{
			fprintf(stderr, "Thread join error\n");
			exit(1);
		}
	}
	
	
	
	return 0;
}

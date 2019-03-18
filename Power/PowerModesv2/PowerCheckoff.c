#include "PowerModes.h"

#define DEBUG (1)


void *StateMachine(void *nil)
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

void* PowerCheckoff(void* ptr) {

	/*simulate detumble state and set inputs at varying
	  frequencies except stable*/
	for (int i = 0; i < 10; i++)
	{
		uint8_t temp = rand() % 2;

		//printf("Setting science to: %u\n", temp);
		Set_ScienceEvent(temp);
		
		temp = rand() % 101;
		//printf("Setting power to: %u\n", temp);
		Set_BatteryLevel(temp);
		
		temp = rand() % 2;
		//printf("Setting solar to: %u\n", temp);
		Set_SolarVector(temp);
		for(int k = 0; k < 100000000; k++);		
	}

	// at this point we assume we are now stable
	printf("================================================\n");
	printf("	TRANSITIONING FROM DETUMBLE TO NORMAL       \n");
	printf("================================================\n");
	fflush(stdout);
	Set_BatteryLevel(100);
	Set_SolarVector(ON);
	Set_Stable(ON);
	Set_ScienceEvent(OFF);
	Set_Die(OFF);
	for(int k = 0; k < 100000000; k++);		

//	printf("================================================\n");
//	printf("	TRANSITIONING FROM NORMAL TO ECLIPSE        \n");
//	printf("================================================\n");
	fflush(stdout);
	Set_BatteryLevel(90);
	Set_SolarVector(OFF);
	for(int k = 0; k < 100000000; k++);		

	// at this point we assume we are now stable
//	printf("================================================\n");
//	printf("	TRANSITIONING FROM ECLIPSE TO NORMAL       \n");
//	printf("================================================\n");
	fflush(stdout);
	Set_BatteryLevel(80);
	Set_SolarVector(ON);
	for(int k = 0; k < 200000000; k++);		

	// at this point we assume we are now stable
//	printf("================================================\n");
//	printf("	TRANSITIONING FROM NORMAL TO ECLIPSE        \n");
//	printf("================================================\n");
	fflush(stdout);
	Set_BatteryLevel(70);
	Set_SolarVector(OFF);
	for(int k = 0; k < 200000000; k++);		

	// at this point we assume we are now stable
//	printf("================================================\n");
//	printf("	TRANSITIONING FROM ECLIPSE TO LOW POWER     \n");
//	printf("================================================\n");
	fflush(stdout);
	Set_BatteryLevel(45);
	Set_SolarVector(ON);
	for(int k = 0; k < 200000000; k++);		

	// at this point we assume we are now stable
//	printf("================================================\n");
//	printf("	TRANSITIONING FROM LOW POWER TO ULTRA LOW   \n");
//	printf("================================================\n");
	fflush(stdout);
	Set_BatteryLevel(10);
	Set_SolarVector(ON);
	for(int k = 0; k < 200000000; k++);		

	// at this point we assume we are now stable
//	printf("================================================\n");
//	printf("	TRANSITIONING FROM ULTRA LOW TO ECLIPSE     \n");
//	printf("================================================\n");
	fflush(stdout);
	Set_BatteryLevel(25);
	Set_SolarVector(OFF);
	for(int k = 0; k < 100000000; k++);		

	// at this point we assume we are now stable
//	printf("================================================\n");
//	printf("	TRANSITIONING FROM ECLIPSE TO SCIENCE       \n");
//	printf("================================================\n");
	fflush(stdout);
	Set_BatteryLevel(50);
	Set_SolarVector(ON);
	Set_ScienceEvent(ON);
	for(int k = 0; k < 100000000; k++);		

	// at this point we assume we are now stable
//	printf("================================================\n");
//	printf("	TRANSITIONING FROM SCIENCE TO NORMAL       \n");
//	printf("================================================\n");
	fflush(stdout);
	Set_BatteryLevel(70);
	Set_SolarVector(ON);
	Set_ScienceEvent(OFF);
	for(int k = 0; k < 100000000; k++);		


	/* Normal state to eclipse */
	for (int i = 0; i < 10; i++)
	{
		uint8_t temp = rand() % 2;

		//printf("Setting science to: %u\n", temp);
		Set_ScienceEvent(temp);
		
		temp = rand() % 101;
		//printf("Setting power to: %u\n", temp);
		Set_BatteryLevel(temp);
		
		temp = rand() % 2;
		//printf("Setting solar to: %u\n", temp);
		Set_SolarVector(temp);
		for(int k = 0; k < 100000000; k++);		
	}

	// at this point we assume we are now stable
	printf("================================================\n");
	printf("	TRANSITIONING FROM RANDOM TO DIE       \n");
	printf("================================================\n");
	fflush(stdout);
	Set_Die(ON);

	pthread_exit(NULL);
}

int main(int argc, char **argv)
{

	pthread_t thread_id[2];

	if (pthread_create(&thread_id[1], NULL , &StateMachine, NULL) != 0)
	{
		fprintf(stderr, "Thread not able to be created\n");
		exit(1);
	}

	if (pthread_create(&thread_id[0], NULL , &PowerCheckoff, NULL) != 0)
	{
		fprintf(stderr, "Thread not able to be created\n");
		exit(1);
	}

	// Wait for all of threads to finish processing before continuing
	for (int i = 0; i < 2; i++)
	{
		if (i == 1)
		{
			for(int k = 0; k < 100000000; k++);		
			pthread_cancel(thread_id[1]);
		}

		char *val;
		if (pthread_join(thread_id[i], (void **)&val) != 0)
		{
			fprintf(stderr, "Thread join error\n");
			exit(1);
		}
	}



	return 0;
}

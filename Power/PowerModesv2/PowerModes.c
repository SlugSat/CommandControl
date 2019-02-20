#include "PowerModes.h"

void *change_variables(void * hi)
{
	while(1)
	{
		uint8_t temp = rand() % 2;
		printf("Setting stable to: %u\n", temp);
		set_stable(temp);
		
		temp = rand() % 2;
		//printf("Setting science to: %u\n", temp);
		set_science(temp);
		
		temp = rand() % 101;
		//printf("Setting power to: %u\n", temp);
		set_power(temp);
		
		temp = rand() % 2;
		//printf("Setting solar to: %u\n", temp);
		set_solar(temp);
		for(int i = 0; i < 1000000000; i++);		
	}
	return NULL;	
}

/* Set the intial power modes of each system */
void Initialize_Functions(system_function *functions)
{
	// Initialize everything as off except what will always be on
	functions->MechanicalSys = NoPower;
	functions->MechanicalMCU = Power; // Always On
	functions->LinearTransponderSys = NoPower;
	functions->LinearTransponderMCU = NoPower;
	functions->TelemetrySys = NoPower;
	functions->TelemetryMCU = NoPower; 
	functions->SciencePayloadSys = NoPower;
	functions->SciencePayloadMCU = NoPower;	
	return;
}

/* Sets the power mode of each of the systems */
void Set_PowerModes(system_function *function, uint8_t state)
{
	Set_Mechanical(function, state);
	Set_LinearTransponder(function, state);
	Set_Telemetry(function, state);
	Set_SciencePayload(function, state);
	return;
}

/* Set the power mode for mechanical */
void Set_Mechanical(system_function *function, uint8_t state)
{
	// Set the MechanicalSys power mode
	if (state == Detumble || state == Normal || state == LowPower)
	{
		function->MechanicalSys = Power;
	}
	else if (state == UltraLowPower || state == Eclipse)
	{
		function->MechanicalSys = LimitedPower;
	}
	else // if (state == Kill || state == ScienceOnly)
	{
		function->MechanicalSys = NoPower;
	}
	return;
}

/* Set the power mode for linear transponder */
void Set_LinearTransponder(system_function *function, uint8_t state)
{
	// Set the LinearTransponderSys power mode
	if (state == Normal)
	{
		function->LinearTransponderSys = Power;
	}
	else if (state == Eclipse)
	{
		function->LinearTransponderSys = LimitedPower;
	}
	else // if (state == Detumble || state == Kill || state == UltraLowPower || state == LowPower || state == ScienceOnly)
	{
		function->LinearTransponderSys = NoPower;
	}
	
	// Set the LinearTransponderMCU power mode
	if (state == Detumble || state == Normal || state == UltraLowPower || state == LowPower || state == Eclipse || state == ScienceOnly)
	{
		function->LinearTransponderMCU = Power;
	}
	else if (OFF)
	{
		function->LinearTransponderMCU = LimitedPower;
	}
	else // if (state == Kill)
	{
		function->LinearTransponderMCU = NoPower;
	}
	
	
	return;
}

/* Set the power mode for telemery */
void Set_Telemetry(system_function *function, uint8_t state)
{
	// Set TelemetrySys power mode
	if (state == Normal || state == LowPower)
	{
		function->TelemetrySys = Power;
	}
	else if (state == UltraLowPower || state == Eclipse)
	{
		function->TelemetrySys = LimitedPower;
	}
	else // if (state == Detumble || state == Kill || state == ScienceOnly)
	{
		function->TelemetrySys = NoPower;
	}
	
	// Set TelemetryMCU power mode
	if (state == Detumble || state == Normal || state == UltraLowPower || state == LowPower || state == Eclipse || state == ScienceOnly)
	{
		function->TelemetryMCU = Power;
	}
	else if (OFF)
	{
		function->TelemetryMCU = LimitedPower;
	}
	else // if (state == Kill)
	{
		function->TelemetryMCU = NoPower;
	}
	return;
}

/* Set the power mode for science payload */
void Set_SciencePayload(system_function *function, uint8_t state)
{
	// Set SciencePayloadSys power mode
	if (state == Normal || state == ScienceOnly)
	{
		function->SciencePayloadSys = Power;
	}
	else if (state == Eclipse)
	{
		function->SciencePayloadSys = LimitedPower;
	}
	else // if (state == Detumble || state == Kill || state == UltraLowPower || state == LowPower)
	{
		function->SciencePayloadSys = NoPower;
	}
	
	// Set SciencePayloadMCU power mode
	if (state == Detumble || state == Normal || state == Eclipse || state == ScienceOnly)
	{
		function->SciencePayloadMCU = Power;
	}
	else if (OFF)
	{
		function->SciencePayloadMCU = LimitedPower;
	}
	else // if (state == Kill || state == UltraLowPower || state == LowPower)
	{
		function->SciencePayloadMCU = NoPower;
	}
	return;
}

/* Transition between the states in the state machine */
uint8_t Transition(uint8_t event)
{
	// Not sure what types of input lead to a transition
	// Create a function once this is known
	
	// Delay the program for awhile to test they are correct
	for (int i = 0; i < 100000000; i++);
	
	
	if (event == Detumble)
	{
		return Detumble_Transition();
	}
	else if (event == Kill)
	{
		return Kill;
	}
	else if (event == Normal)
	{
		return Normal_Transition();
	}
	else if (event == UltraLowPower)
	{
		return UltraLowPower_Transition();
	}
	else if (event == LowPower)
	{
		return LowPower_Transition();
	}
	else if (event == Eclipse)
	{
		return Eclipse_Transition();
	}
	else if (event == ScienceOnly)
	{
		return ScienceOnly_Transition(); 
	}
	return 255;
}


/* Used for debugging and testing the system. Print out all of te current power modes. */
void Print_System_Settings(system_function *function)
{
	printf("0 is Power, 1 is limited power, 2 is No power...\n");
	printf("MechanicalSys: %u\t MechanicalMCU: %u\n", function->MechanicalSys, function->MechanicalMCU);
	printf("LinearTransponderSys %u\t LinearTransponderMCU %u\n", function->LinearTransponderSys, function->LinearTransponderMCU);
	printf("TelemetrySys %u\t\t TelemetryMCU %u\n", function->TelemetrySys, function->TelemetryMCU);
	printf("SciencePayloadSys %u\t SciencePayloadMCU %u\n", function->SciencePayloadSys, function->SciencePayloadMCU);
	return;
}
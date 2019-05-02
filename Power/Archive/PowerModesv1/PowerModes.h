#ifndef POWERMODES_H
#define POWERMODES_H


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#define OFF (0)
#define ON (1)

/* Define states and system function variables */
enum State {StartUp, Shutdown, Normal, PowerSave, TelemetryOnly, Eclipse, ScienceOnly};

enum PowerLevel {Power, LimitedPower, NoPower};

typedef struct system_function 
{
	uint8_t MechanicalSys;
	uint8_t MechanicalMCU;
	uint8_t LinearTransponderSys;
	uint8_t LinearTransponderMCU;
	uint8_t TelemetrySys;
	uint8_t TelemetryMCU; 
	uint8_t SciencePayloadSys;
	uint8_t SciencePayloadMCU;
} system_function;

typedef struct events
{
	uint8_t event1;
} events;


/* Define function prototypes */

/* Set the intial power modes of each system */
void Initialize_Functions(system_function *functions);

/* Sets the power mode of each of the systems */
void Set_PowerModes(system_function *function, uint8_t state);

/* Set the power mode for mechanical */
void Set_Mechanical(system_function *function, uint8_t state);

/* Set the power mode for linear transponder */
void Set_LinearTransponder(system_function *function, uint8_t state);

/* Set the power mode for telemery */
void Set_Telemetry(system_function *function, uint8_t state);

/* Set the power mode for science payload */
void Set_SciencePayload(system_function *function, uint8_t state);

/* Transition between the states in the state machine */
uint8_t Transition(uint8_t event);

/* Used for debugging and testing the system. Print out all of te current power modes. */
void Print_System_Settings(system_function *function);

#endif
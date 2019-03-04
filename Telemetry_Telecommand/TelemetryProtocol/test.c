#include "protocol.h"


int main(int argc, char **argv)
{
	
	// Create a packet
	
	// Decode packet
	
	// Verify that it was decoded and made properly
	
	// Testing the log science command
	//uint8_t *pack = malloc(3 * sizeof(uint8_t));
	
	uint8_t pack[3];
	
	uint8_t logType = 1;
	time_of_day testTime;
	
	testTime.hour = 20; // 0001 0100
	testTime.min = 35; // 0010 0011
	testTime.sec = 59; // 0011 1011
	
	uint8_t t = Create_Command_LogSciEvent(pack, logType, &testTime);
	if (t)
		printf("Final packet: %u\t%u\t%u\n", pack[0], pack[1], pack[2]);
	
	//free(pack);
	return 0;
}
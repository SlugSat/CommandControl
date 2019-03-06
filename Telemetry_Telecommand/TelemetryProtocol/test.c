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
	{	
		printf("The output should be 15 48 FB----------logType =1 = logat a certain time \n");
		printf("Final packet: %x\t%x\t%x\n", pack[0], pack[1], pack[2]);
	}
	/////////////////////////////////////////////////////////////////////////////
	

	uint8_t packet[3];
	
	uint8_t logType1 =0 ;
	
	time_of_day testTime1;
	testTime1.hour = 20; // 0001 0100
	testTime1.min = 35; // 0010 0011
	testTime1.sec = 59; // 0011 1011
	
	uint8_t u = Create_Command_LogSciEvent(packet, logType1, &testTime1);
	if (u)
	{
		printf("The output should be 10 00 00 --------logType =0 log now \n");
		printf("Final packet: %x\t%x\t%x\n", packet[0], packet[1], packet[2]);
	}	
	////////////////////////////////////////////////////////////////////////////////	
	uint8_t pack2[3];
	
	uint8_t logType2 = 1;
	time_of_day testTime2;
	
	testTime2.hour = 13; // 0000 1101
	testTime2.min = 29; // 0001 1101
	testTime2.sec = 02; // 0000 0010
	
	uint8_t v = Create_Command_LogSciEvent(pack2, logType2, &testTime2);
	if (v)
	{
		printf("The output should be 14 d7 42----------logType =1 = logat a certain time \n");
		printf("Final packet: %x\t%x\t%x\n", pack2[0], pack2[1], pack2[2]);
	}
	/////////////////////////////////////////////////////////////////////////////

	//free(pack);
	return 0;
}

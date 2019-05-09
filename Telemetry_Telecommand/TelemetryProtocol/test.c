#include "protocol.h"

#include <limits.h>



uint64_t double_to_unsigned(double input);
void double_to_bytes(double input, uint8_t *retArr);

int main(int argc, char **argv)
{
	
	printf("*First create tests that will encapsulate a packet and then test the packet contents*\n");
	
//This is for the ground station side
	////////////////////// Testing the log science command///////////////////////////
	uint8_t pack[3];
	uint8_t logType = 1;
	time_of_day testTime;
	testTime.hour = 20; // 0001 0100
	testTime.min = 35; // 0010 0011
	testTime.sec = 59; // 0011 1011
	uint8_t t = Create_Command_LogSciEvent(pack, logType, &testTime);
	if (t)
	{	
		printf("Request: Log Science Event at a specified time\n");
		printf("\tThe packet should be:\t14 48 FB\n");
		
		printf("\tPacket created:\t\t");
		for (int i = 0; i < 3; i++)	printf("%02X ", pack[i]);
		printf("\n");
	}
	
	/////////////////////////////////////////////////////////////////////////////
	
	uint8_t packet[3];
	uint8_t logType1 = 0;
	time_of_day testTime1;
	testTime1.hour = 20; // 0001 0100
	testTime1.min = 35; // 0010 0011
	testTime1.sec = 59; // 0011 1011
	uint8_t u = Create_Command_LogSciEvent(packet, logType1, &testTime1);
	if (u)
	{
		printf("Request: Log Science Event Now\n");
		printf("\tThe packet should be:\t10 00 00\n");
		
		printf("\tPacket created:\t\t");
		for (int i = 0; i < 3; i++)	printf("%02X ", packet[i]);
		printf("\n");
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
		printf("Request: Log Science Event at a specified time\n");
		printf("\tThe packet should be:\t14 D7 42\n");
		
		printf("\tPacket created:\t\t");
		for (int i = 0; i < 3; i++)	printf("%02X ", pack2[i]);
		printf("\n");
	}
	
	//////////////////////////Testing  kill command //////////////////////
	uint8_t packKill;
	uint8_t w = Create_Kill_Packet(&packKill);
	if (w)
	{
		printf("Command: Kill\n");
		printf("\tThe packet should be:\tF0\n");
		printf("\tPacket created:\t\t%02X\n", packKill);
	}
	
	//////////////////////////Testing request status command  //////////////////////
	uint8_t packReqstat;
	uint8_t x = Create_Request_Status(&packReqstat);
	if (x)
	{
		printf("Request: Status of CubeSat\n");
		printf("\tThe packet should be:\t20\n");
		printf("\tPacket created:\t\t%02X\n", packReqstat);
	}
	
	//////////////////////////Testing request science data command  //////////////////////
	/* Chunk Size is 0 so the packet should only use the start and finish time */
	uint8_t packReqSci[7];
	uint32_t chunkSize = 0;
	time_of_day testTimeStart;
	testTimeStart.hour = 13; // 0000 1101//0d
	testTimeStart.min = 29; // 0001 1101//1D
	testTimeStart.sec = 02; // 0000 0010//02
	time_of_day testTimeFinish;
	testTimeFinish.hour = 20; // 0001 0100//14
	testTimeFinish.min = 35; // 0010 0011//23
	testTimeFinish.sec = 59; // 0011 1011//3B
	uint8_t y = Create_Request_ScienceData(packReqSci, &testTimeStart, &testTimeFinish,  chunkSize);
	if (y)
	{	
		printf("Request: Science Data with an end time\n");
		printf("\tThe packet should be:\t40 0D 1D 02 14 23 3B\n");
		
		printf("\tPacket created:\t\t");
		for (int i = 0; i < 7; i++)	printf("%02X ", packReqSci[i]);
		printf("\n");
	}
	
	/* Chunk Size is non zero so the packet should only use the start time and the chunk size */
	uint8_t packReqSci2[7];
	uint32_t chunkSize2 = 16;
	time_of_day testTimeStart2;
	testTimeStart2.hour = 13; // 0000 1101//0d
	testTimeStart2.min = 29; // 0001 1101//1D
	testTimeStart2.sec = 02; // 0000 0010//02
	time_of_day testTimeFinish2;//wont be: needed since chunk size is > 0
	
	uint8_t z = Create_Request_ScienceData(packReqSci2, &testTimeStart2, &testTimeFinish2,  chunkSize2);
	if (z)
	{	
		printf("Request: Science Data with a chunk\n");
		printf("\tThe packet should be:\t44 0D 1D 02 00 00 10\n");
		
		printf("\tPacket created:\t\t");
		for (int i = 0; i < 7; i++)	printf("%02X ", packReqSci2[i]);
		printf("\n");
	}
	
	//////////////////////////Testing request location command  //////////////////////
	uint8_t packReqLoc;
	uint8_t aa = Create_Request_Location(&packReqLoc);
	if (aa)
	{
		printf("Request: CubeSat location\n");
		printf("\tThe packet should be:\t50\n");
		printf("\tPacket created:\t\t%02X\n", packReqLoc);
	}
	
	//////////////////////////Testing update location command  //////////////////////
	uint8_t packUpdateLoc[4];
	uint8_t KepElem1 = 13; // 0000 1101//0d
	uint8_t KepElem2 = 29; // 0001 1101//1D
	uint8_t KepElem3 = 02; // 0000 0010//02
	uint8_t a = Create_Command_UpdateKep( packUpdateLoc,  KepElem1,  KepElem2,  KepElem3);
	if (a)
	{
		printf("Command: Update Keplerian Elements\n");
		printf("\tThe packet should be:\t30 0D 1D 02\n");
		
		printf("\tPacket created:\t\t");
		for (int i = 0; i < 4; i++)	printf("%02X ", packUpdateLoc[i]);
		printf("\n");
	}
	
	/////////////////////////////////////////////////////////////////////////////

	/*This is for the cubesat side*/
	/* creating  a packet responding to the status request from ground station */
	//////////////////////////Testing cube sat status   //////////////////////
	uint8_t packResponseStat[4];
	uint8_t status  = 2; 
	time_of_day satTime;
	satTime.hour = 13; // 0000 1101//0d
	satTime.min = 29; // 0001 1101//1D
	satTime.sec = 02; // 0000 0010//02
	uint8_t b = Create_Response_Status( packResponseStat,  status,  satTime);
	if (b)
	{
		printf("Respond with Satellite Status\n");
		printf("\tThe packet should be:\t69 D0 42 02\n");
		
		printf("\tPacket created:\t\t");
		for (int i = 0; i < 4; i++)	printf("%02X ", packResponseStat[i]);
		printf("\n");
	}
	////////////////////////// testing ScienceData//////////////////////
	uint8_t packSciData[18];//change this
	uint32_t data[5]={996425292, 1555747228,429467295,487478373,555555555 }; 
	uint16_t dataLength =15;
	uint8_t c = Create_ScienceData(packSciData, data,  dataLength,  satTime);
	if (c)
	{
		printf("Respond with Science data\n");
		printf("\tThe packet should be:\t69 D0 44 64 3E 4C BA D1 9C 99 26 9F 0E 54 65 1D 1A E3 \n");
		
		printf("\tPacket created:\t\t");
		for (int i = 0; i < 18; i++)      printf("%02X ",packSciData[i]); 
		printf("\n");
	}
	/////////////////// testing Create Acknowledgement for cubesat///////////////
	uint8_t packAckCube[4];
	uint8_t hashValue  = 2; 
	time_of_day satTime3;
	satTime3.hour = 13; // 0000 1101 // 0d
	satTime3.min = 29; // 0001 1101 // 1D
	satTime3.sec = 02; // 0000 0010 // 02
	uint8_t d = Create_Acknowledgement( packAckCube,  hashValue,  satTime3);
	if (d)
	{
		printf("Satellite Acknowledgement\n");
		printf("\tThe packet should be:\t69 D0 47 02\n");
		
		printf("\tPacket created:\t\t");
		for (int i = 0; i < 4; i++)	printf("%02X ", packAckCube[i]);
		printf("\n");
	}
	
	/////////////////// testing Create location data for cubesat///////////////
	uint8_t packLocCube[6];
	uint8_t KepElem11 = 13; // 0000 1101 // 0d
	uint8_t KepElem22 = 29; // 0001 1101 // 1D
	uint8_t KepElem33 = 02; // 0000 0010 // 02 
	time_of_day satTime4;
	satTime4.hour = 20; 	// 0001 0100 // 14
	satTime4.min = 35; 		// 0010 0011 // 23
	satTime4.sec = 59; 		// 0011 1011 // 3B
	uint8_t e = Create_LocationData( packLocCube,  KepElem11,  KepElem22,  KepElem33,  satTime4);
	if (e)
	{
		printf("Respond with Location Data\n");
		printf("\tThe packet should be:\tA2 37 65 0D 1D 02\n");
		
		printf("\tPacket created:\t\t");
		for (int i = 0; i < 6; i++)	printf("%02X ", packLocCube[i]);
		printf("\n");
	}
	/////////////////////////////////////////////////////////////////////////////

	// Test decoding certain types of packets
	printf("\n*Now Testing Decoding Packets from the CubeSat:*\n");
	Decode_Ground_Packet(packResponseStat);
	Decode_Ground_Packet(packSciData);
	Decode_Ground_Packet(packAckCube);
	Decode_Ground_Packet(packLocCube);

	printf("\n*Now Testing Decoding Packets from the Ground Station:*\n");
	
	Decode_Sat_Packet(pack);
	Decode_Sat_Packet(packet);
	Decode_Sat_Packet(pack2);
	Decode_Sat_Packet(&packKill);
	Decode_Sat_Packet(&packReqstat);
	Decode_Sat_Packet(packReqSci2);
	Decode_Sat_Packet(packReqSci);
	Decode_Sat_Packet(&packReqLoc);
	Decode_Sat_Packet(packUpdateLoc);


	printf("\n\nTesting going from a double to a byte and then recreating it\n");
	
	double testDouble = 2458610.512340;
	double outDouble = 0.0;
	
	uint8_t p[8];
	
	// Writes n floats to a packet of size n*4 bytes
	unsigned int p_i = 0; // Packet index
	uint64_t intermed = (uint64_t) testDouble; 
	for(unsigned int b_i = 0;b_i < 8;b_i++) 
	{ 	// Loop over bytes in float
		//p[p_i++] = (uint8_t) (intermed & (0xff << 8*b_i)) >> (8*b_i);
		p[p_i] = p[p_i] | ((intermed & (0xFF << 8*b_i)) >> (8*b_i));
		p_i++;
	}

	uint64_t temp = 0;
	// Reads n floats from a packet of size n*4 bytes
	for(unsigned int b_i = 0 ; b_i < 8 ; b_i++) 
	{ 	// Loop over bytes in float
		temp = (uint64_t) temp | (p[b_i] << b_i);
	}
	outDouble = (double) temp;
	
	printf("Started (unsigned): %u\t(double): %f\n", (uint64_t) testDouble, testDouble);
	printf("Double start: 0x%016x\n", testDouble);
	printf("Uint64 start: 0x%016x\tEnd: 0x%016x\n", intermed, temp); 

	printf("Started: %f\tOutput: %f\n", testDouble, outDouble);

	printf("\n\n\n");
	unsigned idx;
/* 	unsigned char *ptr = (unsigned char*) &testDouble;

	for (idx=CHAR_BIT * sizeof(testDouble); idx-- ; ) {
        	putc(
        	( ptr[idx/CHAR_BIT] & (1u << (idx%CHAR_BIT) ) ) ? '1' : '0', stdout);
        } */
	printf("\n\n");
	printf("0100000101000010110000011111100101000001100101000101101101101100\n\n");	
		
	uint8_t testP[8] = {0};
	unsigned char *ptr1 = (unsigned char*) &testDouble;
	for (idx = CHAR_BIT * sizeof(testDouble); idx--; ) 
	{
		if ((ptr1[idx/CHAR_BIT] & (1u << (idx%CHAR_BIT))) == 0)
		{
			
			//printf("%u: %u: 0\n", idx/CHAR_BIT, idx);
			printf("0");
		}
		else
		{
			testP[idx/CHAR_BIT] = testP[idx/CHAR_BIT] | (1u << idx%CHAR_BIT);
			//printf("%u: %u: 1\n", idx/CHAR_BIT, idx);
			printf("1");
		}
    } 
	printf("\n\n");
	uint64_t hi = 0x4142C1F941945B6C;
	printf("%02lx\n\n", hi);
	for (int i = 7; i >= 0; i--)
	{
		printf("%02lx", testP[i]);
	}
	printf("\n\n");
	// https://gregstoll.com/~gregstoll/floattohex/
	// 4142C1F941945B6C
	
	temp = 0;
	temp |= (uint64_t) testP[7];
	for(int b_i = 6 ; b_i >= 0 ; b_i--) 
	{ 	// Loop over bytes in float
		temp <<= 8;
		temp |= (uint64_t) testP[b_i];
		printf("%d: %016lx\n", b_i, temp);
	}
	outDouble = (double) temp;
	uint64_t temp1 = 0x2583f2;
	printf("Started: %lf\tOutput: %lf\n\n\n", (double) temp1, (double) outDouble);

	union Date {
		double date;
		uint64_t udate;
		uint8_t idate[8];
	};

	union Date D;
	D.date = 2458610.512340;
	//D.udate = 0x4142c1f941945b6c;

	printf( "Double date : %lf\n", D.date);
	   
	printf( "Unsigned date : %016lx\n", D.udate);
	for (int k = 7 ; k >= 0 ; k--)
	{
		printf("Byte: %d  Value: 0x%02x\n", k, D.idate[k]);
	}		

       uint64_t output9 = double_to_unsigned(testDouble);
       printf("\n\nOutput: 0x%016lx\n\n", output9);
	

	for(int k = 7; k >= 0; k--)
	{	
		printf("Byte: %d  Value: 0x%02x\n", k, D.idate[k]);
	} 

}

uint64_t double_to_unsigned(double input)
{
        union Date {
                double date;
                uint64_t udate;
        };
        union Date D;
        D.date = input;
	printf("udate: 0x%016lx\n", D.udate);
        return D.udate;
}

void double_to_bytes(double input, uint8_t *retArr)
{	
	union Date {
		double date;
		uint64_t udate;
		uint8_t bdate[8];
	};
	union Date D;
	D.date = input;
	memcpy(retArr, D.bdate, 8);
}

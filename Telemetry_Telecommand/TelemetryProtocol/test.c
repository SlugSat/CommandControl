#include "protocol.h"


int main(int argc, char **argv)
{
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
	//////////////////////////Testing  kill command //////////////////////
	uint8_t packKill;
	uint8_t w = Create_Kill_Packet(&packKill);
	if (w)
	{
		printf("The output should be f0 for kill command \n");
		printf("Final packet: %x\t\n", packKill);
	}
	//////////////////////////Testing request status command  //////////////////////
	uint8_t packReqstat;
	uint8_t x = Create_Request_Status(&packReqstat);
	if (x)
	{
		printf("The output should be f0 for request status command \n");
		printf("Final packet: %x\t\n", packReqstat);
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
		printf("The output should be 40 0d 1D 02 14 23 3B\n");
		printf("Final packet: %x\t%x\t%x\t%x%x\t%x\t%x\n", pack[0], pack[1], pack[2],pack[3], pack[4], pack[5], pack[6]);
	}
	/* Chunk Size is non zero so the packet should only use the start time and the chunk size */
	uint8_t packReqSci2[7];
	uint32_t chunkSize2 = 16;
	time_of_day testTimeStart2;
	testTimeStart2.hour = 13; // 0000 1101//0d
	testTimeStart2.min = 29; // 0001 1101//1D
	testTimeStart2.sec = 02; // 0000 0010//02
	time_of_day testTimeFinish2;//wont be needed since chunk size is > 0
	
	uint8_t z = Create_Request_ScienceData(packReqSci2, &testTimeStart2, &testTimeFinish2,  chunkSize2);
	if (z)
	{	
		printf("The output should be 44 0d 1D 02 00 00 01\n");
		printf("Final packet: %x\t%x\t%x\t%x\t%x\t%x\t%x\n", pack[0], pack[1], pack[2],pack[3], pack[4], pack[5], pack[6]);
	}
	//////////////////////////Testing request location command  //////////////////////
	uint8_t packReqLoc;
	uint8_t aa = Create_Request_Location(&packReqLoc);
	if (aa)
	{
		printf("The output should be 50 for request location command \n");
		printf("Final packet: %x\t\n", packReqLoc);
	}
	//////////////////////////Testing update location command  //////////////////////
	uint8_t packUpdateLoc[4];
	uint8_t KepElem1 = 13; // 0000 1101//0d
	uint8_t KepElem2 = 29; // 0001 1101//1D
	uint8_t KepElem3 = 02; // 0000 0010//02
	uint8_t a = Create_Command_UpdateKep( packUpdateLoc,  KepElem1,  KepElem2,  KepElem3);
	if (a)
	{
		printf("The output should be 30 0d 1D 02 for updating location command \n");
		printf("Final packet: %x\t%x\t%x\t%x\n", packUpdateLoc[0], packUpdateLoc[1], packUpdateLoc[2], packUpdateLoc[3]);
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
		printf("The output should be 69 D0 42 02 for Satellite  Status\n");
		printf("Final packet: %x\t%x\t%x\t%x\n", packResponseStat[0], packResponseStat[1], packResponseStat[2], packResponseStat[3]);
	}
	////////////////////////// testing ScienceData//////////////////////
	
	/////////////////// testing Create Acknowledgement for cubesat///////////////
	uint8_t packAckCube[4];
	uint8_t hashValue  = 2; 
	time_of_day satTime3;
	satTime3.hour = 13; // 0000 1101//0d
	satTime3.min = 29; // 0001 1101//1D
	satTime3.sec = 02; // 0000 0010//02
	uint8_t d = Create_Acknowledgement( packAckCube,  hashValue,  satTime3);
	if (d)
	{
		printf("The output should be 69 D0 47 02 for Satellite  Status\n");
		printf("Final packet: %x\t%x\t%x\t%x\n", packAckCube[0], packAckCube[1], packAckCube[2], packAckCube[3]);
	}
	/////////////////// testing Create location data for cubesat///////////////
	uint8_t packLocCube[6];
	uint8_t KepElem11 = 13; // 0000 1101//0d
	uint8_t KepElem22 = 29; // 0001 1101//1D
	uint8_t KepElem33 = 02; // 0000 0010//02 
	time_of_day satTime4;
	satTime4.hour = 20; // 0001 0100//14
	satTime4.min = 35; // 0010 0011//23
	satTime4.sec = 59; // 0011 1011//3B
	uint8_t e = Create_LocationData( packLocCube,  KepElem11,  KepElem22,  KepElem33,  satTime4);
	if (e)
	{
		printf("The output should be A2 37 65 0D 1D 02 for Satellite  Status\n");
		printf("Final packet: %x\t%x\t%x\t%x\t%x\t%x\n", packLocCube[0], packLocCube[1], packLocCube[2], packLocCube[3], packLocCube[4], packLocCube[5]);
	}
	/////////////////////////////////////////////////////////////////////////////


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//free(pack);
	return 0;
}

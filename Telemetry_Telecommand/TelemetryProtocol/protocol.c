#include "protocol.h"


/***** Functions for the ground station side *****/

/* This function will decode packets that have come into the ground station */
uint8_t Decode_Ground_Packet(uint8_t *packet)
{
	return SUCCESS;
}

/* Create the packet used for sending a Kill signal */
uint8_t Create_Kill_Packet(uint8_t *retPacket)
{
	return SUCCESS;
}

/* Create a log science event packet */
uint8_t Create_Command_LogSciEvent(uint8_t *retPacket, uint8_t logType, time_of_day *StartTime)
{
	return SUCCESS;
}

/* Create a request for the status of the CubeSat */ 
uint8_t Create_Request_Status(uint8_t *retPacket)
{
	return SUCCESS;
}

/* Create a request for science data */
uint8_t Create_Request_ScienceData(uint8_t *retPacket, time_of_day *StartTime, time_of_day *FinishTime, uint32_t chunkSize)
{
	return SUCCESS;
}

/* Create a request for the CubeSat's location (Keplerian Elements) */
uint8_t Create_Request_Location(uint8_t *retPacket)
{
	return SUCCESS;
}

/* Create a packet to update the Keplerian elements on the CubeSat */
uint8_t Create_Command_UpdateKep(uint8_t *retPacket, uint8_t KepElem1, uint8_t KepElem2, uint8_t KepElem3)
{
	return SUCCESS;
}


/***** Functions for the CubeSat side *****/

/* This function will decode packets that have come into the ground station */
uint8_t Decode_Sat_Packet(uint8_t *packet)
{
	return SUCCESS;
}

/* Create a packet responding to the stats request from ground station */
uint8_t Create_Response_Status(uint8_t *retPacket, uint8_t status)
{
	return SUCCESS;
}

/* Create a packet containing science payload data */
uint8_t Create_ScienceData(uint8_t *retPacket, sci_data *data, uint16_t dataLength)
{
	return SUCCESS;
}

/* Acknowledgement of certain messages or responses */
uint8_t Create_Acknowledgement(uint8_t *retPacket, uint8_t hashValue)
{
	return SUCCESS;
}

/* Create a packet with the current CubeSat's location (Keplerian Elements) */
uint8_t Create_LocationData(uint8_t *retPacket, uint8_t KepElem1, uint8_t KepElem2, uint8_t KepElem3)
{
	return SUCCESS;
}
#ifndef PROTO_H
#define PROTO_H

// Standard include files
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>
#include <math.h>
#include <time.h>

#include "DateConversion.h"
#include "CC1200_SPI_functions.h"
#include "SPI_FRAM.h"
#include "main.h"
#include "FRAM_Lib.h"
#include "FRAM_Tests.h"

/********** Type defined structs and enums **********/

// This struct will hold each of the times needed 
typedef struct time_of_day
{
	uint16_t millisec;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t month;
	uint8_t day;
	uint8_t year; // year since 1900
} time_of_day;

// This is a science data point that was logged by the science payload system
typedef struct ScienceDataPoint
{
	uint32_t Time;
	uint8_t Energy;
} ScienceDataPoint;


/********** Defined macros **********/
#define MAX_PACK_SIZE 	(1024)
#define FIXED_PACK_SIZE (24)
#define SUCCESS 	  		(1)
#define FAIL 		  			(0)

#define SAT_STATUS 		(0x2)
#define SCI_DATA   		(0x4)
#define SAT_LOCATION 	(0x5)
#define SAT_ACK 			(0x7)

#define REQ_LOG 			(0x10)
#define REQ_LOG_TIME	(0x14)
#define REQ_STATUS 		(0x20)
#define UPDATE_KEP 		(0x30)
#define REQ_SCI_DATA	(0x40)
#define REQ_SCI_DATA2	(0x44)
#define REQ_LOCATION 	(0x50)
#define KILL					(0xF0)


/********** Function Declarations **********/

/*** General functions used to send and receive packets on the satellite or the ground station ***/
void CC1200_Transmit_Packet(uint8_t *packet, uint16_t packetLength, SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart);

/*** Functions for the ground station side ***/
/* This function will decode the time of the CubeSat by looking at the packet received */
double Decode_CubeSat_Time (uint8_t *packet);

/* This function will decode packets that have come into the ground station */
uint8_t Decode_Ground_Packet(uint8_t *packet, uint8_t hashValue);

/* Create the packet used for sending a Kill signal */
uint8_t Create_Kill_Packet(uint8_t *retPacket);

/* Create a log science event packet */
uint8_t Create_Command_LogSciEvent(uint8_t *retPacket, uint8_t logType, double StartTime);

/* Create a request for the status of the CubeSat */ 
uint8_t Create_Request_Status(uint8_t *retPacket);

/* Create a request for science data */
uint8_t Create_Request_ScienceData(uint8_t *retPacket, time_of_day *StartTime, time_of_day *FinishTime, uint32_t chunkSize);

/* Create a request for the CubeSat's location (Keplerian Elements) */
uint8_t Create_Request_Location(uint8_t *retPacket);

/* Create a packet to update the Keplerian elements on the CubeSat */
uint8_t Create_Command_UpdateKep(uint8_t *retPacket, uint8_t KepElem1, uint8_t KepElem2, uint8_t KepElem3);


/*** Functions for the CubeSat side ***/

/* This function will decode packets that have come into the CubeSat */
uint8_t Decode_Sat_Packet(uint8_t *packet, SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, SPI_HandleTypeDef *fram_hspi, I2C_HandleTypeDef *hi2c);

/* Create a packet responding to the stats request from ground station */
uint8_t Create_Response_Status(uint8_t *retPacket, uint8_t status, double julianDate);

/* Create a packet containing science payload data */
uint8_t Create_ScienceData(uint8_t *retPacket, ScienceDataPoint *data, uint16_t dataLength, double julianDate);

/* Acknowledgement of certain messages or responses */
uint8_t Create_Acknowledgement(uint8_t *retPacket, uint8_t hashValue, double julianDate);

/* Create a packet with the current CubeSat's location (Keplerian Elements) */
uint8_t Create_LocationData(uint8_t *retPacket, float latitude, float longitude, float altitude, double julianDate);



/*** Functions for printing out data sent by the CubeSat ***/
void Handle_Sat_Status(uint8_t *packet);
void Handle_Sat_Sci(uint8_t *packet);
uint8_t Handle_Sat_Ack(uint8_t *packet, uint8_t hashValue);
void Handle_Sat_Location(uint8_t *packet);


/*** Functions for handling packets sent by the ground station to the CubeSat***/
void Handle_Kill_Packet(uint8_t *packet, SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart);
void Handle_LogSci_Packet(uint8_t *packet, SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, SPI_HandleTypeDef *fram_hspi);
void Handle_ReqStatus_Packet(uint8_t *packet, SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, SPI_HandleTypeDef *fram_hspi);
void Handle_ReqSciData_Packet(uint8_t *packet, SPI_HandleTypeDef *hspi, I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart);
void Handle_ReqLoc_Packet(uint8_t *packet, SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, SPI_HandleTypeDef *fram_hspi);
void Handle_UpdateKep_Packet(uint8_t *packet, SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, SPI_HandleTypeDef *fram_hspi);

#endif // PROTO_H

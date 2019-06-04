#include "Telemetry_Packet_Protocol.h"

#define DEBUG (0)

/***** General functions used to send and receive packets on the satellite or the ground station *****/
/**
  * @brief Transmit a packet
  * @param The packet to be transmitted
	* @param The length of the packet
	* @param The SPI connected to the CC1200
	* @param The UART connected to the terminal for debugging purposes
  * @retval None
  */
void CC1200_Transmit_Packet(uint8_t *packet, uint16_t packetLength, SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart)
{
	char msg1[100] = {0};
	uint8_t mode = 0;
	
	for (int i = 0; i < packetLength; i++)
	{
		ReadWriteExtendedReg(hspi, CC1200_WRITE_BIT, CC1200_TXFIFO, packet[i]);
	}

	HAL_Delay(6000);
	
	// Go into transmit mode
	do
	{
		ReadWriteCommandReg(hspi, CC1200_STX);
		mode = ReadWriteCommandReg(hspi, CC1200_SNOP);
		HAL_Delay(20);
	} while ((mode & 0x20) != 0x20);
	
	// Go back to transmit mode as long as there are bytes in the buffer
	uint8_t txValue = ReadWriteExtendedReg (hspi, CC1200_READ_BIT, CC1200_NUM_TXBYTES, 0);
	while(txValue != 0)
	{
		mode = ReadWriteCommandReg(hspi, CC1200_SNOP);
		HAL_Delay(20);
		if ((mode & 0x20) != 0x20)
		{
			ReadWriteCommandReg(hspi, CC1200_STX);
		}
		txValue = ReadWriteExtendedReg (hspi, CC1200_READ_BIT, CC1200_NUM_TXBYTES, 0);
	}
	snprintf(msg1, 100, "\nFinished Transmission               \n");
	HAL_UART_Transmit(huart, (uint8_t *) msg1, sizeof(msg1), 1);
}

/***** Functions for the ground station side *****/

/**
  * @brief Decode the time of the CubeSat by looking at the packet received
  * @param The packet from the satellite
  * @retval The julian date expression of time on the satellite
  */
double Decode_CubeSat_Time (uint8_t *packet)
{
	// Packet header structure: First 8 bytes are time data
	double julianDate = bytes_to_double(&packet[0]);
	
	if (DEBUG) printf("Value: %lf\n",  julianDate);
	
	return julianDate;	
}

/**
  * @brief This function will decode packets that have come into the ground station
  * @param The packet from the satellite
	* @param A hash value that could be used for acknowledgements (A hash of each byte of the packet for error checking)
  * @retval The type of packet that it is given by its 1 byte opcode value
  */
uint8_t Decode_Ground_Packet(uint8_t *packet, uint8_t hashValue)
{
	double julianDate = 0.0;
	
	// Decode the packet given to the ground station from the CubeSat
	uint8_t command = (packet[8] & 0x07);
	
	// First decode the opcode
	switch (command)
	{
		case (SAT_STATUS):
			printf("Received a packet containing the status of the CubeSat\n");
			// Handle the request here
			julianDate = Decode_CubeSat_Time(packet);
			//Handle_Sat_Status(packet);
			break;
		case (SCI_DATA):
			printf("Received a packet of science data\n");
			// Handle the request here
			julianDate = Decode_CubeSat_Time(packet);
			//Handle_Sat_Sci(packet);
			break;
		case (SAT_LOCATION):
			printf("Received a packet with the CubeSat's location\n");
			// Handle the request here
			julianDate = Decode_CubeSat_Time(packet);
			//Handle_Sat_Location(packet);
			break;
		case (SAT_ACK):
			printf("Received an Acknowledgement\n");
			// Handle the request here
			julianDate = Decode_CubeSat_Time(packet);
			if (/*Handle_Sat_Ack(packet, hashValue) == 1*/ 0)
			{
				return command | 0xF0;
			}
			break;
		default:
			fprintf(stderr, "Invalid packet command received\n");
			return FAIL; // Packet cold not be decoded	
	}
	
	return command;
}


/**
  * @brief Create the packet used for sending a Kill signal
  * @param The packet created
  * @retval A success or fail
  */
uint8_t Create_Kill_Packet(uint8_t *retPacket)
{
	// Opcode: 1111
	
	uint8_t packet = KILL;
	
	retPacket[0] = packet;
	
	return SUCCESS;
}


/**
  * @brief Create a log science event packet 
  * @param The packet created
	* @param The type of log request to be made: log now or log at StartTime
	* @param The time to start logging data
  * @retval A success or fail
  */
uint8_t Create_Command_LogSciEvent(uint8_t *retPacket, uint8_t logType, double StartTime)
{	
	uint8_t packet[9] = {0};
	
	// 1st byte is 0001 0X00 where X is the value of logtype, 1 or 0
	
	if (logType == 0) // log now
	{
		packet[0] = REQ_LOG;
	}
	else if (logType == 1) // log at a specific later date
	{
		packet[0] = REQ_LOG_TIME;
		
		// Packet          
		// Packet structure: 0001 0100 TIME (8 byte julian date)
		double_to_bytes(StartTime, &packet[1]);
	}
	else
	{
		retPacket = NULL;
		return FAIL; // Packet could not be created
	}

	// Return the packet to be used outside this function
	memcpy(retPacket, packet, 8);
	
	return SUCCESS;
}

/* Create a request for the status of the CubeSat */ 
uint8_t Create_Request_Status(uint8_t *retPacket)
{
	// 1st byte is 0010 0000, where the bottom 4 bits could be used for optional messages in the future
	uint8_t packet = REQ_STATUS;
	
	// Return the packet to be used outside this function
	retPacket[0] = packet;
	
	return SUCCESS;
}

/**
  * @brief Create a request to downlink science data
  * @param The packet created
	* @param The time stamp used to start looking for data to be downlinked
	* @param The time stamp used to mark the end of the data to be downlinked
	* @param If a chunk of data is requested instead, then a chunkSize number of data points will be downlinked
  * @retval A success or fail
  */
uint8_t Create_Request_ScienceData(uint8_t *retPacket, time_of_day *StartTime, time_of_day *FinishTime, uint32_t chunkSize)
{
	uint8_t packet[7] = {0};

	// 1st byte is 0100 0X00, where X is whether there is a chunk or end time
	if ((chunkSize != 0) && (chunkSize < 0xFFFFFF))
	{
		packet[0] = REQ_SCI_DATA2;
		packet[1] = StartTime->hour;
		packet[2] = StartTime->min;
		packet[3] = StartTime->sec;
		packet[4] = (chunkSize & 0x00FF0000);
		packet[5] = (chunkSize & 0x0000FF00);
		packet[6] = (chunkSize & 0x000000FF);
	} 
	else if (chunkSize == 0) // chunk is not specified
	{
		packet[0] = REQ_SCI_DATA;
		packet[1] = StartTime->hour;
		packet[2] = StartTime->min;
		packet[3] = StartTime->sec;
		packet[4] = FinishTime->hour;
		packet[5] = FinishTime->min;
		packet[6] = FinishTime->sec;
	}
	
	// Return the packet to be used outside this function
	memcpy(retPacket, packet, 7);
	
	return SUCCESS;
}


/**
  * @brief Create a request for the CubeSat's location (Keplerian Elements)
  * @param The packet created
  * @retval A success or fail
  */
uint8_t Create_Request_Location(uint8_t *retPacket)
{
	// 1st byte is 0101 0000, where the bottom 4 bits could be used for optional messages in the future
	uint8_t packet = REQ_LOCATION;
	
	// Return the packet to be used outside this function
	retPacket[0] = packet;
	return SUCCESS;
}


/**
  * @brief Create a packet to update the Keplerian elements on the CubeSat (Update the Sat's location)
  * @param The packet created
	* @param Various Keplerian elements. Size and amount may need to be changed in the future
  * @retval A success or fail
  */
uint8_t Create_Command_UpdateKep(uint8_t *retPacket, uint8_t KepElem1, uint8_t KepElem2, uint8_t KepElem3)
{
	// 1st byte is 0011 0000
	// The other 3 bytes are each Keplerian elements
	uint8_t packet[4] = {UPDATE_KEP,
						 KepElem1,
						 KepElem2,
						 KepElem3};
	
	// Return the packet to be used outside this function
	memcpy(retPacket, packet, 4);	
	
	return SUCCESS;
}


/***** Functions for the CubeSat side *****/


/**
  * @brief This function will decode packets that have come into the CubeSat
  * @param The packet from the ground station
	* @param SPI connected to the CC1200
	* @param UART used for debugging
	* @param SPI connected to the shared SPI FRAM
	* @param I2C connected to the science payload's I2C FRAM
  * @retval The type of packet received
  */
uint8_t Decode_Sat_Packet(uint8_t *packet, SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, SPI_HandleTypeDef *fram_hspi, I2C_HandleTypeDef *hi2c)
{
	char msg1[100] = {0};
	
	// Decode the packet given to the CubeSat from the ground station
	uint16_t command = (packet[0] & 0xFF);
	
	// First decode the opcode
	switch (command)
	{
		case (REQ_LOG):
			snprintf(msg1, 100, "\nGot a request to log a science event now\n");
			HAL_UART_Transmit(huart, (uint8_t *) msg1, sizeof(msg1), 1);
			// Handle the request here
			Handle_LogSci_Packet(packet, hspi, huart, fram_hspi);
			break;
		case (REQ_LOG_TIME):
			snprintf(msg1, 100, "\nGot a request to log a science event\n");
			HAL_UART_Transmit(huart, (uint8_t *) msg1, sizeof(msg1), 1);
			// Handle the request here
			Handle_LogSci_Packet(packet, hspi, huart, fram_hspi);
			break;
		case (REQ_STATUS):
			snprintf(msg1, 100, "\nGot a status packet\n");
			HAL_UART_Transmit(huart, (uint8_t *) msg1, sizeof(msg1), 1);
			// Handle the request here
			Handle_ReqStatus_Packet(packet, hspi, huart, fram_hspi);
			break;
		case (UPDATE_KEP):
			// Handle the request here
			Handle_UpdateKep_Packet(packet, hspi, huart, fram_hspi);
			break;
		case (REQ_SCI_DATA):
			snprintf(msg1, 100, "\nGot a request for science data by time\n");
			HAL_UART_Transmit(huart, (uint8_t *) msg1, sizeof(msg1), 1);
			// Handle the request here
			Handle_ReqSciData_Packet(packet, hspi, hi2c, huart);
			break;
		case (REQ_SCI_DATA2):
			snprintf(msg1, 100, "\nGot a request for science data by chunk\n");
			HAL_UART_Transmit(huart, (uint8_t *) msg1, sizeof(msg1), 1);
			// Handle the request here
			Handle_ReqSciData_Packet(packet, hspi, hi2c, huart);
			break;
		case (REQ_LOCATION):
			snprintf(msg1, 100, "\nGot a req location packet\n\n");
			HAL_UART_Transmit(huart, (uint8_t *) msg1, sizeof(msg1), 1);
			// Handle the request here
			Handle_ReqLoc_Packet(packet, hspi, huart, fram_hspi);
			break;
		case (KILL):
			snprintf(msg1, 100, "\nGot a kill packet\n");
			HAL_UART_Transmit(huart, (uint8_t *) msg1, sizeof(msg1), 1);
			// Handle the request here
			Handle_Kill_Packet(packet, hspi, huart);
			break;
		default:
			fprintf(stderr, "Invalid packet command received\n");
			return FAIL; // Packet cold not be decoded	
	}
	ReadWriteCommandReg(hspi, CC1200_SFTX);
	return command;
}


/**
  * @brief Create a packet responding to the status request from ground station
  * @param The packet created
	* @param Status of the satellite
	* @param Current time of the CubeSat in julian date form
  * @retval A success or fail
  */
uint8_t Create_Response_Status(uint8_t *retPacket, uint8_t status, double julianDate)
{
	// Packet header: TIME(8 bytes) 0000 0XXX
	
	//Initialize the packet
	uint8_t packet[10] = {0};

	// Convert the date into bytes
	double_to_bytes(julianDate, &packet[0]);
	
	// The command code filling in the XXX is 010
	packet[8] = 0x02;

	// Store the status 
	packet[9] = status;
	
	// Return the packet to be used outside this function
	memcpy(retPacket, packet, 10);
	
	return SUCCESS;
}


/**
  * @brief Create a packet containing science payload data
  * @param The packet created
	* @param Science data points
	* @param Number of science data points
	* @param Current time of the CubeSat in julian date form
  * @retval A success or fail
  */
uint8_t Create_ScienceData(uint8_t *retPacket, ScienceDataPoint *data, uint16_t dataLength, double julianDate)
{
	// Packet header: TIME(8 bytes) 0000 0XXX
	
	// Initialize the packet
	uint8_t packet[24] = {0};
	
	// Convert the date into bytes
	double_to_bytes(julianDate, &packet[0]);
	
	// The command code filling in the XXX is 100
	packet[8] = 0x04;
	
	// Store the science payload data
	for (uint8_t i = 0; i < dataLength; i++)
	{
		memcpy(&packet[9 + 5*i], &data[i], 5);
	}
	// Return the packet to be used outside this function
	memcpy(retPacket, packet, 9 + 5*dataLength);
	
	return SUCCESS;
}


/**
  * @brief Create an acknowledgement to certain messages or responses
  * @param The packet created
	* @param Hash value of the message acknowledging
	* @param Current time of the CubeSat in julian date form
  * @retval A success or fail
  */
uint8_t Create_Acknowledgement(uint8_t *retPacket, uint8_t hashValue, double julianDate)
{
	// Packet header: TIME(8 bytes) 0000 0XXX
	
	// Initialize the packet
	uint8_t packet[10] = {0};
	
	// Convert the date into bytes
	double_to_bytes(julianDate, &packet[0]);
	
	// The command code filling in the XXX is 111
	packet[8] = 0x07;
	
	// Store the hashValue
	packet[9] = hashValue;	
	
	// Return the packet to be used outside this function
	memcpy(retPacket, packet, 10);
	
	return SUCCESS;
}

/* Create a packet with the current CubeSat's location (Keplerian Elements) */
/**
  * @brief Create a log science event packet 
  * @param The packet created
	* @param Location of the satellite currently in latitude, longitude, and altitude
	* @param Current time of the CubeSat in julian date form
  * @retval A success or fail
  */
uint8_t Create_LocationData(uint8_t *retPacket, float latitude, float longitude, float altitude, double julianDate)
{
	// Packet header: TIME(8 bytes) 0000 0XXX

	// Initialize the packet
	uint8_t packet[21] = {0};
	
	// Convert the date into bytes
	double_to_bytes(julianDate, &packet[0]);
	
	// The command code filling in the XXX is 101
	packet[8] = 0x05;
	
	// Store the location data
	float_to_bytes(latitude, &packet[9]);
	float_to_bytes(longitude, &packet[13]);
	float_to_bytes(altitude, &packet[17]);
	
	// Return the packet to be used outside this function
	memcpy(retPacket, packet, 21);
	
	return SUCCESS;
}



/*** Functions for handling packets sent by the ground station to the CubeSat***/

/**
  * @brief This function will handle kill packets sent in by the ground station
  * @param The packet from the ground station
	* @param SPI connected to the CC1200
	* @param UART used for debugging
  * @retval None
  */
void Handle_Kill_Packet(uint8_t *packet, SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart)
{
	// Create an Ack
	uint8_t ackPacket[FIXED_PACK_SIZE] = {0};
	// Send an Ack
	Create_Acknowledgement(ackPacket, 0, 2458615.42743); 
	
	// Transmit the packet back to the ground station
	CC1200_Transmit_Packet(ackPacket, FIXED_PACK_SIZE, hspi, huart);
	
	// Continually send an interrupt to the power modes to kill the system
	for (uint32_t i = 0; i < 5; i++)
	{
		HAL_GPIO_TogglePin(GPIOA, Kill_to_PModes_Int_Pin);
		HAL_Delay(10);
		HAL_GPIO_TogglePin(GPIOA, Kill_to_PModes_Int_Pin);
		HAL_Delay(1000);
	}
}

/**
  * @brief This function will handle log data request packets sent in by the ground station
  * @param The packet from the ground station
	* @param SPI connected to the CC1200
	* @param UART used for debugging
	* @param SPI connected to the shared SPI FRAM
  * @retval None
  */
void Handle_LogSci_Packet(uint8_t *packet, SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, SPI_HandleTypeDef *fram_hspi)
{
	// Write to SPI FRAM in the location for a logged science event
	
	if ((packet[0] & 0x04) == 0x04)// Only log science events that happen in the future
	{
		double julianDate = bytes_to_double(&packet[1]);
		
		SPI_FRAM_Write(fram_hspi, SPI_FRAM_LOG_DATA_TIME_ADDR, &packet[1], 8, huart, 0);
		
		char msg1[100] = {0};
		uint8_t ackPacket[FIXED_PACK_SIZE] = {0};
		
		snprintf(msg1, 100, "\nTime to log: %lf\n", bytes_to_double(&packet[1]));
		HAL_UART_Transmit(huart, (uint8_t *) msg1, sizeof(msg1), 1);
		
		// Send an Ack
		Create_Acknowledgement(ackPacket, 0, 2458615.42743); // The double value should be the current time
		
		// Transmit the packet back to the ground station
		CC1200_Transmit_Packet(ackPacket, FIXED_PACK_SIZE, hspi, huart);
	}
}

/**
  * @brief This function will handle status request packets sent in by the ground station
  * @param The packet from the ground station
	* @param SPI connected to the CC1200
	* @param UART used for debugging
	* @param SPI connected to the shared SPI FRAM
  * @retval None
  */
void Handle_ReqStatus_Packet(uint8_t *packet, SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, SPI_HandleTypeDef *fram_hspi)
{
	// The status that will be used for the flat sat is the battery level
	uint8_t battLevel;
	SPI_FRAM_Read(fram_hspi, SPI_FRAM_BATT_LEVEL_ADDR, &battLevel, 1, huart, 0);
	
	//battLevel = 50;
	
	uint8_t statusPacket[FIXED_PACK_SIZE] = {0};
	Create_Response_Status(statusPacket, battLevel, 2458615.42743);
	
	// Transmit the packet back to the ground station
	CC1200_Transmit_Packet(statusPacket, FIXED_PACK_SIZE, hspi, huart);
}

/**
  * @brief This function will handle downlink science data packets sent in by the ground station
  * @param The packet from the ground station
	* @param SPI connected to the CC1200
	* @param I2C connected to the science payload's I2C FRAM
	* @param UART used for debugging
  * @retval None
  */
void Handle_ReqSciData_Packet(uint8_t *packet, SPI_HandleTypeDef *hspi, I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart)
{
	// Decode packet to see which data points to get. 
	// Access I2C FRAM and read the proper data points
	// Go to transmit mode, send the data down to earth
		
	char msg2[100] = {0};
	uint32_t startTime = ((((uint32_t)packet[1]) << 16) | (((uint32_t)packet[2]) << 8) | ((uint32_t) packet[3]));
	uint8_t chunk = packet[4];
	uint8_t endTime = startTime + chunk;
	
	struct ScienceDataPoint sdp[chunk];
	
	FRAM_IO_Search_Start(hi2c, startTime, endTime, huart);

	
	// Get each of the data points
	struct ScienceDataPackage dataPoint;
	FRAM_Return out = FRAM_IO_Search_GetNextItem(hi2c, &dataPoint);
//	snprintf(msg2, 100, "\nTime: 0x%08x    Energy: 0x%02x\n", dataPoint.Time, dataPoint.Energy);
//	HAL_UART_Transmit(huart, (uint8_t *) msg2, sizeof(msg2), 1);
	
	for (int16_t i = 0; i < chunk; i++) 
	{
		if (out == FRAM_SEARCH_ENDED)
		{
			snprintf(msg2, 100, "\nFRAM_SEARCH_ENDED: %d\n", i);
			HAL_UART_Transmit(huart, (uint8_t *) msg2, sizeof(msg2), 1);
			break;
		}
		memcpy(&sdp[i], &dataPoint, 5);
		out = FRAM_IO_Search_GetNextItem(hi2c, &dataPoint);
	}

	// Debugging
	snprintf(msg2, 100, "\nTime: 0x%08x    Energy: 0x%02x\n", sdp[0].Time, sdp[0].Energy);
	HAL_UART_Transmit(huart, (uint8_t *) msg2, sizeof(msg2), 1);
	
	snprintf(msg2, 100, "\nTime: 0x%08x    Energy: 0x%02x\n", sdp[1].Time, sdp[1].Energy);
	HAL_UART_Transmit(huart, (uint8_t *) msg2, sizeof(msg2), 1);
	
	snprintf(msg2, 100, "\nTime: 0x%08x    Energy: 0x%02x\n", sdp[2].Time, sdp[2].Energy);
	HAL_UART_Transmit(huart, (uint8_t *) msg2, sizeof(msg2), 1);
	
	// Create the science payload packet
	uint8_t sciDataPacket[FIXED_PACK_SIZE] = {0};
	Create_ScienceData(sciDataPacket, sdp, 3, 2458615.42743);
	
	// Transmit the packet back to the ground station
	CC1200_Transmit_Packet(sciDataPacket, FIXED_PACK_SIZE, hspi, huart);
	
}

/**
  * @brief This function will handle request location packets sent in by the ground station
  * @param The packet from the ground station
	* @param SPI connected to the CC1200
	* @param UART used for debugging
	* @param SPI connected to the shared SPI FRAM
  * @retval None
  */
void Handle_ReqLoc_Packet(uint8_t *packet, SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, SPI_HandleTypeDef *fram_hspi)
{
	// Access SPI FRAM to get the latitude, longitude, and altitude
	// Create a packet that will go back to the ground station
	
	// The location data is currently being generated via the mechanical simulation
	// The location is the longitude, latitude, and altitude each as a floating point 
	float longitude, latitude, altitude;
	uint8_t floatBytes[4] = {0};
	
	// Get the latitude
	SPI_FRAM_Read(fram_hspi, SPI_FRAM_LATITUDE_ADDR, floatBytes, 4, huart, 0);
	latitude = bytes_to_float(floatBytes);
	
	// Get the longitude
	SPI_FRAM_Read(fram_hspi, SPI_FRAM_LONGITUDE_ADDR, floatBytes, 4, huart, 0);
	longitude = bytes_to_float(floatBytes);
	
	// Get the altitude
	SPI_FRAM_Read(fram_hspi, SPI_FRAM_ALTITUDE_ADDR, floatBytes, 4, huart, 0);
	altitude = bytes_to_float(floatBytes);
	
	// Create the packet with the current location
	uint8_t locationPacket[FIXED_PACK_SIZE] = {0};
	Create_LocationData(locationPacket, latitude, longitude, altitude, 2458615.42743);
	
	char msg1[100] = {0};
	HAL_Delay(10);
	
	// Debugging
	snprintf(msg1, 100, "\nLat: %f   Longit: %f    Alt: %f\n", latitude, longitude, altitude);
	HAL_UART_Transmit(huart, (uint8_t *) msg1, sizeof(msg1), 1);
	
	// Transmit the packet back to the ground station
	CC1200_Transmit_Packet(locationPacket, FIXED_PACK_SIZE, hspi, huart);
}

/**
  * @brief This function will decode pdate location packets that have come into the CubeSat
  * @param The packet from the ground station
	* @param SPI connected to the CC1200
	* @param UART used for debugging
	* @param SPI connected to the shared SPI FRAM
  * @retval None
  */
void Handle_UpdateKep_Packet(uint8_t *packet, SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, SPI_HandleTypeDef *fram_hspi)
{
	// Read new keplerian elements. 
	// Store them into the shared SPI FRAM.
	// Send an interrupt to the mechanical board to read the new location data
	
	// Not needed by SlugSat 2018-2019. Should be implemented next year to update
	// the position of the CubeSat in the event that the CubeSat has a faulty position
}





// Not needed by SlugSat 2018-2019. Should be implemented next year.
// Implement these functions so that the ground station can handle and 
// decode packets sent to it by the CubeSat.
# if 0
/* These functions will print out the response from the CubeSat */
void Handle_Sat_Status(uint8_t *packet)
{
	// Read the status
	uint8_t status = packet[9];
	
	// Print out the status of the requested packet
	//Hal_UART_Transmit(&huart, 
	
}

void Handle_Sat_Sci(uint8_t *packet)
{
	char buffer[100] = {0};
	
	snprintf(&buffer[0],  33, "Time: %f\tData: %u\n", bytes_to_float(&packet[9]),  packet[13]);
	snprintf(&buffer[33], 33, "Time: %f\tData: %u\n", bytes_to_float(&packet[14]),  packet[18]);
	snprintf(&buffer[66], 33, "Time: %f\tData: %u\n", bytes_to_float(&packet[19]),  packet[23]);
	
	// Hal_UART_Transmit(&huart, buffer,

}

uint8_t Handle_Sat_Ack(uint8_t *packet, uint8_t hashValue)
{
	// Check if the proper hash value was given
	if (packet[9] == hashValue)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void Handle_Sat_Location(uint8_t *packet)
{
	
}
#endif

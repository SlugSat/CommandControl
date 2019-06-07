#include "Sci_Tel_SM.h"

/* Peripheral Handlers */
static I2C_HandleTypeDef *hi2cFRAM;
static SPI_HandleTypeDef *hspiCC;
static SPI_HandleTypeDef *hspiFRAM;
static UART_HandleTypeDef *debugUART;

// Initialize the initial state
static States state;

void Sci_Tel_SM_Init(I2C_HandleTypeDef *hi2c, SPI_HandleTypeDef *hspiRadio, SPI_HandleTypeDef *hspiMem, UART_HandleTypeDef *huart)
{
	hi2cFRAM = hi2c;
	hspiCC = hspiRadio;
	hspiFRAM = hspiMem;
	debugUART = huart;
	
	
	char msg1[100] = {0};
	char msgClear[100] = {0};
	
	snprintf((char *)msg1, 100, "\n\n-----Start the test-----\n\n");
	HAL_UART_Transmit(debugUART, (uint8_t *) msg1, strlen(msg1), 10);
	
	/* Initialization code goes here */
	//Set CS high
	HAL_GPIO_WritePin(GPIOA, SPI_CC_CS_Pin, GPIO_PIN_SET);
	SPI_FRAM_Init(hspiFRAM);

	//Set reset high, low, high to begin
	HAL_GPIO_WritePin(SP_CC_RESET_GPIO_Port, SP_CC_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(SP_CC_RESET_GPIO_Port, SP_CC_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(SP_CC_RESET_GPIO_Port, SP_CC_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	
	uint8_t readValue;
	
	// Set a science event to off mode
	HAL_GPIO_WritePin(Science_Event_GPIO_Port, Science_Event_Pin, GPIO_PIN_RESET);	
	
	readValue = ReadWriteCommandReg(hspiCC, CC1200_SNOP); // Check the state of the CC1200
	memcpy(msg1, msgClear, 100);
	snprintf((char *)msg1, 100, "\nState of the CC1200: 0x%02x\n", readValue);
	HAL_UART_Transmit(debugUART, (uint8_t *) msg1, strlen(msg1), 10);
	
	// Initialize the I2C FRAM
	//Wipe_Memory(&hi2c1, 0, 1);
	//Wipe_Memory(&hi2c1, 1, 1);

	FRAM_IO_Init(hi2cFRAM);
	FRAM_Write_Headers(hi2cFRAM);
	
	CC1200_INIT(hspiCC);
	//ReadWriteCommandReg(&hspi1, CC1200_SFTX); // Flush TX FIFO
	ReadWriteCommandReg(hspiCC, CC1200_SFRX); // Flush RX FIFO
	ReadWriteCommandReg(hspiCC, CC1200_SRX);  // Enter receive mode
	
	do 
	{
		readValue = ReadWriteCommandReg(hspiCC, CC1200_SNOP); // Check the state of the CC1200
		
		HAL_Delay(500);
		memcpy(msg1, msgClear, 100);
		snprintf((char *)msg1, 100, "\nState of the CC1200: 0x%02x\n", readValue);
		HAL_UART_Transmit(debugUART, (uint8_t *) msg1, strlen(msg1), 10);
		
		ReadWriteCommandReg(hspiCC, CC1200_SRX);
		HAL_Delay(10);
	} while ((readValue & 0x10) != 0x10);

	state = Fetch;
}


void Sci_Tel_SM_Run(void)
{
	char msg1[100] = {0};
	char msgClear[100] = {0};
	uint8_t packet[FIXED_PACK_SIZE] = {0};
		
	memcpy(msg1, msgClear, 100);
	snprintf((char *)msg1, 100, "\nState: 0x%02x\n", state);
	HAL_UART_Transmit(debugUART, (uint8_t *) msg1, strlen(msg1), 10);
	switch (state)
	{
		/* Fetch a packet, location data, or science events mode */
		case (Fetch): 
			// Poll buffers and FRAM to check if an event has occurred
			if 			(Poll_Receive_Packet() == SUCCESS) // Poll for if a packet is received
			{
				state = Decode;
			}
			else if (Poll_FRAM_Location() == SUCCESS) // Poll the location of the CubeSat to check if we are over the equator
			{
				state = Science_Location;
			}
			else if (Poll_FRAM_Time() == SUCCESS) // Poll the the log event for a certain time
			{
				state = Science_Time;
			}
			else
			{
				state = Fetch;
			}
			HAL_Delay(1500); // Poll every 1.5 seconds, implement using sleep mode in the future or add a timer
			break;
		/* Decode a packet and respond accordingly mode */
		case (Decode):
			
			// Get the packet in the RX FIFO
			for (int i = 0; i < FIXED_PACK_SIZE; i++)
			{
				packet[i] = 0;
				packet[i] = ReadWriteExtendedReg(hspiCC, CC1200_READ_BIT, CC1200_RXFIFO, 0);
			}
			memcpy(msg1, msgClear, 100);
			snprintf((char *)msg1, 100, "\nPacket: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", packet[0], packet[1], packet[2], packet[3], packet[4], packet[5]);
			HAL_UART_Transmit(debugUART, (uint8_t *) msg1, strlen(msg1), 10);
			
			// Decode the packet and take action based on the packet
			Decode_Sat_Packet(packet, hspiCC, debugUART, hspiFRAM, hi2cFRAM);
			
			state = Fetch;
			ReadWriteCommandReg(hspiCC, CC1200_SIDLE);
			ReadWriteCommandReg(hspiCC, CC1200_SFRX); // Flush RX FIFO
			ReadWriteCommandReg(hspiCC, CC1200_SRX);
			break;
		/* Log science data based on time mode */	
		case (Science_Time): 
			// Signal to the power modes to turn on the science payload system
			HAL_GPIO_WritePin(Science_Event_GPIO_Port, Science_Event_Pin, GPIO_PIN_SET);	
			// Generate fake data and store it to the I2C FRAM
			Log_Science_Data();
			HAL_Delay(5000);
			HAL_GPIO_WritePin(Science_Event_GPIO_Port, Science_Event_Pin, GPIO_PIN_RESET);	
			state = Fetch;
			break;
		/* Log science data based on location mode */
		case (Science_Location): 
			// Signal to the power modes to turn on the science payload system
			HAL_GPIO_WritePin(Science_Event_GPIO_Port, Science_Event_Pin, GPIO_PIN_SET);	
			// Generate fake data and store it to the I2C FRAM 
			Log_Science_Data();
			HAL_Delay(5000);
			HAL_GPIO_WritePin(Science_Event_GPIO_Port, Science_Event_Pin, GPIO_PIN_RESET);
			state = Fetch;
			break;
		/* An error occurred */
		default:
			break;
	}
}


// Poll the receive buffer to check if a packet was received
uint8_t Poll_Receive_Packet(void)
{
	// Check the state of the chip in the event that an error occurred
	uint8_t readValue = ReadWriteCommandReg(hspiCC, CC1200_SNOP); 
	HAL_Delay(8);
	
	if ((readValue & 0x60) == 0x60) // Flush if FIFO error
	{
		ReadWriteCommandReg(hspiCC, CC1200_SFRX); 
		HAL_Delay(10);
	}
	
	if ((readValue & 0x10) != 0x10) // Re-enter transmit mode if there was a FIFO error
	{
		ReadWriteCommandReg(hspiCC, CC1200_SRX);
	}

	// Poll the receive buffer
	uint8_t rxBytes = ReadWriteExtendedReg (hspiCC, CC1200_READ_BIT, CC1200_NUM_RXBYTES, 0);
	
	// Debugging
	readValue = ReadWriteCommandReg(hspiCC, CC1200_SNOP); 
	char msg1[100] = {0};
	snprintf((char *)msg1, 100, "\n\nNumber of bytes in the receive buffer: 0x%02x    State of CC1200: 0x%02x\r\n", rxBytes, readValue);
	HAL_UART_Transmit(debugUART, (uint8_t *) msg1, strlen(msg1), 10);
	
	
	if (rxBytes <= FIXED_PACK_SIZE)
	{
		return FAIL;
	}
	else
	{
		return SUCCESS;
	}
}

// Poll the housekeeping FRAM to check the location of the CubeSat
uint8_t Poll_FRAM_Location(void)
{
	// Write code here that would access the shared SPI FRAM and get if a science event should be logged based on location
	uint8_t lattBytes[4] = {0};
	SPI_FRAM_Read(hspiFRAM, SPI_FRAM_LATITUDE_ADDR, lattBytes, 4, debugUART, 0);
	
	float lattitude = bytes_to_float(lattBytes);
	if (lattitude < 15.0 && lattitude > -15.0)
	{
		return SUCCESS;
	}
	return FAIL;
}

// Poll the housekeeping FRAM to check the if it is time to log a science event
uint8_t Poll_FRAM_Time(void)
{
	// Write code here that would access the shared SPI FRAM and get if a science event should be logged based on time
	// Needs to be completed by next years team. For now, this will just log data every 90 seconds. 
	static int i = 0;
	if ((i++ % 40) == 20) 
	{
		return SUCCESS;
	}
	return FAIL;
}

void Log_Science_Data(void)
{
	static uint32_t time;
	uint8_t energy;
	
	struct ScienceDataPackage fakeDataPoint;
	
	for (int i = 0; i < 10; i++)
	{
		// Generate test data points
		GenerateTestData(&energy, &time);	
		// Build data into a package
		FRAM_Data_Builder(&fakeDataPoint, time, energy);
		// Write the data point into the FRAM
		FRAM_IO_Write(hi2cFRAM, &fakeDataPoint, debugUART);
	}	
}
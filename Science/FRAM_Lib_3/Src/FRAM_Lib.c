/*
FRAM_Lib.c



*/

/* Includes ------------------------------------------------------------------*/
#include "FRAM_Lib.h"
	
#include <string.h>
#include <stdio.h>

/* Defines -------------------------------------------------------------------- */
#define MAX_FRAM_MODULES 8

//Set to number of FRAM in system
#define NUMFRAM 2

//Sizes
#define FRAM32KB 32768

#define ENERGY_BYTES 1
#define ENERGY_BITS (ENERGY_BYTES*8)

#define TIME_BYTES 4
#define TIME_BITS (TIME_BYTES*8)

#define STORAGE_SIZE_BITS (TIME_BITS + ENERGY_BITS)
#define STORAGE_SIZE_BYTES (TIME_BYTES + ENERGY_BYTES)

#define NUM_DATA_POINTS_PER_FRAM (FRAM32KB - DATA_START) / STORAGE_SIZE_BYTES


//Locations
#define HEADER_END 0x10
#define DATA_START (HEADER_END + 1)

#define FINAL_DATA_ADDRESS (NUM_DATA_POINTS_PER_FRAM + DATA_START)
#define CENTER_DATA_ADDRESS ((NUM_DATA_POINTS_PER_FRAM * STORAGE_SIZE_BYTES / 2) + DATA_START)
#define CENTER_DATA_COUNT (NUM_DATA_POINTS_PER_FRAM / 2)


//I2C
#define TIMEOUT 10


/* Variables ------------------------------------------------------------------*/

//Becomes true when FRAM_IO_Init() is run. Used to check if headers have been set yet
uint8_t isInitilized = 0;

uint32_t currentMemAddress;
uint8_t currentFRAM;

//Search Variables
int8_t searchFram = -1;
uint16_t searchAddress;

/* Functions --------------------------------------------------------------------*/

FRAM_Return FRAM_IO_Init(I2C_HandleTypeDef *i2c_handler){
	for(int i = 0; i < NUMFRAM;  i++){
		//Check if each of the expected FRAM is there, if one is not return error
		//Address is 1010_ _ _ 0/1
		if(HAL_I2C_IsDeviceReady(i2c_handler, 0xA0 | (i << 1)+1, 5, TIMEOUT) != HAL_OK){
			return FRAM_ERROR;
		}
	}
	
	//If it has not been initilized setup headers for first time
	if(isInitilized == 0){
		currentMemAddress = DATA_START;
		currentFRAM = 0;	
		isInitilized = 1;
	}
	
	return FRAM_SUCCESS;
}

FRAM_Return FRAM_Write_Headers(I2C_HandleTypeDef *i2c_handler){
	uint32_t StartTime = 0;
	uint32_t EndTime = 0;
	
	uint8_t DataBufferIn[16] = {0};
	uint8_t	clear[HEADER_END] = {0};
	
	/*
	Build up the header buffer
	*/
	DataBufferIn[0] = 0; 												//Address Bit
	DataBufferIn[1] = currentFRAM;							//Current write head FRAM
	uint8_t* p = (uint8_t*)&currentMemAddress;	
	for(int i=0; i<4; i++){
		DataBufferIn[2+i] = p[3-i];								//Cut up and store current write head address
	}
	DataBufferIn[6] = 0x9;											//Alighnment bits
	for(int i=0; i<TIME_BYTES; i++){
		DataBufferIn[7+i] = 0;                    //Space for start time
	}	
	DataBufferIn[11] = 0x9;											//Alighnment bits
	for(int i=0; i<TIME_BYTES; i++){
		DataBufferIn[12+i] = 0;                   //Space for end time
	}	
	
	//Clear and write each FRAM
	for(int i = 0; i < NUMFRAM;  i++){
		DataBufferIn[0] = i;
		//Zero out header space
		if(HAL_I2C_Mem_Write(i2c_handler, 0xA0 | (i << 1) + 1, 0x0, 0x08, clear, 16, 10) != HAL_OK){
			return FRAM_ERROR;
		}
		
		//Fill header space with built buffer
		if(HAL_I2C_Mem_Write(i2c_handler, 0xA0 | (i << 1) + 1, 0x0, 0x08, DataBufferIn, 16, 10) != HAL_OK){
			return FRAM_ERROR;
		}
	}
	
	return FRAM_SUCCESS;
}

FRAM_Return FRAM_Read_Header(I2C_HandleTypeDef *i2c_handler, uint8_t memNum, uint8_t *currentWriteHead, uint32_t *currentWriteHeadAddress, uint32_t *startTime, uint32_t *endTime){
	if(memNum > NUMFRAM-1){
		return FRAM_ERROR;
	}
	
	uint8_t DataBufferOut[16] = {0};
	
	//Read in full header, 16 bytes
	if(HAL_I2C_Mem_Read(i2c_handler, 0xA0 | (memNum << 1) + 1, 0x0, 0x08, DataBufferOut, 16, 10) != HAL_OK){
		return FRAM_ERROR;
	}
	
	//Break apart bytes into data
	*currentWriteHead = DataBufferOut[1];													//Extract current write head
	for(int i=0; i<4; i++){
		*currentWriteHeadAddress |= (DataBufferOut[i+2] << (3-i)*8);	//Rebuild write head address		
	}
	for(int i=0; i<TIME_BYTES; i++){
		*startTime |= (DataBufferOut[i+7] << (TIME_BYTES-1-i)*8);		//Rebuild start time		
	}
	for(int i=0; i<TIME_BYTES; i++){
		*endTime |= (DataBufferOut[i+12] << (TIME_BYTES-1-i)*8);			//Rebuild end tie		
	}
	
	return FRAM_SUCCESS;
}

FRAM_Return FRAM_IO_Write(I2C_HandleTypeDef *i2c_handler, struct ScienceDataPackage *scienceData){
	//Avoid running off the end of the FRAM
	if(FRAM32KB - currentMemAddress < STORAGE_SIZE_BYTES){
		//Not enough space left in current FRAM
		//Last FRAM, wrap around
		if(currentFRAM == MAX_FRAM_MODULES - 1){
			currentFRAM = 0;
			currentMemAddress = DATA_START;
		}
		//Next FRAM
		else{
			currentFRAM++;
			currentMemAddress = DATA_START;
		}
	}

	//Read in the times from the header in order to decide if start and end time need to be updated
	uint32_t startTime, endTime;
	//Start (7)
	if(Retrieve_Time(i2c_handler, currentFRAM, 7, &startTime) != FRAM_SUCCESS){
		return FRAM_ERROR;
	}
	//End (12)
	if(Retrieve_Time(i2c_handler, currentFRAM, 12, &endTime) != FRAM_SUCCESS){
		return FRAM_ERROR;
	}
	
	//Store Energy Level
	uint8_t DataIn = scienceData->Energy;
	if(HAL_I2C_Mem_Write(i2c_handler, 0xA0 | (currentFRAM << 1) + 1, currentMemAddress, 0x08, &DataIn, 1, 10) != HAL_OK){
		return FRAM_ERROR;
	}
	
	//Store Time Data
	uint32_t Time = scienceData->Time;
	if(Store_Time(i2c_handler, currentFRAM, currentMemAddress+1, Time) != FRAM_SUCCESS){		
		return FRAM_ERROR;
	}else{
		//If earlier time then in header, update
		if(startTime > Time){
			if( Store_Time(i2c_handler, currentFRAM, 7, Time) != FRAM_SUCCESS){		
				return FRAM_ERROR;
			}		
		}
		//If later time then in header, update
		if(endTime < Time){
			if( Store_Time(i2c_handler, currentFRAM, 12, Time) != FRAM_SUCCESS){		
				return FRAM_ERROR;
			}				
		}
	}
	
	currentMemAddress = currentMemAddress + STORAGE_SIZE_BYTES;
	
	//Store new mem address 
	uint8_t DataBufferIn[4] = {0};
	uint8_t* p = (uint8_t*)&currentMemAddress;	
	for(int i=0; i<4; i++){
		DataBufferIn[i] = p[3-i];								//Cut up and store current write head address
	}
	if(HAL_I2C_Mem_Write(i2c_handler, 0xA0 | (currentFRAM << 1) + 1, 0x2, 0x08, DataBufferIn, 4, 10) != HAL_OK){
		return FRAM_ERROR;
	}
	
	
	return FRAM_SUCCESS;
}


//Function unimplimented, do not use
FRAM_Return FRAM_IO_Write_To_Address(I2C_HandleTypeDef *i2c_handler, struct ScienceDataPackage *scienceData, uint8_t framNum, uint8_t address){
	
//	uint8_t DataIn[9];
//	DataIn[0] = scienceData->Energy;
//	DataIn[1] = scienceData->Time;
//	
//	HAL_I2C_Mem_Write(i2c_handler, 0xA0 | (framNum << 1) + 1, address, 0x08, DataIn, 1, 10);
	
	return FRAM_ERROR;
}

/**
* 
* @param  
**/
FRAM_Return FRAM_IO_Search_Start(I2C_HandleTypeDef *i2c_handler, uint32_t StartTime, uint32_t EndTime, UART_HandleTypeDef *uart_handler){
	uint32_t lowTime, highTime, searchTime;
	uint8_t currentWriteHead; 
	uint32_t currentWriteHeadAddress;
	searchFram = -1;
	
	uint8_t StrOut[25] = {0};
	
	for(int i=0; i < NUMFRAM; i++){
		//Get high and low times
		if(FRAM_Read_Header(i2c_handler, i, &currentWriteHead, &currentWriteHeadAddress, &lowTime, &highTime) != FRAM_SUCCESS){		
			return FRAM_ERROR;
		}
		
		//Delay necessary for Header to fully retrieve. 
		//Retrieval needs to be optimized to remove this
		HAL_Delay(500);	

		//Check if start is between high and low
		if(StartTime > lowTime && StartTime < highTime){
			
			sprintf(StrOut,"FRAM: %d, Low: %d, High: %d \n",i,lowTime,highTime);
			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
			
			searchFram = i;
		}
	}
	
	if(searchFram == -1){
		return FRAM_TIME_NOT_FOUND;
	}
	//Binary Search time block
	
	//searchAddress = CENTER_DATA_ADDRESS;
	uint16_t searchNum = CENTER_DATA_COUNT;	
	
	//Read in time block and binary search
	int iteration = 0;
	while(1){
		
		searchAddress = (searchNum * STORAGE_SIZE_BYTES) + DATA_START;
		
		if(Retrieve_Time(i2c_handler, searchFram, searchAddress + ENERGY_BYTES, &searchTime) != FRAM_SUCCESS){		
			return FRAM_ERROR;
		}
		
		iteration++;
		if(searchTime == StartTime){
			break;
		}else if(iteration > 11){
			break;
		}else if(StartTime > searchTime){
			searchNum = searchNum + (searchNum/2);
		}else{
			searchNum = searchNum/2;
		}
	}
	
	sprintf(StrOut,"FRAM: %d, Address: %d \n",searchFram,searchAddress);
	HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
	
	return FRAM_SUCCESS;
}

/**
* 
* @param  
**/
FRAM_Return FRAM_IO_Search_GetNextItem(I2C_HandleTypeDef *i2c_handler, struct ScienceDataPackage *DataPack){
	if(searchFram == -1){
		return FRAM_ERROR;
	}
	
	uint8_t tOut[8] = {0};
	uint8_t eOut;
	
	if(HAL_I2C_Mem_Read(i2c_handler, 0xA0 | (searchFram << 1) + 1, searchAddress, 0x08, tOut,8, 10) != HAL_OK){
		return FRAM_ERROR;
	}
	if(HAL_I2C_Mem_Read(i2c_handler, 0xA0 | (searchFram << 1) + 1, searchAddress+8, 0x08, &eOut,1, 10) != HAL_OK){
		return FRAM_ERROR;
	}
	
	//rebuild 32 bit number
	DataPack->Time = 0;
	for(int i=0; i<4; i++){
		DataPack->Time |= (tOut[i] << (3-i)*8);
	}
	
	DataPack->Energy = eOut;
	
	searchAddress = searchAddress + STORAGE_SIZE_BYTES;
	
	return FRAM_SUCCESS;
}

/**
* 
* @param  
**/
FRAM_Return FRAM_IO_Search_EndSearch(){
	searchFram = -1;
	return FRAM_SUCCESS;
}

FRAM_Return FRAM_Reset(I2C_HandleTypeDef *i2c_handler){
	
	currentFRAM = 0;
	currentMemAddress = 0;
	
	uint8_t DataBufferIn[] = {0,currentFRAM,currentMemAddress};
	
	for(int i = 0; i < NUMFRAM;  i++){
		DataBufferIn[0] = i;
		if(HAL_I2C_Mem_Write(i2c_handler, 0xA0 | (i << 1) + 1, 1, 0x08, &DataBufferIn[0], 3, 10) != HAL_OK){
			return FRAM_ERROR;
		}
	}
	return FRAM_SUCCESS;
}

FRAM_Return FRAM_Data_Builder(struct ScienceDataPackage *DataPack, uint32_t Time, FRAM_Science_Levels Energy){
	if(DataPack == NULL){
		return FRAM_ERROR;
	}
	
	DataPack->Time = Time;
	DataPack->Energy = Energy;
	
	return FRAM_SUCCESS;
}

/* Helper Functions *******************************************************************************************/

FRAM_Return Retrieve_Time(I2C_HandleTypeDef *i2c_handler, uint8_t FRAM_Num, uint16_t address, uint32_t *output){
	uint8_t tOut[4] = {0};
	output = 0;
	
	if(HAL_I2C_Mem_Read(i2c_handler, 0xA0 | (FRAM_Num << 1) + 1, address, 0x08, tOut, 4, 10) != HAL_OK){
		return FRAM_ERROR;
	}
	
	//rebuild 32 bit number
	for(int i=0; i<4; i++){
		*output |= (tOut[i] << (3-i)*8);		
	}

	return FRAM_SUCCESS;
}

FRAM_Return Store_Time(I2C_HandleTypeDef *i2c_handler, uint8_t FRAM_Num, uint16_t address, uint32_t time){
	uint8_t tIn[4] = {0};
	uint8_t* p = (uint8_t*)&time;
	
	for(int i=0; i<4; i++){
		tIn[i] = p[3-i];
	}
	
	if(HAL_I2C_Mem_Write(i2c_handler, 0xA0 | (FRAM_Num << 1) + 1, address, 0x08, tIn, 4, 10) != HAL_OK){
			return FRAM_ERROR;
	}

	return FRAM_SUCCESS;
}

FRAM_Return Retrieve_Full_Package(I2C_HandleTypeDef *i2c_handler, uint8_t FRAM_Num, uint16_t address, struct ScienceDataPackage *DataPack){
	//Retrieve Energy
	uint8_t EOut = 0;
	if(HAL_I2C_Mem_Read(i2c_handler, 0xA0 | (FRAM_Num << 1) + 1, address, 0x08, &EOut, 1, 10) != HAL_OK){
		return FRAM_ERROR;
	}
	DataPack->Energy = EOut;
	
	//Retrieve Time
	uint32_t TOut = 0;
	if(Retrieve_Time(i2c_handler, 0xA0 | (FRAM_Num << 1) + 1,address+1, &TOut) != HAL_OK){
			return FRAM_ERROR;
	}
	DataPack->Time = TOut;
	
	return FRAM_SUCCESS;
}

FRAM_Return Wipe_Memory(I2C_HandleTypeDef *i2c_handler, uint8_t FRAM_Num, uint8_t Mode){
	int i =0;
	uint8_t zero = 0;
	
	if(Mode){
		//Clean All
			for(i = 0; i < FRAM32KB; i++){
				if(HAL_I2C_Mem_Write(i2c_handler, 0xA0 | (FRAM_Num << 1) + 1, i, 0x08, &zero, 1, 10) != HAL_OK){
					return FRAM_ERROR;
				}
			}
	}else{
		//Clean First 1000 KB
			for(i = 0; i < 1000; i++){
				if(HAL_I2C_Mem_Write(i2c_handler, 0xA0 | (FRAM_Num << 1) + 1, i, 0x08, &zero, 1, 10) != HAL_OK){
					return FRAM_ERROR;
				}
			}		
	}
		
	
	return FRAM_SUCCESS;
}


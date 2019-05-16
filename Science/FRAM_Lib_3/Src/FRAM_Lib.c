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
uint16_t searchAddress = 0;
uint32_t stopTime = 0;

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
	DataBufferIn [1] = currentFRAM;							//Current write head FRAM
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

FRAM_Return FRAM_IO_Write(I2C_HandleTypeDef *i2c_handler, struct ScienceDataPackage *scienceData, UART_HandleTypeDef *uart_handler){
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
	uint32_t startTime = 0;
	uint32_t endTime = 0;
	uint8_t dumpValue1 = 0;
	uint32_t dumpValue2 = 0;
//	//Start (7)
//	if(Retrieve_Time(i2c_handler, currentFRAM, 7, &startTime) != FRAM_SUCCESS){
//		return FRAM_ERROR;
//	}
//	//End (12)
//	if(Retrieve_Time(i2c_handler, currentFRAM, 12, &endTime) != FRAM_SUCCESS){
//		return FRAM_ERROR;
//	}
	
	if(FRAM_Read_Header(i2c_handler, currentFRAM,&dumpValue1,&dumpValue2,&startTime,&endTime) != FRAM_SUCCESS){
		return FRAM_ERROR;
	}
	
	//Store Energy Level
	uint8_t DataIn = scienceData->Energy;
	if(HAL_I2C_Mem_Write(i2c_handler, 0xA0 | (currentFRAM << 1) + 1, currentMemAddress, 0x08, &DataIn, 1, 10) != HAL_OK){
		return FRAM_ERROR;
	}
	
	uint8_t StrOut[50];
	
	//Store Time Data
	uint32_t Time = scienceData->Time;
	if(Store_Time(i2c_handler, currentFRAM, currentMemAddress+1, Time) != FRAM_SUCCESS){		
		return FRAM_ERROR;
	}
	//If earlier time then in header, update
	if((startTime == 0) || (startTime > Time)){
		//sprintf(StrOut,"Start Time Replaced in %d With: %d which is less then: %d\n",currentFRAM, Time, startTime);
		if( Store_Time(i2c_handler, currentFRAM, 7, Time) != FRAM_SUCCESS){		
			return FRAM_ERROR;
		}
	}
	//If later time then in header, update
	if(endTime < Time){
		//sprintf(StrOut,"End Time Replaced in %d With: %d which is greater then: %d\n",currentFRAM, Time, endTime);
		if( Store_Time(i2c_handler, currentFRAM, 12, Time) != FRAM_SUCCESS){		
			return FRAM_ERROR;
		}				
	}
	
	//HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);	
	uint8_t currentWriteHead = 0;
	uint32_t currentWriteHeadAddress = 0;
	uint32_t sT = 0;
	uint32_t eT = 0;
	
//		if(FRAM_Read_Header(i2c_handler, 1, &currentWriteHead, &currentWriteHeadAddress, &sT, &eT) != FRAM_SUCCESS){
//			sprintf(StrOut,"FRAM %d Read Error, possibly not active\r\n", 1);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);			
//		}
//			sprintf(StrOut,"Header %d: WrtHd: %d, WrtHdAd: %d, Str: %d, End: %d\n", 1, currentWriteHead, currentWriteHeadAddress, sT, eT);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);					
 
	currentMemAddress = currentMemAddress + STORAGE_SIZE_BYTES;
	
	//Store new mem address 
	uint8_t DataBufferIn[4] = {0};
	uint8_t* p = (uint8_t*)&currentMemAddress;	
	for(int i=0; i<4; i++){
		DataBufferIn[i] = p[3-i];								//Cut up and store current write head address
	}
	for(int i = 0; i < NUMFRAM;  i++){
		if(HAL_I2C_Mem_Write(i2c_handler, 0xA0 | (currentFRAM << 1) + 1, 0x2, 0x08, DataBufferIn, 4, 10) != HAL_OK){
			return FRAM_ERROR;
		}
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
FRAM_Return FRAM_IO_Search_Start(I2C_HandleTypeDef *i2c_handler, uint32_t StartTime, uint32_t EndTime){
	uint32_t lowTime = 0;
	uint32_t highTime = 0;
	uint32_t searchTime = 0;
	uint8_t currentWriteHead = 0;
	uint32_t currentWriteHeadAddress = 0;
	searchFram = -1;
	
	uint8_t StrOut[25] = {0};
	
	stopTime = EndTime;
	
	for(int i=0; i < NUMFRAM; i++){
		lowTime = 0;
		highTime = 0;
		//Get high and low times
		if(FRAM_Read_Header(i2c_handler, i, &currentWriteHead, &currentWriteHeadAddress, &lowTime, &highTime) != FRAM_SUCCESS){
//			sprintf(StrOut,"FRAM %d Read Error, possibly not active\r\n", i);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);			
			return FRAM_ERROR;
		}
		
		//Delay necessary for Header to fully retrieve. 
		//Retrieval needs to be optimized to remove this
		HAL_Delay(1000);	

		//Check if start is between high and low
//		sprintf(StrOut,"FRAM: %d, Low: %d, High: %d, Search: %d \n",i,lowTime,highTime,StartTime);
//		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
		
		if(StartTime > lowTime && StartTime < highTime){
//			sprintf(StrOut,"Target time %d found in FRAM %d between %d and %d \n", StartTime,i,lowTime,highTime);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
			searchFram = i;
			break;
		}else{
//			HAL_Delay(1000);	
//			sprintf(StrOut,"Target time %d NOT found in FRAM %d between %d and %d \n", StartTime,i,lowTime,highTime);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);	
			HAL_Delay(1000);	
		}
	}
	
	if(searchFram == -1){
		return FRAM_TIME_NOT_FOUND;
	}
	//Binary Search time block
	
	//searchAddress = CENTER_DATA_ADDRESS;
	//uint16_t searchNum = CENTER_DATA_COUNT;	
	uint32_t searchNum = 0;
	
	//Read in time block and binary search
	//int iteration = 0;
	uint8_t tOut[5] = {0};
	uint32_t lastTime = 0;
	searchAddress = DATA_START;
	uint8_t e = 0;
	
	while(1){
		for(int i = 0; i<5; i++){
			tOut[i] = 0;
		}		
		//HAL_Delay(1000);
		if(HAL_I2C_Mem_Read(i2c_handler, 0xA0 | (searchFram << 1) + 1, searchAddress, 0x08, tOut, 5, 10) != HAL_OK){
			return FRAM_ERROR;
		}
		for(int i=0; i<4; i++){
			searchTime |= (tOut[i+1] << (3-i)*8);		
		}
		e = tOut[0];
		
		//HAL_Delay(1000);
		//sprintf(StrOut,"Searching... SearchNum: %d, Address: %d, E: %d, Time Retrieved: %#x\n",searchNum, searchAddress,e,searchTime);
		//sprintf(StrOut,"Searching... SearchNum: %d, Address: %d, Time Retrieved: %#x %#x %#x %#x\n",searchNum, searchAddress,tOut[0],tOut[1],tOut[2],tOut[3]);
		//HAL_Delay(1000);
		//HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
		//HAL_Delay(1000);
		
		searchAddress = searchAddress + STORAGE_SIZE_BYTES;
//		if(searchAddress >= 32768){
//			return FRAM_ERROR;
//		}
		
		if((StartTime > lastTime) && (StartTime <= searchTime)){
			break;
		}
	
//	sprintf(StrOut,"FRAM: %d, Address: %d, searchTime %d \n",searchFram,searchAddress, searchTime);
//	HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
		
		//iteration++;
//		if(searchTime == StartTime){
//			break;
//		}else if(iteration > 11){
//			break;
//		}else if(StartTime > searchTime){
//			searchNum = searchNum + (searchNum/2);
//		}else{
//			searchNum = searchNum/2;
//		}
	}
	
	sprintf(StrOut,"FRAM: %d, Address: %d, searchTime %d \n",searchFram, searchAddress, searchTime);
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
	
	uint8_t tOut[5] = {0};
	uint8_t eOut = 0;
	DataPack->Time = 0;
	uint8_t StrOut[50];
//	sprintf(StrOut,"searchFram %d \n",searchFram);
//	HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
	
	if(HAL_I2C_Mem_Read(i2c_handler, 0xA0 | (searchFram << 1) + 1, searchAddress, 0x08, tOut,5, 10) != HAL_OK){
		return FRAM_ERROR;
	}
//	if(HAL_I2C_Mem_Read(i2c_handler, 0xA0 | (searchFram << 1) + 1, searchAddress+4, 0x08, &eOut,1, 10) != HAL_OK){
//		return FRAM_ERROR;
//	}
	
	//rebuild 32 bit number

	for(int i=0; i<4; i++){
		DataPack->Time |= (tOut[i+1] << (3-i)*8);
	}
	
	DataPack->Energy = tOut[0];

//	sprintf(StrOut,"searchFram %d, searchAddress %d, E: %d, T: %d \n",searchFram,searchAddress,DataPack->Energy,DataPack->Time);
//	HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
	
	searchAddress = searchAddress + STORAGE_SIZE_BYTES;
	
	if(DataPack->Time > stopTime){
		FRAM_IO_Search_EndSearch();
	}
	
	return FRAM_SUCCESS;
}

/**
* 
* @param  
**/
FRAM_Return FRAM_IO_Search_EndSearch(){
	searchFram = -1;
	return FRAM_SEARCH_ENDED;
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
/*
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
*/
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
/*
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
*/
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


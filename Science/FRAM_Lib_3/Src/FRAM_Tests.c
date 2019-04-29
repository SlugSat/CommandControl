/*
FRAM_Tests.h



*/
/* Defines */

#include "FRAM_Tests.h"
//#include "ScienceData.h"

#define TIMEOUT 10

uint8_t StrOut[50];

uint8_t TotalTests = 0;
uint8_t Successes = 0;
uint8_t Failures = 0;

/* Helper Functions -------------------------------------------------------- */
static uint32_t lastTime = 1;
void GenerateTestData(uint8_t *energy, uint32_t *time, UART_HandleTypeDef *uart_handler){
	*time = (uint32_t)(lastTime + (rand() % (300 + 1 - 1) + 1));
	lastTime = *time;
  *energy = (uint8_t)(rand() % (17 + 1 - 1) + 1);
	//sprintf(StrOut,"T: %d\n", *time);
	//HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);		
}

/* Tests -------------------------------------------------------------------- */

void FRAM_Test(I2C_HandleTypeDef *i2c_handler, UART_HandleTypeDef *uart_handler){
	uint8_t SUCCESS[9] = "Success\n";
	uint8_t ERROR[6] = "Fail\n";
	uint8_t NEWLINE[2] = "\n";
	uint8_t VALIDATION[12] = "Validation: ";
	
	/*
	Check each FRAM connection
	*/
	for(int i = 0; i < 8;  i++){
		//Address is 1010_ _ _1
		if(HAL_I2C_IsDeviceReady(i2c_handler, 0xA0 | (i << 1) + 1, 5, TIMEOUT) == HAL_OK){
			sprintf(StrOut,"FRAM %d is active, zeroing memory\n", i);
			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
			
			//Set Mode to 0 for fast wipe, 1 for full wipe
			if(Wipe_Memory(i2c_handler, i, 0) != FRAM_SUCCESS){
				HAL_UART_Transmit(uart_handler,ERROR,5,10);
			}
		}
		else{
			sprintf(StrOut,"FRAM %d is not active\r\n", i);
			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
		}
	}
	
	HAL_UART_Transmit(uart_handler,NEWLINE,1,10);
	
	/*
	FRAM_IO_Init()
	*/
	uint8_t INIT[20] = "FRAM_IO_Init(): ";
	HAL_UART_Transmit(uart_handler,INIT,16,10);
	
	if(FRAM_IO_Init(i2c_handler) != FRAM_SUCCESS){
		HAL_UART_Transmit(uart_handler,ERROR,6,10);
	}
	else{
		HAL_UART_Transmit(uart_handler,SUCCESS,8,10);
	}

	HAL_UART_Transmit(uart_handler,NEWLINE,1,10);
	
	/*
	FRAM_Write_Headers()
	*/
	
	uint8_t data[16] = {0};
//	if(HAL_I2C_Mem_Read(i2c_handler, 0xA0, 0x0, 0x08, data, 16, 10) != HAL_OK){
//		sprintf(StrOut,"Header Read Error\n");
//		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);			
//	}
//	
//	for(int i=0; i<16; i++){
//		sprintf(StrOut,"Header %d: %d\r\n",i,data[i]);
//		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
//	}
	
	uint8_t WRITE_HEAD[22] = "FRAM_Write_Headers(): ";
	HAL_UART_Transmit(uart_handler,WRITE_HEAD,22,10);
	
	if(FRAM_Write_Headers(i2c_handler) != FRAM_SUCCESS){
		HAL_UART_Transmit(uart_handler,ERROR,6,10);
	}
	else{
		HAL_UART_Transmit(uart_handler,SUCCESS,8,10);
	}	

	/*
	FRAM_Read_Header()
	*/
	
	uint8_t READ_HEAD[21] = "FRAM_Read_Headers()\r\n";
	HAL_UART_Transmit(uart_handler,READ_HEAD,21,10);

	uint8_t currentWriteHead;
	uint32_t currentWriteHeadAddress, startTime, endTime;
	
	for(int i = 0; i < 8;  i++){
		if(FRAM_Read_Header(i2c_handler, i, &currentWriteHead, &currentWriteHeadAddress, &startTime, &endTime) != FRAM_SUCCESS){
			sprintf(StrOut,"FRAM %d Read Error, possibly not active\r\n", i);
			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);			
		}else{ 
			if(currentWriteHead != 0x0){
				sprintf(StrOut,"FRAM %d is not correct, currentWriteHead, %x\r\n", i, currentWriteHead);
				HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);		
			}else{
				if(currentWriteHeadAddress != 0x10 + 1){
					sprintf(StrOut,"FRAM %d is not correct, currentWriteHeadAddress, %x\r\n", i, currentWriteHeadAddress);
					HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);						
				}else{
					if(startTime != 0){
						sprintf(StrOut,"FRAM %d is not correct, startTime, %x\r\n", i, startTime);
						HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
					}else{
						if(endTime != 0){
							sprintf(StrOut,"FRAM %d is not correct, endTime, %x\r\n", i, endTime);
							HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);	
							}else{
								sprintf(StrOut,"FRAM %d Header is correct\r\n", i);
								HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);	
						}				
					}
				}
			}
		}
	}
	
	HAL_UART_Transmit(uart_handler,NEWLINE,1,10);
	
	
	//uint8_t testData[] = {0,0,0};
	
	//HAL_I2C_Mem_Read(i2c_handler, 0xA1, 0x0, 0x08, &testData[0],1, 10);
	//HAL_I2C_Mem_Read(i2c_handler, 0xA1, 0x1, 0x08, &testData[1],1, 10);
	//HAL_I2C_Mem_Read(i2c_handler, 0xA1, 0x2, 0x08, &testData[2],1, 10);
	//sprintf(StrOut,"%d, %d, %d\r\n", testData[0], testData[1], testData[2]);
	//HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);	
	//HAL_UART_Transmit(uart_handler,NEWLINE,1,10);
	
	/*
	FRAM_Data_Builder()
	*/
	
	uint8_t BUILDER[21] = "FRAM_Data_Builder(): ";
	HAL_UART_Transmit(uart_handler,BUILDER,21,10);	
	
	struct ScienceDataPackage TestData;
	
	if(FRAM_Data_Builder(&TestData, 0x19, D2_E3) != FRAM_SUCCESS){
		HAL_UART_Transmit(uart_handler,ERROR,6,10);
	}
	else{
		HAL_UART_Transmit(uart_handler,SUCCESS,8,10);
	}
	
	HAL_UART_Transmit(uart_handler,VALIDATION,12,10);
	
	if(TestData.Time != 0x19 || TestData.Energy != D2_E3){
		HAL_UART_Transmit(uart_handler,ERROR,6,10);
	}
	else{
		HAL_UART_Transmit(uart_handler,SUCCESS,8,10);
	}
	
	uint8_t DATA[32] = "Time: 0b110011, Energy: D2_E3\n";
	HAL_UART_Transmit(uart_handler,DATA,30,10);
	
	
	/*
	FRAM_IO_Write()
	*/
	
	uint8_t WRITE[20] = "FRAM_IO_Write(): ";
	HAL_UART_Transmit(uart_handler,NEWLINE,1,10);
	HAL_UART_Transmit(uart_handler,WRITE,17,10);	
	
	if(FRAM_IO_Write(i2c_handler, &TestData, uart_handler) != FRAM_SUCCESS){
		HAL_UART_Transmit(uart_handler,ERROR,6,10);
	}
	else{
		HAL_UART_Transmit(uart_handler,SUCCESS,8,10);
	}
	
	HAL_UART_Transmit(uart_handler,NEWLINE,1,10);
	
//  uint8_t data[16] = {0};
//	if(HAL_I2C_Mem_Read(i2c_handler, 0xA0, 0x0, 0x08, data, 16, 10) != HAL_OK){
//		sprintf(StrOut,"Header Read Error\n");
//		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);			
//	}
	
//	for(int i=0; i<16; i++){
//		sprintf(StrOut,"Header %d: %d\r\n",i,data[i]);
//		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
//	}
	
	/*
	Mass Data Tests
	*/

	
	uint8_t MASS_DATA[17] = "Mass Data Tests\r\n";
	HAL_UART_Transmit(uart_handler,MASS_DATA,17,10);	
	
	uint8_t MASS_LOAD[16] = "Mass Data Load: ";
	HAL_UART_Transmit(uart_handler,MASS_LOAD,16,10);	
	
//	for(int i=0; i<500; i=i+2){
//		if(FRAM_Data_Builder(&TestData, ScienceTestData[i], (uint8_t)ScienceTestData[i+1]) != FRAM_SUCCESS){
//			HAL_UART_Transmit(uart_handler,ERROR,6,10);
//			
//		}else{
////			sprintf(StrOut,"Data Pack: %d, %d\n",ScienceTestData[i], (uint8_t)ScienceTestData[i+1]);
////			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
//			if(FRAM_IO_Write(i2c_handler, &TestData) != FRAM_SUCCESS){
//				HAL_UART_Transmit(uart_handler,ERROR,6,10);
//			}
//		}
//	}

	currentWriteHead = 0;
	currentWriteHeadAddress = 0;
	startTime = 0;
	endTime = 0;
	
//	for(int i = 0; i < 2;  i++){
//		if(FRAM_Read_Header(i2c_handler, i, &currentWriteHead, &currentWriteHeadAddress, &startTime, &endTime) != FRAM_SUCCESS){
//			sprintf(StrOut,"FRAM %d Read Error, possibly not active\r\n", i);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);			
//		}
//			sprintf(StrOut,"Header %d: WrtHd: %d, WrtHdAd: %d, Str: %d, End: %d\n", i, currentWriteHead, currentWriteHeadAddress, startTime, endTime);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);					
//	}


	uint8_t energy;
	uint32_t time;
	for(int i=0; i<1000; i=i+2){
		GenerateTestData(&energy, &time, uart_handler);
		if(FRAM_Data_Builder(&TestData, time, energy) != FRAM_SUCCESS){
			HAL_UART_Transmit(uart_handler,ERROR,6,10);
			
		}else{
//			sprintf(StrOut,"Data Pack: %d, %d\n",ScienceTestData[i], (uint8_t)ScienceTestData[i+1]);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
			if(FRAM_IO_Write(i2c_handler, &TestData, uart_handler) != FRAM_SUCCESS){
				HAL_UART_Transmit(uart_handler,ERROR,6,10);
			}
		}
	}	
	
//	uint8_t test[30];
//	HAL_I2C_Mem_Read(i2c_handler, 0xA0 | (0 << 1) + 1, 17, 0x08, test, 30, 10);
//	for(int i = 0; i<30; i++){
//		sprintf(StrOut,"Data at %x: %d\n",i+17,test[i]);
//		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);	
//	}
	
	HAL_UART_Transmit(uart_handler,READ_HEAD,21,10);

	currentWriteHead = 0;
	currentWriteHeadAddress = 0;
	startTime = 0;
	endTime = 0;
	
//	for(int i = 0; i < 2;  i++){
//		if(FRAM_Read_Header(i2c_handler, i, &currentWriteHead, &currentWriteHeadAddress, &startTime, &endTime) != FRAM_SUCCESS){
//			sprintf(StrOut,"FRAM %d Read Error, possibly not active\r\n", i);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);			
//		}
//			sprintf(StrOut,"Header %d: WrtHd: %d, WrtHdAd: %d, Str: %d, End: %d\n", i, currentWriteHead, currentWriteHeadAddress, startTime, endTime);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);					
//	}
	
	
	
	HAL_UART_Transmit(uart_handler,SUCCESS,8,10);
	
	uint8_t MASS_RET[20] = "Mass Data Retrieve\n";
	HAL_UART_Transmit(uart_handler,MASS_RET,19,10);		

	uint8_t SEARCH_START[14] = "Search Start: ";
	HAL_UART_Transmit(uart_handler,SEARCH_START,14,10);			

	uint32_t TestStart = 450;
	uint32_t TestEnd = 15000;
	
	FRAM_Return SearchReturn = FRAM_IO_Search_Start(i2c_handler, TestStart, TestEnd, uart_handler);
	if(SearchReturn == FRAM_ERROR){
		HAL_UART_Transmit(uart_handler,ERROR,5,10);
	}
	else if(SearchReturn == FRAM_TIME_NOT_FOUND){
		uint8_t NO_TIME[20] = "Time not found\n";
		HAL_UART_Transmit(uart_handler,NO_TIME,15,10);		
	}else{
		HAL_UART_Transmit(uart_handler,SUCCESS,8,10);
	}
	
	uint8_t RUN_SEARCH[11] = "Run Start: ";
	HAL_UART_Transmit(uart_handler,RUN_SEARCH,11,10);		
//	
//	if(FRAM_IO_Search_GetNextItem(i2c_handler, &TestData, uart_handler) != FRAM_SUCCESS){
//		HAL_UART_Transmit(uart_handler,ERROR,5,10);
//	}
//	else{
//		HAL_UART_Transmit(uart_handler,SUCCESS,8,10);
//	}
//	
	//uint8_t data[16] = {0};
//	if(HAL_I2C_Mem_Read(i2c_handler, 0xA0, 0x0, 0x08, data, 16, 10) != HAL_OK){
//		sprintf(StrOut,"Header Read Error\n");
//		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);			
//	}
//	
//	for(int i=0; i<16; i++){
//		sprintf(StrOut,"Header %d: %d\r\n",i,data[i]);
//		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
//	}
	
	//Demo first 50 Energy Points
	HAL_UART_Transmit(uart_handler,NEWLINE,1,10);
	
	uint8_t END_SEARCH[14] = "Search Over\n";
	FRAM_Return SearchOut;
	int i = 0;
	while(1){
		SearchOut = FRAM_IO_Search_GetNextItem(i2c_handler, &TestData, uart_handler);
		if(SearchOut == FRAM_ERROR){
			HAL_UART_Transmit(uart_handler,ERROR,5,10);
		}
		if(SearchOut ==FRAM_SEARCH_ENDED){
			HAL_UART_Transmit(uart_handler,END_SEARCH,12,10);
			break;
		}
		sprintf(StrOut,"%4d: Time: %5d, Energy: %2d\n",i , TestData.Time, TestData.Energy);
		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
		i++;
		
		if(i>1000){
			break;
		}
	}

	
	
		for(int i=0; i<1000; i=i+2){
		GenerateTestData(&energy, &time, uart_handler);
		if(FRAM_Data_Builder(&TestData, time, energy) != FRAM_SUCCESS){
			HAL_UART_Transmit(uart_handler,ERROR,6,10);
			
		}else{
//			sprintf(StrOut,"Data Pack: %d, %d\n",ScienceTestData[i], (uint8_t)ScienceTestData[i+1]);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
			if(FRAM_IO_Write(i2c_handler, &TestData, uart_handler) != FRAM_SUCCESS){
				HAL_UART_Transmit(uart_handler,ERROR,6,10);
			}
		}
	}	
		
	TestStart = 20000;
	TestEnd = 25000;
	
	SearchReturn = FRAM_IO_Search_Start(i2c_handler, TestStart, TestEnd, uart_handler);
	if(SearchReturn == FRAM_ERROR){
		HAL_UART_Transmit(uart_handler,ERROR,5,10);
	}
	else if(SearchReturn == FRAM_TIME_NOT_FOUND){
		uint8_t NO_TIME[20] = "Time not found\n";
		HAL_UART_Transmit(uart_handler,NO_TIME,15,10);		
	}else{
		HAL_UART_Transmit(uart_handler,SUCCESS,8,10);
	}
	
	i = 0;
	while(1){
		SearchOut = FRAM_IO_Search_GetNextItem(i2c_handler, &TestData, uart_handler);
		if(SearchOut == FRAM_ERROR){
			HAL_UART_Transmit(uart_handler,ERROR,5,10);
		}
		if(SearchOut ==FRAM_SEARCH_ENDED){
			HAL_UART_Transmit(uart_handler,END_SEARCH,12,10);
			break;
		}
		sprintf(StrOut,"%4d: Time: %5d, Energy: %2d\n",i , TestData.Time, TestData.Energy);
		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
		i++;
		
		if(i>1000){
			break;
		}
	}
	
	
	for(int i=0; i<1000; i=i+2){
		GenerateTestData(&energy, &time, uart_handler);
		if(FRAM_Data_Builder(&TestData, time, energy) != FRAM_SUCCESS){
			HAL_UART_Transmit(uart_handler,ERROR,6,10);
			
		}else{
//			sprintf(StrOut,"Data Pack: %d, %d\n",ScienceTestData[i], (uint8_t)ScienceTestData[i+1]);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
			if(FRAM_IO_Write(i2c_handler, &TestData, uart_handler) != FRAM_SUCCESS){
				HAL_UART_Transmit(uart_handler,ERROR,6,10);
			}
		}
	}	
		
	TestStart = 50000;
	TestEnd = 100000;
	
	SearchReturn = FRAM_IO_Search_Start(i2c_handler, TestStart, TestEnd, uart_handler);
	if(SearchReturn == FRAM_ERROR){
		HAL_UART_Transmit(uart_handler,ERROR,5,10);
	}
	else if(SearchReturn == FRAM_TIME_NOT_FOUND){
		uint8_t NO_TIME[20] = "Time not found\n";
		HAL_UART_Transmit(uart_handler,NO_TIME,15,10);		
	}else{
		HAL_UART_Transmit(uart_handler,SUCCESS,8,10);
	}
	
	i = 0;
	while(1){
		SearchOut = FRAM_IO_Search_GetNextItem(i2c_handler, &TestData, uart_handler);
		if(SearchOut == FRAM_ERROR){
			HAL_UART_Transmit(uart_handler,ERROR,5,10);
		}
		if(SearchOut ==FRAM_SEARCH_ENDED){
			HAL_UART_Transmit(uart_handler,END_SEARCH,12,10);
			break;
		}
		sprintf(StrOut,"%4d: Time: %5d, Energy: %2d\n",i , TestData.Time, TestData.Energy);
		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
		i++;
		
		if(i>1000){
			break;
		}
	}
	
			for(int i=0; i<1000; i=i+2){
		GenerateTestData(&energy, &time, uart_handler);
		if(FRAM_Data_Builder(&TestData, time, energy) != FRAM_SUCCESS){
			HAL_UART_Transmit(uart_handler,ERROR,6,10);
			
		}else{
//			sprintf(StrOut,"Data Pack: %d, %d\n",ScienceTestData[i], (uint8_t)ScienceTestData[i+1]);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
			if(FRAM_IO_Write(i2c_handler, &TestData, uart_handler) != FRAM_SUCCESS){
				HAL_UART_Transmit(uart_handler,ERROR,6,10);
			}
		}
	}	
		
	TestStart = 100000;
	TestEnd = 200000;
	
	SearchReturn = FRAM_IO_Search_Start(i2c_handler, TestStart, TestEnd, uart_handler);
	if(SearchReturn == FRAM_ERROR){
		HAL_UART_Transmit(uart_handler,ERROR,5,10);
	}
	else if(SearchReturn == FRAM_TIME_NOT_FOUND){
		uint8_t NO_TIME[20] = "Time not found\n";
		HAL_UART_Transmit(uart_handler,NO_TIME,15,10);		
	}else{
		HAL_UART_Transmit(uart_handler,SUCCESS,8,10);
	}
	
	i = 0;
	while(1){
		SearchOut = FRAM_IO_Search_GetNextItem(i2c_handler, &TestData, uart_handler);
		if(SearchOut == FRAM_ERROR){
			HAL_UART_Transmit(uart_handler,ERROR,5,10);
		}
		if(SearchOut ==FRAM_SEARCH_ENDED){
			HAL_UART_Transmit(uart_handler,END_SEARCH,12,10);
			break;
		}
		sprintf(StrOut,"%4d: Time: %5d, Energy: %2d\n",i , TestData.Time, TestData.Energy);
		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
		i++;
		
		if(i>1000){
			break;
		}
	}
	
			for(int i=0; i<1000; i=i+2){
		GenerateTestData(&energy, &time, uart_handler);
		if(FRAM_Data_Builder(&TestData, time, energy) != FRAM_SUCCESS){
			HAL_UART_Transmit(uart_handler,ERROR,6,10);
			
		}else{
//			sprintf(StrOut,"Data Pack: %d, %d\n",ScienceTestData[i], (uint8_t)ScienceTestData[i+1]);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
			if(FRAM_IO_Write(i2c_handler, &TestData, uart_handler) != FRAM_SUCCESS){
				HAL_UART_Transmit(uart_handler,ERROR,6,10);
			}
		}
	}	
		
	TestStart = 20000;
	TestEnd = 25000;
	
	SearchReturn = FRAM_IO_Search_Start(i2c_handler, TestStart, TestEnd, uart_handler);
	if(SearchReturn == FRAM_ERROR){
		HAL_UART_Transmit(uart_handler,ERROR,5,10);
	}
	else if(SearchReturn == FRAM_TIME_NOT_FOUND){
		uint8_t NO_TIME[20] = "Time not found\n";
		HAL_UART_Transmit(uart_handler,NO_TIME,15,10);		
	}else{
		HAL_UART_Transmit(uart_handler,SUCCESS,8,10);
	}
	
	i = 0;
	while(1){
		SearchOut = FRAM_IO_Search_GetNextItem(i2c_handler, &TestData, uart_handler);
		if(SearchOut == FRAM_ERROR){
			HAL_UART_Transmit(uart_handler,ERROR,5,10);
		}
		if(SearchOut ==FRAM_SEARCH_ENDED){
			HAL_UART_Transmit(uart_handler,END_SEARCH,12,10);
			break;
		}
		sprintf(StrOut,"%4d: Time: %5d, Energy: %2d\n",i , TestData.Time, TestData.Energy);
		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
		i++;
		
		if(i>1000){
			break;
		}
	}
	
	TestStart = 300000;
	TestEnd = 600000;
	
	SearchReturn = FRAM_IO_Search_Start(i2c_handler, TestStart, TestEnd, uart_handler);
	if(SearchReturn == FRAM_ERROR){
		HAL_UART_Transmit(uart_handler,ERROR,5,10);
	}
	else if(SearchReturn == FRAM_TIME_NOT_FOUND){
		uint8_t NO_TIME[20] = "Time not found\n";
		HAL_UART_Transmit(uart_handler,NO_TIME,15,10);		
	}else{
		i = 0;
		while(1){
			SearchOut = FRAM_IO_Search_GetNextItem(i2c_handler, &TestData, uart_handler);
			if(SearchOut == FRAM_ERROR){
				HAL_UART_Transmit(uart_handler,ERROR,5,10);
			}
			if(SearchOut ==FRAM_SEARCH_ENDED){
				HAL_UART_Transmit(uart_handler,END_SEARCH,12,10);
				break;
			}
			sprintf(StrOut,"%4d: Time: %5d, Energy: %2d\n",i , TestData.Time, TestData.Energy);
			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
			i++;
			
			if(i>1000){
				break;
			}
		}
		HAL_UART_Transmit(uart_handler,SUCCESS,8,10);
	}
	
	
	
	//Tests Complete
	uint8_t Done[6] = "Done: ";
	HAL_UART_Transmit(uart_handler,NEWLINE,1,10);
	HAL_UART_Transmit(uart_handler,Done,6,10);
	if(Successes == TotalTests){
		HAL_UART_Transmit(uart_handler,SUCCESS,8,10);
	}else{
		HAL_UART_Transmit(uart_handler,ERROR,9,10);
	}
	
//	uint8_t energy;
//	uint32_t time;
//	for(int i = 0; i<100; i++){
//		GenerateTestData(&energy, &time);
//		sprintf(StrOut,"T: %d, E: %d\n",time, energy);
//		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);
//	}
//	uint8_t dataOut;
//	for(int i=16; i<200; i++){
//		HAL_I2C_Mem_Read(i2c_handler, 0xA0, i, 0x08, &dataOut, 1, 10);
//		sprintf(StrOut,"%#2x ", dataOut);
//		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);		
//		if (((i-16) % 5) == 0) {
//			HAL_UART_Transmit(uart_handler,NEWLINE,1,10);
//			sprintf(StrOut,"%2d (%3d): ", (i-16) / 5, i);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);		
//		}
//	}
	
//	for(int i = 0; i < 2;  i++){
//		if(FRAM_Read_Header(i2c_handler, i, &currentWriteHead, &currentWriteHeadAddress, &startTime, &endTime) != FRAM_SUCCESS){
//			sprintf(StrOut,"FRAM %d Read Error, possibly not active\r\n", i);
//			HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);			
//		}
//		sprintf(StrOut,"!!Header %d: WrtHd: %d, WrtHdAd: %d, Str: %d, End: %d\n", i, currentWriteHead, currentWriteHeadAddress, startTime, endTime);
//		HAL_UART_Transmit(uart_handler,StrOut,strlen(StrOut),10);					
//	}
	
	while(1);
}




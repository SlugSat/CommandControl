/*
FRAM_Lib.h

This code was written for the Nucleo L152RE though it should work for other boards

*/



/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"
	
/* Defines -------------------------------------------------------------------- */
	
//	uint8_t FRAM_HAL_OK[20] = "HAL_OK\r\n";
//	uint8_t FRAM_HAL_ERROR[20] = "HAL_ERROR\r\n";
//	uint8_t FRAM_HAL_BUSY[20] = "HAL_BUSY\r\n";
//	uint8_t FRAM_HAL_TIMEOUT[20] = "HAL_TIMEOUT\r\n";
//	uint8_t FRAM_HAL_SUCCESS[20] = "SUCCESS\r\n";
//	uint8_t FRAM_HAL_FAIL[20] = "FAILURE\r\n";

/* Typedefs ------------------------------------------------------------------- */

#ifndef FRAM_LIB_H
#define FRAM_LIB_H

typedef enum{
	FRAM_NULL = 0,
	FRAM_SUCCESS,
	FRAM_ERROR,
	FRAM_NULL_ERROR,
	FRAM_TIME_NOT_FOUND,
	FRAM_SEARCH_ENDED
}FRAM_Return;


//Detector Number _ Energy Level
typedef enum{
	POWER_NULL = 0,
	
	D1_E1,
	D1_E2,
	D1_E3,
	D1_E4,
	
	D2_E1,
	D2_E2,
	D2_E3,
	D2_E4,
	
	D3_E1,
	D3_E2,
	D3_E3,
	D3_E4,
	
	D4_E1,
	D4_E2,
	D4_E3,
	D4_E4
}FRAM_Science_Levels;



struct ScienceDataPackage{
	uint32_t Time;
	FRAM_Science_Levels Energy;
};

#endif

/* Primatives ----------------------------------------------------------------- */

/**
 * @file
 * @Function FRAM_IO_Init()
 * @param *i2c_handler Pointer to the i2c interface FRAM are attached to
 * @return FRAM_Return Success or Errors
 * @brief  Initilizes the FRAM Modules, does not wipe memory 
 * @note  None
 * @author Zee Moffatt */
FRAM_Return FRAM_IO_Init(I2C_HandleTypeDef *i2c_handler);

/**
 * @file
 * @Function FRAM_Write_Headers()
 * @param *i2c_handler Pointer to the i2c interface FRAM are attached to
 * @return FRAM_Return Success or Errors
 * @brief Initilizes the headers to defaults. Can be re run to reset headers
 * @note  None
 * @author Zee Moffatt */
FRAM_Return FRAM_Write_Headers(I2C_HandleTypeDef *i2c_handler);

/**
 * @file
 * @Function FRAM_Read_Header()
 * @param *i2c_handler Pointer to the i2c interface FRAM are attached to
 * @param memNum FRAM number
 * @param *currentWriteHead Pointer to variable that is filled with value of current FRAM containing write head
 * @param *currentWriteHeadAddress Pointer to variable that is filled with value of address of write head inside given FRAM
 * @param *startTime Pointer to variable that is filled with value of headers start time value
 * @param *endTime Pointer to variable that is filled with value of headers end time value
 * @return FRAM_Return Success or Errors
 * @brief  Reads back the segment of the specified header
 * @note  Not totally sure why this would be needed but its there. Could be used for voting I guess
 * @author Zee Moffatt */
FRAM_Return FRAM_Read_Header(I2C_HandleTypeDef *i2c_handler, uint8_t memNum, uint8_t *currentWriteHead, uint32_t *currentWriteHeadAddress, uint32_t *startTime, uint32_t *endTime);
/**
 * @file
 * @Function FRAM_IO_Write()
 * @param *i2c_handler Pointer to the i2c interface FRAM are attached to
 * @param *scienceData Pointer to a set science data package  
 * @return FRAM_Return Success or Errors
 * @brief  Writes the given science payload data to the next spot in the FRAM
 * @note  None
 * @author Zee Moffatt */
FRAM_Return FRAM_IO_Write(I2C_HandleTypeDef *i2c_handler, struct ScienceDataPackage *scienceData, UART_HandleTypeDef *uart_handler);

/**
 * @file
 * @Function FRAM_IO_Write_To_Address()
 * @param *i2c_handler Pointer to the i2c interface FRAM are attached to
 * @param *scienceData Pointer to a set science data package  
 * @param framNum 
 * @param address
 * @return FRAM_Return Success or Errors
 * @brief  Writes the science payload data to the defined address
 * @note  Has NO protection against overwritting headers and/or other data
 * @author Zee Moffatt */
FRAM_Return FRAM_IO_Write_To_Address(I2C_HandleTypeDef *i2c_handler, struct ScienceDataPackage *scienceData, uint8_t framNum, uint8_t address);

/**
 * @file
 * @Function FRAM_IO_Read()
 * @param *i2c_handler Pointer to the i2c interface FRAM are attached to
 * @param 
 * @return FRAM_Return Success or Errors
 * @brief  
 * @note  None.
 * @author Zee Moffatt */
//FRAM_Return FRAM_IO_Read();

/**
 * @file
 * @Function FRAM_IO_Search_Start()
 * @param *i2c_handler Pointer to the i2c interface FRAM are attached to
 * @param StartTime
 * @param EndTime
 * @return FRAM_Return Success or Errors
 * @brief  
 * @note  None.
 * @author Zee Moffatt */
FRAM_Return FRAM_IO_Search_Start(I2C_HandleTypeDef *i2c_handler, uint32_t StartTime, uint32_t EndTime, UART_HandleTypeDef *uart_handler);

/**
 * @file
 * @Function FRAM_IO_Search_GetNextItem()
 * @param *i2c_handler Pointer to the i2c interface FRAM are attached to
 * @param *DataPack
 * @return FRAM_Return Success or Errors
 * @brief  
 * @note  None.
 * @author Zee Moffatt */
FRAM_Return FRAM_IO_Search_GetNextItem(I2C_HandleTypeDef *i2c_handler, struct ScienceDataPackage *DataPack, UART_HandleTypeDef *uart_handler);

/**
 * @file
 * @Function FRAM_IO_Search_EndSearch()
 * @return FRAM_Return Success or Errors
 * @brief  
 * @note  None.
 * @author Zee Moffatt */
FRAM_Return FRAM_IO_Search_EndSearch();

/**
 * @file
 * @Function FRAM_IO_Clear()
 * @param *i2c_handler Pointer to the i2c interface FRAM are attached to
 * @param 
 * @return FRAM_Return Success or Errors
 * @brief  
 * @note  None.
 * @author Zee Moffatt */
//FRAM_Return FRAM_IO_Clear();

/**
 * @file
 * @Function FRAM_Reset()
 * @param *i2c_handler Pointer to the i2c interface FRAM are attached to
 * @param 
 * @return FRAM_Return Success or Errors
 * @brief  
 * @note  None.
 * @author Zee Moffatt */
//FRAM_Return FRAM_Reset();

/**
 * @file
 * @Function FRAM_Data_Builder()
 * @param *DataPack
 * @param Time
 * @param Energy
 * @return FRAM_Return Success or Errors
 * @brief  Fills in the ScienceDataPackage struct with the provided time and energy data
 * @note  None.
 * @author Zee Moffatt */
FRAM_Return FRAM_Data_Builder(struct ScienceDataPackage *DataPack, uint32_t Time, FRAM_Science_Levels Energy);

//WARNING
//DO NOT USE, ERRORS
/**
 * @file
 * @Function Retrieve_Time()
 * @param *i2c_handler Pointer to the i2c interface FRAM are attached to
 * @param FRAM_Num
 * @param address
 * @return FRAM_Return Success or Errors
 * @brief  Retrieves and rebuilds the given time data from the given address. Assumes the caller knows if the given address is correct
 * @note  Mostly meant to be a helper function
 * @author Zee Moffatt */
//FRAM_Return Retrieve_Time(I2C_HandleTypeDef *i2c_handler, uint8_t FRAM_Num, uint16_t address, uint32_t *output);

/**
 * @file
 * @Function Store_Time()
 * @param *i2c_handler Pointer to the i2c interface FRAM are attached to
 * @param FRAM_Num Which FRAM to store the data in
 * @param address	Memory address to start storage at
 * @param time Time data to store
 * @return FRAM_Return Success or Errors
 * @brief  Stores the given time at the given address. Assumes the caller has determined if the given location is correct.
 * @note  Mostly meant to be a helper function
 * @author Zee Moffatt */
FRAM_Return Store_Time(I2C_HandleTypeDef *i2c_handler, uint8_t FRAM_Num, uint16_t address, uint32_t time);

//FRAM_Return Retrieve_Full_Package(I2C_HandleTypeDef *i2c_handler, uint8_t FRAM_Num, uint16_t address, struct ScienceDataPackage *DataPack);

FRAM_Return Wipe_Memory(I2C_HandleTypeDef *i2c_handler, uint8_t FRAM_Num, uint8_t Mode);

#ifndef SPI_FRAM_LIB
#define SPI_FRAM_LIB
//This is the library that lets you use the SPI FRAM.
//please make sure that you have designated GPIO A9 as the chip select
//you are only allowed to use macros for adresses in both functions

#include "stm32l1xx_hal.h"

// Constants
#define WRITE_CMD_LEN	(4) 
#define READ_CMD_LEN	(3) 

//opcodes
#define WRITE_OP        (2)
#define READ_OP         (3)
#define WRDI_OP         (4)
#define WREN_OP         (6)

//adresses
#define SPI_LAT_ADDR						(0)
#define SPI_LONG_ADDR						(4)
#define SPI_ALT_ADDR						(8)
#define SPI_TIME_ADDR						(100)
#define SPI_BATT_LEVEL_ADDR			(200)
#define SPI_PM_STATE_ADDR				(300)
#define SPI_LOG_DATA_TIME_ADDR	(400)
#define SPI_NUM_DATA_ADDR 			(500)
#define SPI_SOLAR_VECTOR_ADDR		(600)
#define SPI_LT_STAT_ADDR				(700)
#define SPI_LT_DAILY_USERS_ADDR (800)
#define SPI_POWER_STAT_ADDR     (900) 
#define SPI_MECH_STATE_ADDR			(1000)

void SPI_FRAM_Read( SPI_HandleTypeDef *hspi, uint16_t address, uint8_t *pRxData,uint8_t size);

void SPI_FRAM_Write( SPI_HandleTypeDef *hspi, uint16_t address, uint8_t *pTxData,uint8_t size);
#endif // SPI_FRAM_LIB

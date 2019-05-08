//This is the library that lets you use the SPI FRAM.
//please make sure that you have designated GPIO A9 as the chip select
//you are only allowed to use macros for adresses in both functions
#include "stm32l1xx_hal.h"
//opcodes
#define Write_Opcode        (2)
#define Read_Opcode         (3)
#define Wrdi_opcode         (4)
#define Wren_opcode         (6)

//adresses
#define SPI_latitude_addr            (0)
#define SPI_longitude_addr           (4)
#define SPI_altitude_addr            (8)
#define SPI_time_addr                (100)
#define SPI_battery_level_addr       (200)
#define SPI_PM_state_addr            (300)
#define SPI_I2c_datapoints_addr      (400)
#define SPI_Sun_State_addr           (500)
#define SPI_LT_status_addr           (600)
#define SPI_LT_Daily_users_addr      (700)
#define SPI_Power_status_addr        (800) 


void SPI_FRAM_Read( SPI_HandleTypeDef hspi1,uint16_t address, uint8_t *pRxData,uint8_t size);

void SPI_FRAM_Write( SPI_HandleTypeDef hspi1,uint16_t address, uint8_t *pTxData,uint8_t size);

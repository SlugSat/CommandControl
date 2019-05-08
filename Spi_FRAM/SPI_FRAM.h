//This is the library that lets you use the SPI FRAM.
//please make sure that you have designated GPIO A9 as the chip select
//you are only allowed to use macros for adresses in both functions

//opcodes
#define Write_Opcode        (2)
#define Read_Opcode         (3)
#define Wrdi_opcode         (4)
#define Wren_opcode         (6)

//adresses
#define latitude            (0)
#define longitude           (4)
#define altitude            (8)
#define time                (100)
#define battery_level       (200)
#define PM_state            (300)
#define I2c_datapoints      (400)
#define Sun_State           (500)
#define LT_status           (600)
#define LT_Daily_users      (700)
#define Power_status        (800) 


void SPI_FRAM_Read( uint16_t address, uint8_t *pRxData,uint8_t size);

void SPI_FRAM_Write( uint16_t address, uint8_t *pTxData,uint8_t size);

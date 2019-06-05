
#include "main.h"

#include "string.h"
#include "CC1200_SPI_functions.h"
#include "FRAM_Lib.h"
#include "FRAM_Tests.h"
#include "Telemetry_Packet_Protocol.h"
#include "SPI_FRAM.h"

/* Macros */
#define SUCCESS (1)
#define FAIL 		(0)


/* Define states and system function variables */
typedef enum States {Fetch, Decode, Science_Time, Science_Location} States;

/*** Function Prototypes ***/
/* Initalize the state machine */
void Sci_Tel_SM_Init(I2C_HandleTypeDef *hi2c, SPI_HandleTypeDef *hspiRadio, SPI_HandleTypeDef *hspiMem, UART_HandleTypeDef *huart);
/* Run the state machine */
void Sci_Tel_SM_Run(void);


/* Poll to transition between the different states */
uint8_t Poll_Receive_Packet(void);
uint8_t Poll_FRAM_Location(void);
uint8_t Poll_FRAM_Time(void);

/* Log fake science data */
void Log_Science_Data(void);

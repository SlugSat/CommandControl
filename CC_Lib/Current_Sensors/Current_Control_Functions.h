

#include "string.h"
#include <math.h> 
#include "main.h"

/* I2C slave address */
//Default Slave Address = 01000000
#define SLAVE_ADDRESS (0x80)

/* Configuration Registers */
#define CONFIG_REG (0x00)

/* Calibration Registers */
#define CALIB_REG (0x05)

/* Output registers */
#define Shunt_Volt_REG (0x01)
#define Bus_Volt_REG (0x02)
#define Power_REG (0x03)
#define Current_REG (0x04)
	
/* Values for init function */	
#define CONFIG_REG_value (0x4F3F)
#define CALIB_REG_value (0x0521)	

//define error and success in an enum
enum {
    FAIL = 0,
    PASS
};

/* Function Prototypes */															
void CurrentSensorRead(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *recv);
void CurrentSensorWrite(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *send);
int  CurrentSensorInit(I2C_HandleTypeDef *hi2c);													

//The output will be in VOLTS													
float Get_Shunt_Voltage(I2C_HandleTypeDef *hi2c); //The output will be in VOLTS
float Get_Bus_Voltage(I2C_HandleTypeDef *hi2c); //The output will be in VOLTS
float Get_power(I2C_HandleTypeDef *hi2c); //The output will be in MILLI_WATTS
float Get_Current(I2C_HandleTypeDef *hi2c); //The output will be in MILLI_AMPS



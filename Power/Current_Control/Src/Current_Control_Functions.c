#include "Current_Control_Functions.h"

/* Multiplier constants used to multiply register value in order to get final result */
float Shunt_Voltage_multiplier = (0.000001)*2.5; //since the shunt voltage lsb is 2.5 micro volts
float Bus_Voltage_multiplier = (0.001)*1.25; //since the bus voltage lsb is 1.25 milli volts
float Current_multiplier_mA = (0.001)*100; //since current lsb is 100 micro amps
float Power_multiplier_mW = 2.5; //since power lsb is 2.5 milli amps
void readReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *recv)
{
	// Values are ent as bytes so make a buffer to get the bytes and then reassemble
	uint8_t recv_buff[2] = {0};
	
	// I2C Read
	HAL_I2C_Master_Transmit(hi2c, SLAVE_ADDRESS, &reg, 1, 10);
	HAL_I2C_Master_Receive(hi2c, SLAVE_ADDRESS, recv_buff, 2, 10);
	
	// Reassemble the bytes into the unsigned integer output
	*recv |= recv_buff[1];
	*recv <<= 8;
	*recv |= recv_buff[0];
}

void writeReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *send)
{
	// Write the value to the register
	uint8_t send_buff[3] = {reg, (uint8_t)(*send & 0x00FF), (uint8_t)((*send >> 8) & 0x00FF)};
	HAL_I2C_Master_Transmit(hi2c, SLAVE_ADDRESS, send_buff, 3, 10);
}

int init(I2C_HandleTypeDef *hi2c)
{	
	//writing the right value to the configration register
	uint16_t send = CONFIG_REG_value;
	writeReg(hi2c, CONFIG_REG, &send);
	
	//writing the right value to the calibration register
	send = CALIB_REG_value;
	writeReg(hi2c, CALIB_REG, &send);
	
	//checking if the registors are holding the right values
	uint16_t config_value = 0;
	uint16_t calib_value = 0;
	readReg(hi2c, CONFIG_REG, &config_value);
	readReg(hi2c, CALIB_REG, &calib_value);
	
	//returning success and failure based on the registor values
	if ((config_value == CONFIG_REG_value)&&(calib_value == CALIB_REG_value))
	{
		return PASS;
	}
	else
	{
		return FAIL;
	}
		
}

float Get_Shunt_Voltage(I2C_HandleTypeDef *hi2c)
{
	uint16_t recv = 0;
	readReg(hi2c, Shunt_Volt_REG, &recv);//read the register
	
	int val = TwosComplementToDecimal( recv) ;//do 2s complement
	float output = val * Shunt_Voltage_multiplier; // use the multiplier to find actual value
	return output;
}

float Get_Bus_Voltage(I2C_HandleTypeDef *hi2c)
{
	uint16_t recv = 0;
	readReg(hi2c, Bus_Volt_REG, &recv);//read the register
	float output = recv * Bus_Voltage_multiplier; // use the multiplier to find actual value
	return output;
}

float Get_power(I2C_HandleTypeDef *hi2c)
{
	uint16_t recv = 0;
	readReg(hi2c, Power_REG, &recv);//read the register
	float output = recv * Power_multiplier_mW; // use the multiplier to find actual value
	return output;
}

float Get_Current(I2C_HandleTypeDef *hi2c)
{
	uint16_t recv = 0;
	readReg(hi2c, Current_REG, &recv);//read the register
	float output = recv * Current_multiplier_mA; // use the multiplier to find actual value
	return output;	
}
int TwosComplementToDecimal(uint16_t binary) 
{
    // If a positive value, return it
    if ((binary & 0x8000) == (0x0000))
    {
        return binary;
    }

    // Otherwise perform the 2's complement math on the value
    return (int)((~(binary - 1)) * -1);
	 
}


#include "Current_Control_Functions.h"

/* Multiplier constants used to multiply register value in order to get final result */
float Shunt_Voltage_multiplier = (0.000001)*2.5; //since the shunt voltage lsb is 2.5 micro volts
float Bus_Voltage_multiplier = (0.001)*1.25; //since the bus voltage lsb is 1.25 milli volts
float Current_multiplier_mA = (0.001)*100; //since current lsb is 100 micro amps
float Power_multiplier_mW = 2.5; //since power lsb is 2.5 milli amps

void CurrentControlRead(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *recv)
{
	// Values are ent as bytes so make a buffer to get the bytes and then reassemble
	uint8_t recv_buff[2] = {0};
	// I2C Read
	HAL_I2C_Master_Transmit(hi2c, SLAVE_ADDRESS, &reg, 1, 10);
	HAL_I2C_Master_Receive(hi2c, SLAVE_ADDRESS, recv_buff, 2, 10);
	
	// Reassemble the bytes into the unsigned integer output
	*recv |= recv_buff[0];
	*recv <<= 8;
	*recv |= recv_buff[1];
}

void CurrentSensorWrite(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *send)
{
	// Write the value to the register
	//uint8_t send_buff[3] = {(uint8_t)(*send & 0x00FF),(uint8_t)((*send >> 8) & 0x00FF),reg };
	uint8_t send_buff[3] = {reg,(uint8_t)((*send >> 8) & 0x00FF),(uint8_t)(*send & 0x00FF) };
	HAL_I2C_Master_Transmit(hi2c, SLAVE_ADDRESS, send_buff, 3, 10);
}

int CurrentSensorInit(I2C_HandleTypeDef *hi2c)
{	
	HAL_Delay(5);
	//writing the right value to the configration register
	uint16_t send = CONFIG_REG_value;
	CurrentSensorWrite(hi2c, CONFIG_REG, &send);
	HAL_Delay(5);
	//writing the right value to the calibration register
	send = CALIB_REG_value;
	CurrentSensorWrite(hi2c, CALIB_REG, &send);
	
	HAL_Delay(10);
	
	//checking if the registors are holding the right values
	uint16_t config_value = 0;
	uint16_t calib_value = 0;
	CurrentControlRead(hi2c, CONFIG_REG, &config_value);
	CurrentControlRead(hi2c, CALIB_REG, &calib_value);
	
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
	CurrentControlRead(hi2c, Shunt_Volt_REG, &recv);//read the register
	
	int16_t val = ((int16_t)recv) ;//do 2s complement
	float output = val * Shunt_Voltage_multiplier; // use the multiplier to find actual value
	return output;
}

float Get_Bus_Voltage(I2C_HandleTypeDef *hi2c)
{
	uint16_t recv = 0;
	CurrentControlRead(hi2c, Bus_Volt_REG, &recv);//read the register
	float output = recv * Bus_Voltage_multiplier; // use the multiplier to find actual value
	return output;
}

float Get_power(I2C_HandleTypeDef *hi2c)
{
	uint16_t recv = 0;
	CurrentControlRead(hi2c, Power_REG, &recv);//read the register
	float output = recv * Power_multiplier_mW; // use the multiplier to find actual value
	return output;
}

float Get_Current(I2C_HandleTypeDef *hi2c)
{
	uint16_t recv = 0;
	CurrentControlRead(hi2c, Current_REG, &recv);//read the register
	float output = recv * Current_multiplier_mA; // use the multiplier to find actual value
	return output;	
}



#include "BNO055_IMU.h"

uint8_t built_in_self_test(I2C_HandleTypeDef *hi2c)
{
	IMU_Reg_t reg = OPR_MODE_ADDR;
	IMU_Op_Mode_t mode = OPERATION_MODE_CONFIG;
	
	// set mode to CONFIG_MODE
	write_byte(hi2c, reg, mode);
	
	
	// set self test bit in system trigger register
	reg = SYS_TRIGGER_ADDR;
	write_byte(hi2c, reg, 0x01);
	
	// read from self test register to get result
	reg = SELFTEST_RESULT_ADDR;
	uint8_t readTest =  read_byte(hi2c, &reg);
	
	// check if test worked
	if (readTest == 0x0F) {
		return TRUE;
	}
	return FALSE;	
}

// set mode
void set_mode(I2C_HandleTypeDef *hi2c, IMU_Op_Mode_t op_mode)
{
	IMU_Reg_t reg = OPR_MODE_ADDR;
		
	// set mode to CONFIG_MODE
	write_byte(hi2c, reg, op_mode);
	
	return;
}

// Get Raw Sensor Data
// get magnetometer data (microteslas)
void get_mag_data(I2C_HandleTypeDef *hi2c, float *data)
{
	IMU_Reg_t reg = MAG_DATA_X_LSB_ADDR;
	
	uint8_t rx_data[6] = {0};
	int16_t raw_data[3] = {0};
	
	read_bytes(hi2c, reg, rx_data);
	
	raw_data[0] = ((int16_t)rx_data[1] << 8) | (int16_t)rx_data[0];
	raw_data[1] = ((int16_t)rx_data[3] << 8) | (int16_t)rx_data[2];
	raw_data[2] = ((int16_t)rx_data[5] << 8) | (int16_t)rx_data[4];
	
	for (int i = 0; i < 3; i++)
	{
		data[i] = (float)raw_data[i]/16.0;
	}

	return;
}

// get gyroscope data
void get_gyr_data(I2C_HandleTypeDef *hi2c, float *data)
{
	IMU_Reg_t reg = GYRO_DATA_X_LSB_ADDR;
	
	uint8_t rx_data[6] = {0};
	int16_t raw_data[3] = {0};
	
	read_bytes(hi2c, reg, rx_data);
	
	raw_data[0] = ((int16_t)rx_data[1] << 8) | (int16_t)rx_data[0];
	raw_data[1] = ((int16_t)rx_data[3] << 8) | (int16_t)rx_data[2];
	raw_data[2] = ((int16_t)rx_data[5] << 8) | (int16_t)rx_data[4];
	
	for (int i = 0; i < 3; i++)
	{
		data[i] = (float)raw_data[i]/16.0;
	}

	return;
}

// get accelerometer data
void get_acc_data(I2C_HandleTypeDef *hi2c, float *data)
{
	IMU_Reg_t reg = ACCEL_DATA_X_LSB_ADDR;
	
	uint8_t rx_data[6] = {0};
	int16_t raw_data[3] = {0};
	
	read_bytes(hi2c, reg, rx_data);
	
	raw_data[0] = ((int16_t)rx_data[1] << 8) | (int16_t)rx_data[0];
	raw_data[1] = ((int16_t)rx_data[3] << 8) | (int16_t)rx_data[2];
	raw_data[2] = ((int16_t)rx_data[5] << 8) | (int16_t)rx_data[4];
	
	for (int i = 0; i < 3; i++)
	{
		data[i] = (float)raw_data[i]/100.0;
	}

	return;
}

// I2C Communication Methods
void write_byte(I2C_HandleTypeDef *hi2c, IMU_Reg_t reg, uint8_t dat) 
{
	// tx data array with register address to write to and data to write
	uint8_t tx_data[2] = {(uint8_t)reg, dat};

	// transmit the register address and data
	HAL_I2C_Master_Transmit(hi2c, IMU_ADDRESS_ALT, tx_data, sizeof(tx_data), 10);	HAL_Delay(30);
	return;
}

//=================================================================
//													READ BYTE
//-----------------------------------------------------------------
// 				reads a single byte from the specified register
//=================================================================
uint8_t read_byte(I2C_HandleTypeDef *hi2c, IMU_Reg_t *reg)
{
	// transmit register address to read from
	uint8_t rx_data = 0;
	HAL_I2C_Master_Transmit(hi2c, IMU_ADDRESS_ALT, (uint8_t *) reg, sizeof(uint8_t), 10);
		
	// read data from register
	HAL_I2C_Master_Receive(hi2c, IMU_ADDRESS_ALT, &rx_data, sizeof(rx_data), 10);
	
	return rx_data;
}
//=================================================================


void read_bytes(I2C_HandleTypeDef *hi2c, IMU_Reg_t reg, uint8_t *rx_data)
{
	// transmit register address to read from
	HAL_I2C_Master_Transmit(hi2c, IMU_ADDRESS_ALT, (uint8_t*)&reg, sizeof(uint8_t), 10);
	
	// read data from registers
	HAL_I2C_Master_Receive(hi2c, IMU_ADDRESS_ALT, rx_data, 6, 10);
	HAL_Delay(1);
	return;
}

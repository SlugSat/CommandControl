#include "Fuel_Gauge_Functions.h"

void readReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *recv)
{
	// Values are ent as bytes so make a buffer to get the bytes and then reassemble
	uint8_t recv_buff[2] = {0};
	
	// I2C Read
	HAL_I2C_Master_Transmit(hi2c, FG_SLAVE_ADDR, &reg, 1, 10);
	HAL_I2C_Master_Receive(hi2c, FG_SLAVE_ADDR, recv_buff, 2, 10);
	
	// Reassemble the bytes into the unsigned integer output
	*recv |= recv_buff[1];
	*recv <<= 8;
	*recv |= recv_buff[0];
}

void writeReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *send)
{
	// Write the value to the register
	uint8_t send_buff[3] = {reg, (uint8_t)(*send & 0x00FF), (uint8_t)((*send >> 8) & 0x00FF)};
	HAL_I2C_Master_Transmit(hi2c, FG_SLAVE_ADDR, send_buff, 3, 10);
}

void init(I2C_HandleTypeDef *hi2c, fg_config_t conf)
{
	/**** Check chip status ****/
	uint16_t status = 0;
	readReg(hi2c, STAT_REG, &status);
	
	// if POR (Power On Reset) bit is set no need to re-configure
	
	if (status & POR) 
	{	
		// Wait until FSTAT.DNR bit is unset indicating completion
		// of startup operations
		do {readReg(hi2c, FSTAT_REG, &status);}
		while (status & FSTAT_DNR);
		
		/**** Initialize configuration ****/
		
		// store HiBCFG value
		uint16_t HiBCFG = 0;
		readReg(hi2c, HIBCFG_REG, &HiBCFG);
		
		// soft wake up command (step 1 of exiting hibernate mode)
		uint16_t cmd = SOFT_WAKE;
		writeReg(hi2c, SOFT_WAKEUP_REG, &cmd);
		
		// Clear HiBCFG and Soft WakeUp Registers
		// (step 2 of exiting hibernate mode)
		cmd = CLEAR;
		writeReg(hi2c, HIBCFG_REG, &cmd);
		writeReg(hi2c, SOFT_WAKEUP_REG, &cmd);
		
		// Configure OPTION 1 EZ Config
		writeReg(hi2c, DESIGN_CAP_REG, &conf.design_cap);
		writeReg(hi2c, V_EMPTY_REG, &conf.v_empty);
		writeReg(hi2c, I_CHG_REG, &conf.current_chg);
		writeReg(hi2c, CONF_REG, &conf.config1);
		writeReg(hi2c, CONF2_REG, &conf.config2);
		writeReg(hi2c, DQACC_REG, &conf.dQAcc);
		writeReg(hi2c, DPACC_REG, &conf.dPAcc);
		writeReg(hi2c, MODEL_CFG_REG, &conf.model_cfg);
		
		// Poll modelCFG refresh bit (MSb) until it is unset
		do {readReg(hi2c, MODEL_CFG_REG, &status);}
		while (status & MODEL_CFG_REFRESH);
		
		/**** Initialization is complete so we need to clear the POR bit ****/
		
		// Store the current status value
		readReg(hi2c, STAT_REG, &status);
		
		// loop until POR bit has been successfully cleared
		while (status & POR) 
		{ 
			status &= 0xFFFD;
			writeReg(hi2c, STAT_REG, &status);
			HAL_Delay(1);
			readReg(hi2c, STAT_REG, &status);
		}
	}
	
	// SUCCESS!!!!
	
}

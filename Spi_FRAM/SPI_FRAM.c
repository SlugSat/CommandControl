#include SPI_FRAM.h
//to read 

/**
  * @param  address: 16 bit address
  * @param  pRxData: pointer to reception data buffer to be
  * @param  size: size of the data you are expecting (# of bytes)
  */
void SPI_FRAM_Read( uint16_t address, uint8_t *pRxData, uint8_t size)
{
	// Chip select low
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);

	// initialize read command
	uint8_t read_command = {READ_OP, 			// read operation code
							address>>8,			// MSB of address
							(uint8_t)address}; 	// LSB of address

	// initiate read operation
	HAL_SPI_Transmit(&hspi1, read_command, READ_CMD_LEN, 10);	

	// recieve data from appropriate register
	for(int i = size - 1; i >= 0; i--)
	{
		HAL_SPI_Receive(&hspi1, &pRxData[i], 1, 10);
	}

	// Chip select high
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

}
//to write
/**
  * @param  address: 16 bit address
  * @param  pTxData: pointer to reception data buffer to be
  * @param  size: size of the data you are Transmiting (# of bytes)
  */
void SPI_FRAM_Write( uint16_t address, uint8_t *pTxData,uint8_t size)
{
	// Chip select low
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);

	// initialize write command bytes
	uint8_t write_command = {WREN_OP, 			// enable write operation
							 WRITE_OP,			// write operation code
							 address>>8,		// MSB of address
							 (uint8_t)address}; // LSB of address


	// initiate write operation
	HAL_SPI_Transmit(&hspi1, write_command, CMD_LEN, 10);

	for(int i = size - 1; i >= 0; i--)
	{
		HAL_SPI_Transmit(&hspi1, &pTxData[i], 1, 10);
	}

	// terminate write operation
	uint8_t wrdi = WRDI_OP; 
	HAL_SPI_Transmit(&hspi1, &wrdi, 1, 10);

	// Chip select high
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}

#include SPI_FRAM.h
//to read 

/**
  * @param  address: 16 bit address
  * @param  pRxData: pointer to reception data buffer to be
  * @param  size: size of the data you are expecting (# of bytes)
  */
void SPI_FRAM_Read( uint16_t address, uint8_t *pRxData,uint8_t size)
{
	// Chip select low
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	uint8_t opcode = 3;
	uint8_t addrHI = address>>8;
	uint8_t addrLO = (uint8_t) address;
	HAL_SPI_Transmit(&hspi1,&opcode, 1, 10);
	HAL_SPI_Transmit(&hspi1,&addrHI, 1, 10);
	HAL_SPI_Transmit(&hspi1,&addrLO, 1, 10);	
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
	//the wren command sets write enable latch. wel shall be set with the wren command before writting operation..
	uint8_t wren = 6;
	uint8_t wrdi = 4;//to reset write enable latch after writting
	uint8_t opcode = 2;
	uint8_t addrHI = address>>8;
	uint8_t addrLO = (uint8_t) address;
	//WREN command
	HAL_SPI_Transmit(&hspi1,&wren, 1, 10);
	HAL_SPI_Transmit(&hspi1,&opcode, 1, 10);
	HAL_SPI_Transmit(&hspi1,&addrHI, 1, 10);
	HAL_SPI_Transmit(&hspi1,&addrLO, 1, 10);	
	for(int i = size - 1; i >= 0; i--)
	{
		HAL_SPI_Transmit(&hspi1, &pTxData[i], 1, 10);
	}
	HAL_SPI_Transmit(&hspi1,&wrdi, 1, 10);
	// Chip select high
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}

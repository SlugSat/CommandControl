#include "SPI_FRAM.h"
//to read 

/**
  * @param  address: 16 bit address
  * @param  pRxData: pointer to reception data buffer to be
  * @param  size: size of the data you are expecting (# of bytes)
  */
void SPI_FRAM_Read( SPI_HandleTypeDef *hspi,uint16_t address, uint8_t *pRxData, uint8_t size, UART_HandleTypeDef *huart)
{
	// Acquire the SPI FRAM lock
	Get_Lock(hspi, huart);
	
	// Chip select low
	HAL_GPIO_WritePin(GPIOB, SPI_FRAM_CS_Pin, GPIO_PIN_RESET);

	// initialize read command
	uint8_t read_command[READ_CMD_LEN] = {READ_OP, 			// read operation code
																				address>>8,			// MSB of address
																				(uint8_t)address}; 	// LSB of address

	// initiate read operation
	HAL_SPI_Transmit(hspi, read_command, READ_CMD_LEN, 10);	

	// recieve data from appropriate register
	for(int i = size - 1; i >= 0; i--)
	{
		HAL_SPI_Receive(hspi, &pRxData[i], 1, 10);
	}

	// Chip select high
	HAL_GPIO_WritePin(GPIOB, SPI_FRAM_CS_Pin, GPIO_PIN_SET);

	// Free the lock
	Free_Lock(hspi, huart);
}
//to write
/**
  * @param  address: 16 bit address
  * @param  pTxData: pointer to reception data buffer to be
  * @param  size: size of the data you are Transmiting (# of bytes)
  */
void SPI_FRAM_Write(SPI_HandleTypeDef *hspi, uint16_t address, uint8_t *pTxData, uint8_t size, UART_HandleTypeDef *huart)
{
	// Acquire the SPI FRAM lock
	Get_Lock(hspi, huart);
	
	// Chip select low
	HAL_GPIO_WritePin(GPIOB, SPI_FRAM_CS_Pin, GPIO_PIN_RESET);
	
	// initialize write command bytes
	uint8_t write_command[WRITE_CMD_LEN] = {WREN_OP, 			// enable write operation
																					WRITE_OP,									// write operation code
																					address>>8,								// MSB of address
																					(uint8_t)address}; 				// LSB of address

	// initiate write operation
	HAL_SPI_Transmit(hspi, &write_command[0], 1, 10);

	// Chip select high
	HAL_GPIO_WritePin(GPIOB, SPI_FRAM_CS_Pin, GPIO_PIN_SET);
																					
	HAL_Delay(1);																				
	
	// Chip select low
	HAL_GPIO_WritePin(GPIOB, SPI_FRAM_CS_Pin, GPIO_PIN_RESET);
																					
	HAL_SPI_Transmit(hspi, &write_command[1], 3, 10);																					
																					
	for(int i = size - 1; i >= 0; i--)
	{
		HAL_SPI_Transmit(hspi, &pTxData[i], 1, 10);
	}

	// Chip select high
	HAL_GPIO_WritePin(GPIOB, SPI_FRAM_CS_Pin, GPIO_PIN_SET);																			
	HAL_Delay(1);																				
	// Chip select low
	HAL_GPIO_WritePin(GPIOB, SPI_FRAM_CS_Pin, GPIO_PIN_RESET);
	
	// terminate write operation
	uint8_t wrdi = WRDI_OP; 
	HAL_SPI_Transmit(hspi, &wrdi, 1, 10);

	// Chip select high
	HAL_GPIO_WritePin(GPIOB, SPI_FRAM_CS_Pin, GPIO_PIN_SET);
	
	// Free the lock
	Free_Lock(hspi, huart);
}


void Get_Lock(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart)
{
	char *msg1 = "\nWAITING FOR LOCK.....\n";
	
	do
	{
		HAL_UART_Transmit(huart, (uint8_t *)msg1, strlen(msg1), 1);
		HAL_Delay(500);
	} while (HAL_GPIO_ReadPin(GPIOA, SPI_FRAM_IN1_Pin) == GPIO_PIN_RESET || 
														HAL_GPIO_ReadPin(GPIOB, SPI_FRAM_IN2_Pin) == GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(GPIOA, SPI_FRAM_LOCK_Pin, GPIO_PIN_RESET);
	while(HAL_GPIO_ReadPin(GPIOA, SPI_FRAM_IN1_Pin) == GPIO_PIN_RESET || 
														HAL_GPIO_ReadPin(GPIOB, SPI_FRAM_IN2_Pin) == GPIO_PIN_RESET)
	{
		HAL_UART_Transmit(huart, (uint8_t *)msg1, strlen(msg1), 1);
		HAL_GPIO_WritePin(GPIOA, SPI_FRAM_LOCK_Pin, GPIO_PIN_SET);
		for(int i = 0; i < 1000; i++);
		HAL_GPIO_WritePin(GPIOA, SPI_FRAM_LOCK_Pin, GPIO_PIN_RESET);
	}
}

void Free_Lock(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart)
{
	char *msg1 = "\nFreeing lock.....\n";

	HAL_UART_Transmit(huart, (uint8_t *)msg1, strlen(msg1), 1);
	
	HAL_GPIO_WritePin(GPIOA, SPI_FRAM_LOCK_Pin, GPIO_PIN_SET);	
}

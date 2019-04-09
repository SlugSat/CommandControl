/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "string.h"
#include "CC1200_reg.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

void trxRfSpiInterfaceInit(uint8_t prescalerValue);
uint8_t trx8BitRegAccess(uint8_t accessType, uint8_t addrByte, uint8_t *pData, int len);
uint8_t ReadWriteExtendedReg (uint8_t accessType, uint16_t address, uint8_t value);
uint8_t ReadWriteCommandReg (uint8_t address);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	
	
	/* Start CC1200 SPI Test Harness
	 * 
	 * Wiring:
	 * Reset -- D7 (PA8)
	 * CSN   -- D8 (PA9)
	 * SCLK  -- D3 (PB3)
	 * MOSI  -- D4 (PB5)
	 * MISO  -- D5 (PB4)
	 */
	 
	 
//	//Set CS high
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

	//Set reset high, low, high to begin
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_Delay(10);

	
		
  // test
	uint16_t address = 0x2F02;
	uint8_t value = 0xa;
	uint8_t readValue;
	char Msg1[100] = {0};
	char Msg2[100] = {0};
	
  readValue = ReadWriteExtendedReg (CC1200_READ_BIT, address, value); // Read initial reg value
		
	snprintf((char *)Msg1, sizeof(Msg1), "\r\nTest: read from register 0x%x: 0x%x\r\n", address, readValue);
	HAL_UART_Transmit(&huart2, (uint8_t *) Msg1, sizeof(Msg1), 1);
		
  readValue = ReadWriteExtendedReg (CC1200_WRITE_BIT, address, value); // Write to register
	
	readValue = ReadWriteExtendedReg (CC1200_READ_BIT, address, value);  // Read new value
		
	snprintf((char *)Msg1, sizeof(Msg1), "\r\nTest: read from register 0x%x after writing to the register: 0x%x\r\n", address, readValue);
	HAL_UART_Transmit(&huart2, (uint8_t *) Msg1, sizeof(Msg1), 1);
	
	
	// Test command strobes
	
  memcpy(Msg1, Msg2, 100);
	snprintf((char *)Msg1, sizeof(Msg1), "\r\nMode Test Expected Bytes: transmit 0x2F, idle 0x0F, receive 0x1F\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *) Msg1, sizeof(Msg1), 1);
	
	readValue = 0;
	readValue = ReadWriteCommandReg(CC1200_SIDLE); // 
	HAL_Delay(10);
	readValue = ReadWriteCommandReg(CC1200_SNOP); // Seems to need HAL_Delay and a NOP to produce the correct status bit

	memcpy(Msg1, Msg2, 100);
	snprintf((char *)Msg1, sizeof(Msg1), "\r\nMode Test: transmit status byte: 0x%x\r\n", readValue);
	HAL_UART_Transmit(&huart2, (uint8_t *) Msg1, sizeof(Msg1), 1);
	
	readValue = 0;
	readValue = ReadWriteCommandReg(CC1200_STX);
	HAL_Delay(10);
	readValue = ReadWriteCommandReg(CC1200_SNOP); // Seems to need HAL_Delay and a NOP to produce the correct status bit

	memcpy(Msg1, Msg2, 100);
	snprintf((char *)Msg1, sizeof(Msg1), "\r\nMode Test: idle status byte: 0x%x\r\n", readValue);
	HAL_UART_Transmit(&huart2, (uint8_t *) Msg1, sizeof(Msg1), 1);
	
	readValue = 0;
	readValue = ReadWriteCommandReg(CC1200_SRX); // Recieve
	HAL_Delay(10);
	readValue = ReadWriteCommandReg(CC1200_SNOP); // Seems to need HAL_Delay and a NOP to produce the correct status bit
	memcpy(Msg1, Msg2, 100);
	snprintf((char *)Msg1, sizeof(Msg1), "\r\nMode Test: receive status byte: 0x%x\r\n", readValue);
	HAL_UART_Transmit(&huart2, (uint8_t *) Msg1, sizeof(Msg1), 1);

//	uint8_t wrData = 0;
//	readValue = trx8BitRegAccess(CC1200_WRITE_BIT, CC1200_SRX, &wrData, sizeof (wrData));
//	HAL_Delay(10);
//	readValue = trx8BitRegAccess(CC1200_WRITE_BIT, CC1200_SNOP, &wrData, sizeof (wrData));
//	HAL_Delay(10);

//	memcpy(Msg1, Msg2, 100);
//	snprintf((char *)Msg1, sizeof(Msg1), "\r\nMode Test: receive status byte: 0x%x\r\n", readValue);
//	HAL_UART_Transmit(&huart2, (uint8_t *) Msg1, sizeof(Msg1), 1);
//	
//  wrData = 0;
//	readValue = trx8BitRegAccess(CC1200_WRITE_BIT, CC1200_SIDLE, &wrData, sizeof (wrData));
//	HAL_Delay(10);
//	readValue = trx8BitRegAccess(CC1200_WRITE_BIT, CC1200_SNOP, &wrData, sizeof (wrData));
//	HAL_Delay(10);

//	memcpy(Msg1, Msg2, 100);
//	snprintf((char *)Msg1, sizeof(Msg1), "\r\nMode Test: receive status byte: 0x%x\r\n", readValue);
//	HAL_UART_Transmit(&huart2, (uint8_t *) Msg1, sizeof(Msg1), 1);
//	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA5 PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */



static void trxReadWriteBurstSingle(uint8_t addr, uint8_t *pData, int len)
{
    int i;
    uint8_t dummy;
	  uint8_t pDataRead = *pData;
    /* Communicate len number of bytes: if RX - the procedure sends 0x00 to push bytes from slave*/
    if (addr & CC1200_READ_BIT) {
        if (addr & CC1200_BURST_BIT) {
            for (i = 0; i < len; i++) {
				
				HAL_SPI_Transmit(&hspi1,0x0, 1, 10);
                while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
                HAL_SPI_Receive(&hspi1, pData, 1, 10);
                pData++;
				
            }
        } else {
			HAL_SPI_Transmit(&hspi1,0x0, 1, 10);
            while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
            HAL_SPI_Receive(&hspi1, pData, 1, 10);
        }
    } else {
        if (addr & CC1200_BURST_BIT) {
            /* Communicate len number of bytes: if TX - the procedure doesn't overwrite pData */
            for (i = 0; i < len; i++) {
				HAL_SPI_Transmit(&hspi1,pData, 1, 10);
                while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
				HAL_SPI_Receive(&hspi1, &dummy, 1, 10);
                pData++;
            }
        } else {
						HAL_SPI_Transmit(&hspi1,pData, 1, 10);
            while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
						HAL_SPI_Receive(&hspi1, &dummy, 1, 10);
        }
    }
}

uint8_t trx8BitRegAccess(uint8_t accessType, uint8_t addrByte, uint8_t *pData, int len)
{
	uint8_t addr;
	uint8_t readValue  = 0;
	addr = accessType | addrByte;
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	
	HAL_SPI_Transmit(&hspi1,&addr, 1, 10);
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY){};
	HAL_SPI_Receive(&hspi1, &readValue, 1, 10);
	trxReadWriteBurstSingle(accessType | addrByte, pData, len);
		
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

	return (readValue);
}

uint8_t ReadWriteExtendedReg (uint8_t accessType, uint16_t address, uint8_t value)
{
	// Chip select low
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	
	uint8_t addrHI = address>>8;
	uint8_t addrLO = (uint8_t) address;
	uint8_t readValue = 0;
	
	
	if(accessType) { 	// Read from address
		
		addrHI |= CC1200_READ_BIT;
		
		HAL_SPI_Transmit(&hspi1,&addrHI, 1, 10);	// Access address	
		HAL_SPI_Transmit(&hspi1,&addrLO, 1, 10);
		while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY){};
			
		HAL_SPI_Transmit(&hspi1,0x0, 1, 10); // Transmit 0 for read
		HAL_SPI_Receive(&hspi1, &readValue, 1, 10); // Read value
			
	} else { // Write to address
			
		addrHI |= CC1200_WRITE_BIT;
		
		HAL_SPI_Transmit(&hspi1,&addrHI, 1, 10);	// Access address
		HAL_SPI_Transmit(&hspi1,&addrLO, 1, 10);
		while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY){};
			
		HAL_SPI_Transmit(&hspi1,&value, 1, 10);		// Write value
	}
			
	// Chip select high	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	
	return readValue;
}

uint8_t ReadWriteCommandReg (uint8_t address)
{
	// Chip select low
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	
  uint8_t readValue  = 0;
	
	HAL_SPI_Transmit(&hspi1,&address, 1, 10);
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY){};
	HAL_SPI_Receive(&hspi1, &readValue, 1, 10);
		
	// Chip select high	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	
	return readValue;
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

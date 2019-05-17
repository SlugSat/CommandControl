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
#include "CC1200_SPI_functions.h"
#include "FRAM_Lib.h"
#include "FRAM_Tests.h"
#include "Telemetry_Packet_Protocol.h"
#include "SPI_FRAM.h"
//#include "DateConversion.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* Define states and system function variables */
typedef enum States {Fetch, Decode, Science_Time, Science_Location} States;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SUCCESS (1)
#define FAIL 		(0)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t Poll_Receive_Packet(SPI_HandleTypeDef *hspi);
uint8_t Poll_FRAM_Location(SPI_HandleTypeDef *hspi);
uint8_t Poll_FRAM_Time(SPI_HandleTypeDef *hspi);

void Log_Science_Data(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

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
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	
	// Initialize the initial state
	States state;
	state = Fetch;
	
	char msg1[100] = {0};
	char msg2[100] = {0};
	
	snprintf((char *)msg1, 100, "\n\n-----Start the test-----\n\n");
	HAL_UART_Transmit(&huart2, (uint8_t *) msg1, sizeof(msg1), 1);
	
	/* Initialization code goes here */
	//Set CS high
	HAL_GPIO_WritePin(GPIOA, SPI_CC_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, SPI_FRAM_CS_Pin, GPIO_PIN_SET);

	//Set reset high, low, high to begin
	HAL_GPIO_WritePin(GPIOA, SP_CC_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA, SP_CC_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA, SP_CC_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	
	uint8_t readValue;
	
	readValue = ReadWriteCommandReg(&hspi1, CC1200_SNOP); // Check the state of the CC1200
	memcpy(msg1, msg2, 100);
	snprintf((char *)msg1, 100, "\nState of the CC1200: 0x%02x\n", readValue);
	HAL_UART_Transmit(&huart2, (uint8_t *) msg1, sizeof(msg1), 1);
	
	// Initialize the I2C FRAM
	//Wipe_Memory(&hi2c1, 0, 1);
	//Wipe_Memory(&hi2c1, 1, 1);

	FRAM_IO_Init(&hi2c1);
	FRAM_Write_Headers(&hi2c1);
	
	
	CC1200_INIT(&hspi1);
	//ReadWriteCommandReg(&hspi1, CC1200_SFTX); // Flush RX FIFO
	ReadWriteCommandReg(&hspi1, CC1200_SFRX); // Flush RX FIFO
	ReadWriteCommandReg(&hspi1, CC1200_SRX);  // Enter receive mode
	
	do 
	{
		readValue = ReadWriteCommandReg(&hspi1, CC1200_SNOP); // Check the state of the CC1200
		
		HAL_Delay(500);
		memcpy(msg1, msg2, 100);
		snprintf((char *)msg1, 100, "\nState of the CC1200: 0x%02x\n", readValue);
		HAL_UART_Transmit(&huart2, (uint8_t *) msg1, sizeof(msg1), 1);
		
		ReadWriteCommandReg(&hspi1, CC1200_SRX);
		HAL_Delay(10);
	} while ((readValue & 0x10) != 0x10);

	uint8_t packet[FIXED_PACK_SIZE] = {0};
	state = Fetch;
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		memcpy(msg1, msg2, 100);
		snprintf((char *)msg1, 100, "\nState: 0x%02x\n", state);
		HAL_UART_Transmit(&huart2, (uint8_t *) msg1, sizeof(msg1), 1);
		switch (state)
		{
			/* Fetch a packet, location data, or science events mode */
			case (Fetch): 
				// Poll buffers and FRAM to check if an event has occurred
				if 			(Poll_Receive_Packet(&hspi1) == SUCCESS) // Poll for if a packet is received
				{
					state = Decode;
				}
				else if (Poll_FRAM_Location(&hspi2) == SUCCESS) // Poll the location of the CubeSat to check if we are over the equator
				{
					state = Science_Location;
				}
				else if (Poll_FRAM_Time(&hspi2) == SUCCESS) // Poll the the log event for a certain time
				{
					state = Science_Time;
				}
				else
				{
					state = Fetch;
				}
				HAL_Delay(1500); // Poll every 3 seconds
				break;
			/* Decode a packet and respond accordingly mode */
			case (Decode):; // Semi colon because cant declare right after case statement 
				
				// Get the packet in the RX FIFO
				for (int i = 0; i < FIXED_PACK_SIZE; i++)
				{
					packet[i] = 0;
					packet[i] = ReadWriteExtendedReg(&hspi1, CC1200_READ_BIT, CC1200_RXFIFO, 0);
				}
				memcpy(msg1, msg2, 100);
				snprintf((char *)msg1, 100, "\nPacket: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", packet[0], packet[1], packet[2], packet[3], packet[4], packet[5]);
				HAL_UART_Transmit(&huart2, (uint8_t *) msg1, sizeof(msg1), 1);
				
				// Decode the packet and take action based on the packet
				Decode_Sat_Packet(packet, &hspi1, &huart2, &hspi2, &hi2c1);
				
				state = Fetch;
				ReadWriteCommandReg(&hspi1, CC1200_SFRX); // Flush RX FIFO
				ReadWriteCommandReg(&hspi1, CC1200_SRX);
				break;
			/* Log science data based on time mode */	
			case (Science_Time): 
				
				// Generate fake data and store it to the I2C FRAM
				Log_Science_Data(&hi2c1, &huart2);
			
				state = Fetch;
				break;
			/* Log science data based on location mode */
			case (Science_Location): 
				
				// Generate fake data and store it to the I2C FRAM 
				Log_Science_Data(&hi2c1, &huart2);
			
				state = Fetch;
				break;
			/* An error occurred */
			default:
				fprintf(stderr, "\nInvalid state in the state machine reached...\nExiting\n");
		}
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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the CPU, AHB and APB busses clocks 
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
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
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  HAL_GPIO_WritePin(SPI_FRAM_CS_GPIO_Port, SPI_FRAM_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SP_CC_RESET_Pin|SPI_CC_CS_Pin|Kill_to_PModes_Int_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : SPI_FRAM_CS_Pin */
  GPIO_InitStruct.Pin = SPI_FRAM_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI_FRAM_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SP_CC_RESET_Pin SPI_CC_CS_Pin Kill_to_PModes_Int_Pin */
  GPIO_InitStruct.Pin = SP_CC_RESET_Pin|SPI_CC_CS_Pin|Kill_to_PModes_Int_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
// Poll the receive buffer to check if a packet was received
uint8_t Poll_Receive_Packet(SPI_HandleTypeDef *hspi)
{
	// Check the state of the chip in the event that an error occurred
	uint8_t readValue = ReadWriteCommandReg(hspi, CC1200_SNOP); 
	HAL_Delay(8);
	
	if ((readValue & 0x60) == 0x60) // Flush if FIFO error
	{
		ReadWriteCommandReg(hspi, CC1200_SFRX); 
		HAL_Delay(10);
	}
	
	if ((readValue & 0x10) != 0x10) // Re-enter transmit mode if there was a FIFO error
	{
		ReadWriteCommandReg(hspi, CC1200_SRX);
	}

	// Poll the receive buffer
	uint8_t rxBytes = ReadWriteExtendedReg (hspi, CC1200_READ_BIT, CC1200_NUM_RXBYTES, 0);
	
	// Debugging
	readValue = ReadWriteCommandReg(hspi, CC1200_SNOP); 
	char msg1[100] = {0};
	snprintf((char *)msg1, 100, "\n\nNumber of bytes in the receive buffer: 0x%02x    State of CC1200: 0x%02x\r\n", rxBytes, readValue);
	HAL_UART_Transmit(&huart2, (uint8_t *) msg1, sizeof(msg1), 1);
	
	
	if (rxBytes <= FIXED_PACK_SIZE)
	{
		return FAIL;
	}
	else
	{
		return SUCCESS;
	}
}

// Poll the housekeeping FRAM to check the location of the CubeSat
uint8_t Poll_FRAM_Location(SPI_HandleTypeDef *hspi)
{
	// Write code here that would access the shared SPI FRAM and get if a science event should be logged based on location
	uint8_t longitudeBytes[4] = {0};
	SPI_FRAM_Read(hspi, SPI_FRAM_LONGITUDE_ADDR, longitudeBytes, 4);
	
	float longitude = bytes_to_float(longitudeBytes);
	if (longitude < 15.0 && longitude > -15.0)
	{
		return SUCCESS;
	}
	return FAIL;
}

// Poll the housekeeping FRAM to check the if it is time to log a science event
uint8_t Poll_FRAM_Time(SPI_HandleTypeDef *hspi)
{
	// Write code here that would access the shared SPI FRAM and get if a science event should be logged based on time
	static int i = 0;
	if ((i++ % 30) == 20) 
	{
		return SUCCESS;
	}
	return FAIL;
}





void Log_Science_Data(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart)
{
	static uint32_t time;
	uint8_t energy;
	
	struct ScienceDataPackage fakeDataPoint;
	
	for (int i = 0; i < 500; i++)
	{
		// Generate test data points
		GenerateTestData(&energy, &time);	
		// Build data into a package
		FRAM_Data_Builder(&fakeDataPoint, time, energy);
		// Write the data point into the FRAM
		FRAM_IO_Write(hi2c, &fakeDataPoint, huart);
	}	
	
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

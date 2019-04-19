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
#include "UM7_reg.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UM7_READ_BIT  (0x00)
#define UM7_WRITE_BIT (0x01)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
uint8_t UM7_Write_Data(uint8_t *data, uint8_t length, uint8_t reg);
uint8_t UM7_Read_Data(uint8_t *data , uint8_t length, uint8_t reg);
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
  /* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(GPIOB, SPI1_CHIPSEL_Pin, GPIO_PIN_SET);
	
//	HAL_GPIO_WritePin(GPIOB, SPI1_CHIPSEL_Pin, GPIO_PIN_RESET);
//	
//	uint8_t output[5] = {0};
//	uint8_t reg = CREG_COM_SETTINGS;
//	//UM7_Read_Data(output, 4, CREG_COM_SETTINGS); 

//	uint8_t requestData[2] = {reg}; 
//	HAL_SPI_Transmit(&hspi1, requestData, 1, 20);
//	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY){};
//	HAL_SPI_Receive(&hspi1, &output[4], 1, 10);
//		
//	//HAL_SPI_Receive(&hspi1, output, 4, 20);
//	for (int8_t i = 3; i >= 0; i--)
//	{
//		HAL_SPI_Transmit(&hspi1, 0x00, 1, 10);
//		while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY){};
//		HAL_Delay(1);
//		HAL_SPI_Receive(&hspi1, &output[i], 1, 10);
//		
//	}
//	
//	HAL_GPIO_WritePin(GPIOB, SPI1_CHIPSEL_Pin, GPIO_PIN_SET);


	uint8_t output[5] = {0};
	uint8_t reg = CREG_COM_SETTINGS;
	UM7_Read_Data(output, 4, reg); 
	
	uint8_t newVal[4] = {0x00, 0x00, 0x00, 0x03};
	UM7_Write_Data(newVal, 4, reg);
	
	uint8_t out2[4] = {0};
	UM7_Read_Data(out2, 4, reg); 
	

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

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
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
  HAL_GPIO_WritePin(SPI1_CHIPSEL_GPIO_Port, SPI1_CHIPSEL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : SPI1_CHIPSEL_Pin */
  GPIO_InitStruct.Pin = SPI1_CHIPSEL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI1_CHIPSEL_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

// Function that will use SPI to write data to a register
uint8_t UM7_Write_Data(uint8_t *data, uint8_t length, uint8_t reg)
{
	uint8_t dummy = 0;
	HAL_GPIO_WritePin(GPIOB, SPI1_CHIPSEL_Pin, GPIO_PIN_RESET);

	// Create request for the register value
	uint8_t requestWrite[2] = {UM7_WRITE_BIT, reg};
	// Send the register that we want to read from
	HAL_SPI_Transmit(&hspi1, requestWrite, 2, 10);
	
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY){};
	HAL_SPI_Receive(&hspi1, &dummy, 1, 10);
		
	// Loop through 4 times to get each byte of the register data
	for (int8_t i = length - 1; i >= 0; i--)
	{
		HAL_SPI_Transmit(&hspi1, &data[i], 1, 10);
		//HAL_SPI_Receive(&hspi1, &dummy, 1, 10);
	}
	
	HAL_GPIO_WritePin(GPIOB, SPI1_CHIPSEL_Pin, GPIO_PIN_SET);
	return 0;
}

// Function that will use SPI to read data from 1 register at a time 
uint8_t UM7_Read_Data(uint8_t *data, uint8_t length, uint8_t reg)
{
	uint8_t dummy = 0;
	HAL_GPIO_WritePin(GPIOB, SPI1_CHIPSEL_Pin, GPIO_PIN_RESET);

	// Create request for the register value
	uint8_t requestData[2] = {UM7_READ_BIT, reg};
	// Send the register that we want to read from
	HAL_SPI_Transmit(&hspi1, requestData, 2, 10);
	
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY){};
	HAL_SPI_Receive(&hspi1, &dummy, 1, 10);
	// Loop through 4 times to get each byte of the register data
	for (int8_t i = length - 1; i >= 0; i--)
	{
		HAL_SPI_Receive(&hspi1, &data[i], 1, 10);
		HAL_SPI_Transmit(&hspi1, 0x00, 1, 10);
	}
	
	HAL_GPIO_WritePin(GPIOB, SPI1_CHIPSEL_Pin, GPIO_PIN_SET);
	return 0;
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

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
#include "PowerModes.h"
#include "SPI_FRAM.h"
#include "DateConversion.h"
#include "Current_Control_Functions.h"
#include "Fuel_Gauge_Functions.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBUG (0)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim10;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint16_t globalIntterupt = 0;
States globalState = Detumble;
volatile States state;
uint8_t shortCheck = 0;
uint8_t checkBatt = FALSE;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM10_Init(void);
/* USER CODE BEGIN PFP */
void Output_Power_Pins(uint8_t currState);
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
  MX_SPI2_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */
	
	/* Initialize Variables */
	uint8_t firstTransition = 0;
	/* Initialize the current controllers */
	//Initialize_All_Current_Sensors(&hi2c1);
	
	/* Initialize the fuel gauge */
	Fuel_Gauge_Init(&hi2c1);
	
	/* Set Initial state */
	state = Detumble;
	
	/* Start Timer for interrupt drive fuel gauge check */
	HAL_TIM_Base_Start(&htim10);
	
	/* Start Timer Interrupt Handler */
	HAL_TIM_Base_Start_IT(&htim10);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	char msg[200] = {0};
  while (1)
  {
//		float battPercentage = Get_Voltage(&hi2c1, 0); // Currently returns a value in volts
//		snprintf(msg, 200, "\nBatt: %f \n\n", battPercentage);
//		HAL_UART_Transmit(&huart2, (uint8_t *)msg, 200, 100);
//		HAL_Delay(1000);
		/* Enter the state machine */
		while(1)
		{
			
			if (checkBatt)
			{
				HAL_GPIO_TogglePin(GPIOA, BOARD_LED_Pin);

				// Check battery level
				float battPercentage = Get_Voltage(&hi2c1, 0); // Currently returns a value in volts
				// Write the battery level to the fram
				uint8_t battBytes[4] = {0};
				float_to_bytes(battPercentage, battBytes);
				SPI_FRAM_Write(&hspi2, SPI_FRAM_BATT_LEVEL_ADDR, battBytes, 4, &huart2);
				Set_BatteryLevel(battPercentage);
				
				// Check the science event pin
				uint8_t scienceStatus = 0;
				if (HAL_GPIO_ReadPin(GPIOA, SCIENCE_EVENT_Pin) == GPIO_PIN_SET)
				{
					scienceStatus = Set_ScienceEvent(TRUE);
				}
				else
				{
					scienceStatus = Set_ScienceEvent(FALSE);
				}
				
				// Check if there is a short in any of the rails
				//Check_for_Shorts(&hi2c1, &shortCheck);
				
				// Get whether the craft is detumbling or not
				uint8_t stable[1] = {0};
				SPI_FRAM_Read(&hspi2, SPI_FRAM_MECH_STATE_ADDR, stable, 1, &huart2);
				if (stable[0] != 0x2) // Detumble
				{
					change_variables(STABLE, 0);
				}
				
				// Get the solar vector status
				uint8_t currentSensor[1] = {0};
				SPI_FRAM_Read(&hspi2, SPI_FRAM_SOLAR_VECTOR_ADDR, currentSensor, 1, &huart2);
				change_variables(SOLAR, currentSensor[0]);
				
				
				checkBatt = FALSE;
				snprintf(msg, 200, "\nState: %u    Batt: %f     Shorts: 0x%02X     ScienceStatus: 0x%02x    SolarVector: 0x%02x\n\n", state, battPercentage, shortCheck, scienceStatus, currentSensor[0]);
				HAL_UART_Transmit(&huart2, (uint8_t *)msg, 200, 100);
			}
			
			if (state != globalState)
			{
				firstTransition = 0;
				SPI_FRAM_Write(&hspi2, SPI_FRAM_PM_STATE_ADDR, (uint8_t *) &state, 1, &huart2);
			}
			globalState = state;
			switch (state)
			{
				/* In Detumble mode */
				case (Detumble): 
					// Set rails high for this state
					if (firstTransition == 0)
					{
						Output_Power_Pins(state);
						firstTransition = 1;
					}
					state = Transition(Detumble, &hspi2, &huart2);
					break;
				/* In Kill mode */
				case (Kill): 
					// Set rails high for this state
					if (firstTransition == 0)
					{
						Output_Power_Pins(state);
						firstTransition = 1;
					}
					state = Transition(Kill, &hspi2, &huart2);
					break;
				/* In Normal mode */
				case (Normal): 
					// Set rails high for this state
					if (firstTransition == 0)
					{
						Output_Power_Pins(state);
						firstTransition = 1;
					}
					state = Transition(Normal, &hspi2, &huart2);
					break;
				/* In UltraLowPower mode */	
				case (UltraLowPower): 
					// Set rails high for this state
					if (firstTransition == 0)
					{
						Output_Power_Pins(state);
						firstTransition = 1;
					}
					state = Transition(UltraLowPower, &hspi2, &huart2);
					break;
				/* In LowPower mode */
				case (LowPower): 
					// Set rails high for this state
					if (firstTransition == 0)
					{
						Output_Power_Pins(state);
						firstTransition = 1;
					}
					state = Transition(LowPower, &hspi2, &huart2);
					break;
				/* In Eclipse mode */
				case (Eclipse): 
					// Set rails high for this state
					if (firstTransition == 0)
					{
						Output_Power_Pins(state);
						firstTransition = 1;
					}
					state = Transition(Eclipse, &hspi2, &huart2);
					break;
				/* In ScienceOnly mode */
				case (ScienceOnly): 
					// Set rails high for this state
					if (firstTransition == 0)
					{
						Output_Power_Pins(state);
						firstTransition = 1;
					}
					state = Transition(ScienceOnly, &hspi2, &huart2);
					break;
				/* An error occurred */
				default:
					// Error
					return 0;
			}
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
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV16;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV16;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
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
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 0;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 65535;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim10, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

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
  huart2.Init.BaudRate = 9600;
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, BOARD_LED_Pin|Scie_Rail_Pin|Mech_Rail_Pin|Telemetry_Rail_Pin 
                          |DEAD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, SPI_FRAM_LOCK_Pin|Misc_Rail_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SPI_FRAM_CS_Pin|Memory_Rail_Pin|LT_Rail_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BOARD_LED_Pin Scie_Rail_Pin Mech_Rail_Pin Telemetry_Rail_Pin 
                           DEAD_Pin */
  GPIO_InitStruct.Pin = BOARD_LED_Pin|Scie_Rail_Pin|Mech_Rail_Pin|Telemetry_Rail_Pin 
                          |DEAD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI_FRAM_LOCK_Pin Misc_Rail_Pin */
  GPIO_InitStruct.Pin = SPI_FRAM_LOCK_Pin|Misc_Rail_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI_FRAM_CS_Pin Memory_Rail_Pin LT_Rail_Pin */
  GPIO_InitStruct.Pin = SPI_FRAM_CS_Pin|Memory_Rail_Pin|LT_Rail_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : DIE_INT_Pin */
  GPIO_InitStruct.Pin = DIE_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DIE_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI_FRAM_IN2_Pin SPI_FRAM_IN1_Pin */
  GPIO_InitStruct.Pin = SPI_FRAM_IN2_Pin|SPI_FRAM_IN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SCIENCE_EVENT_Pin */
  GPIO_InitStruct.Pin = SCIENCE_EVENT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SCIENCE_EVENT_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{
  if (GPIO_PIN == DIE_INT_Pin)
	{
		change_variables(DIE, 0);
		globalIntterupt = DIE_INT_Pin;
		HAL_GPIO_TogglePin(GPIOA , GPIO_PIN_5);
	}
	else 
	{
		__NOP();
	}
}

void Output_Power_Pins(uint8_t currState)
{	
	// Set the MechanicalSys power mode
	if (currState == Detumble || currState == Normal || currState == LowPower || currState == UltraLowPower || currState == Eclipse)
	{
		if ((shortCheck & 0x16) != 0) // Check if there was a short in the rail
		{
			HAL_GPIO_WritePin(GPIOA, Mech_Rail_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, Mech_Rail_Pin, GPIO_PIN_RESET);
		}
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, Mech_Rail_Pin, GPIO_PIN_RESET);
	}
	
	// Set the power for CC and Telemetry systems
	if (currState == Detumble || currState == Normal || currState == LowPower || currState == UltraLowPower || currState == Eclipse || currState == ScienceOnly)
	{
		if ((shortCheck & 0x14) != 0) // Check if there was a short in the rail
		{
			HAL_GPIO_WritePin(GPIOB, Memory_Rail_Pin, GPIO_PIN_SET);
			if (currState != Detumble)
			{
				HAL_GPIO_WritePin(GPIOB, Telemetry_Rail_Pin, GPIO_PIN_SET);
			}
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, Telemetry_Rail_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, Memory_Rail_Pin, GPIO_PIN_RESET);
		}
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, Telemetry_Rail_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, Memory_Rail_Pin, GPIO_PIN_RESET);
	}
	
	// Set the power for the Science payload systems
	if (currState == ScienceOnly)
	{
		if ((shortCheck & 0x4C) != 0) // Check if there was a short in the rail
		{
			HAL_GPIO_WritePin(GPIOA, Scie_Rail_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, Scie_Rail_Pin, GPIO_PIN_RESET);
		}
	}
	else 
	{
		HAL_GPIO_WritePin(GPIOA, Scie_Rail_Pin, GPIO_PIN_RESET);
	}
	
	// Set the LT power rail
	if (currState == Normal)
	{
		if ((shortCheck & 0x3F) != 0) // Check if there was a short in the rail
		{
			HAL_GPIO_WritePin(GPIOB, LT_Rail_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, LT_Rail_Pin, GPIO_PIN_RESET);
		}
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, LT_Rail_Pin, GPIO_PIN_RESET);
	}
	
	// Set a pin when Kill mode is entered, used for debugging or shutting off all rails
	if (currState == Kill)
	{
		HAL_GPIO_WritePin(GPIOB, DEAD_Pin, GPIO_PIN_SET);
		
		HAL_GPIO_WritePin(GPIOB, LT_Rail_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, Scie_Rail_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, Telemetry_Rail_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, Mech_Rail_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, Memory_Rail_Pin, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, DEAD_Pin, GPIO_PIN_RESET);
	}
}

/* TIMER INTERRUPT SERVICE ROUTINE */
/**
  * @brief This function handles TIM6 update event interrupt.
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
		checkBatt = TRUE;
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

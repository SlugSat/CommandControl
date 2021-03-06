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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUTTON_DOWN 0xFE
#define TEST (2)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
	uint8_t button_states = 0;

//uint16_t push=0;
//uint16_t another =0;
//		uint16_t flag=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void Test1(void);
void BatteryLevelChange(void);

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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if (TEST == 1)
		{
			Test1();
		}
		else if (TEST == 2)
		{
			BatteryLevelChange();
		}
			
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, BOARD_LED_Pin|SCI_EVENT_Pin|STABLE_Pin|SOLAR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BATT_GPIO_Port, BATT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DIE_GPIO_Port, DIE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BLUE_PUSH_Pin */
  GPIO_InitStruct.Pin = BLUE_PUSH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BLUE_PUSH_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BOARD_LED_Pin SCI_EVENT_Pin STABLE_Pin SOLAR_Pin */
  GPIO_InitStruct.Pin = BOARD_LED_Pin|SCI_EVENT_Pin|STABLE_Pin|SOLAR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BATT_Pin */
  GPIO_InitStruct.Pin = BATT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BATT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DIE_Pin */
  GPIO_InitStruct.Pin = DIE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DIE_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void Test1(void)
{
	/*right bit shift button_states variable
		once to save previous state*/
	button_states <<= 1;
	
	/*store current button state into button_states*/
	button_states |= HAL_GPIO_ReadPin(GPIOC, BLUE_PUSH_Pin); 
	
	/* blue button press indicates start of simulation */
	if (button_states == BUTTON_DOWN)
	{
		HAL_GPIO_TogglePin(GPIOA, BOARD_LED_Pin);
		/*simulate detumble state and set pins at varying
			frequencies except stable*/
		for (int i = 0; i < 100000; i++)
		{
			if (!(i % 900))
			{
				// BATTERY LEVEL CHANGE
				HAL_GPIO_TogglePin(GPIOC, BATT_Pin);
//					HAL_Delay(5);
//					HAL_GPIO_TogglePin(GPIOA, BATT_Pin);
			}
			if (!(i % 1000))
			{
				HAL_GPIO_TogglePin(GPIOA, BOARD_LED_Pin);
				// SCIENCE EVENT PULSE
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
//					HAL_Delay(5);
//					HAL_GPIO_TogglePin(GPIOA, SCI_EVENT_Pin);
			}
			if (!(i % 10000))
			{
//					HAL_GPIO_TogglePin(GPIOA, BOARD_LED_Pin);
				// SOLAR VECTOR PULSE
				HAL_GPIO_TogglePin(GPIOA, SOLAR_Pin);
//					HAL_Delay(5);
//					HAL_GPIO_TogglePin(GPIOA, SOLAR_Pin);
			}
		}
		
		/*we have successfully stabilized at this point*/
		// SEND STABLE SIGNAL
		HAL_GPIO_TogglePin(GPIOA, STABLE_Pin);
		HAL_Delay(100);
		HAL_GPIO_TogglePin(GPIOA, STABLE_Pin);
		
		HAL_Delay(100);
		
		HAL_GPIO_TogglePin(GPIOA, STABLE_Pin);
		HAL_Delay(100);
		HAL_GPIO_TogglePin(GPIOA, STABLE_Pin);
		HAL_Delay(100);
		
		HAL_GPIO_TogglePin(GPIOA, STABLE_Pin);
		HAL_Delay(100);
		HAL_GPIO_TogglePin(GPIOA, STABLE_Pin);
		HAL_Delay(100);
		
		HAL_GPIO_TogglePin(GPIOA, STABLE_Pin);
		HAL_Delay(100);
		HAL_GPIO_TogglePin(GPIOA, STABLE_Pin);
	}
}
	
void BatteryLevelChange(void)
{
	// D > LP > > ULP > LP > ULP > LP > N > K
	
	/* right bit shift button_states variable
	 * once to save previous state */
	button_states <<= 1;
	
	/*store current button state into button_states*/
	button_states |= HAL_GPIO_ReadPin(GPIOC, BLUE_PUSH_Pin); 
	
	/* blue button press indicates start of simulation */
	if (button_states == BUTTON_DOWN)
	{
		// Turn on MCU LED
		HAL_GPIO_TogglePin(GPIOA, BOARD_LED_Pin);
		
		// Stable interrupt
		for (int i = 0; i < 2; i++)
		{
			HAL_GPIO_TogglePin(GPIOA, STABLE_Pin);
			HAL_Delay(100);
			HAL_GPIO_TogglePin(GPIOA, STABLE_Pin);
			HAL_Delay(100);
		}
		
		// Battery Interrupt... 100 interrupts to go from 100% to 1%
		for (int i = 100; i > 1; i--)
		{
			HAL_GPIO_TogglePin(GPIOC, BATT_Pin);
			HAL_Delay(50);
		}
		
		// Battery Interrupt... 100 interrupts to go from 1% to 100%
		for (int i = 0; i < 100; i++)
		{
			HAL_GPIO_TogglePin(GPIOC, BATT_Pin);
			HAL_Delay(50);
		}
		HAL_GPIO_TogglePin(GPIOC, BATT_Pin);
		
		// Send the kill signal
		for (int i = 0; i < 5; i++)
		{
			HAL_GPIO_TogglePin(GPIOB, DIE_Pin);
			HAL_Delay(50);
			HAL_GPIO_TogglePin(GPIOB, DIE_Pin);
			HAL_Delay(50);
		}
		// Test End
		return;		
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

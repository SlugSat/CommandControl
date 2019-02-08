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
typedef enum
{
	/* Page id register definition */
	PAGE_ID_ADDR                                     = 0x07,

	/* PAGE0 REGISTER DEFINITION START*/
	CHIP_ID_ADDR                                     = 0x00,
	ACCEL_REV_ID_ADDR                                = 0x01,
	MAG_REV_ID_ADDR                                  = 0x02,
	GYRO_REV_ID_ADDR                                 = 0x03,
	SW_REV_ID_LSB_ADDR                               = 0x04,
	SW_REV_ID_MSB_ADDR                               = 0x05,
	BL_REV_ID_ADDR                                   = 0X06,

	/* Accel data register */
	ACCEL_DATA_X_LSB_ADDR                            = 0x08,
	ACCEL_DATA_X_MSB_ADDR                            = 0x09,
	ACCEL_DATA_Y_LSB_ADDR                            = 0x0A,
	ACCEL_DATA_Y_MSB_ADDR                            = 0x0B,
	ACCEL_DATA_Z_LSB_ADDR                            = 0x0C,
	ACCEL_DATA_Z_MSB_ADDR                            = 0x0D,

	/* Mag data register */
	MAG_DATA_X_LSB_ADDR                              = 0x0E,
	MAG_DATA_X_MSB_ADDR                              = 0x0F,
	MAG_DATA_Y_LSB_ADDR                              = 0x10,
	MAG_DATA_Y_MSB_ADDR                              = 0x11,
	MAG_DATA_Z_LSB_ADDR                              = 0x12,
	MAG_DATA_Z_MSB_ADDR                              = 0x13,

	/* Gyro data registers */
	GYRO_DATA_X_LSB_ADDR                             = 0x14,
	GYRO_DATA_X_MSB_ADDR                             = 0x15,
	GYRO_DATA_Y_LSB_ADDR                             = 0x16,
	GYRO_DATA_Y_MSB_ADDR                             = 0x17,
	GYRO_DATA_Z_LSB_ADDR                             = 0x18,
	GYRO_DATA_Z_MSB_ADDR                             = 0x19,

	/* Euler data registers */
	EULER_H_LSB_ADDR                                 = 0x1A,
	EULER_H_MSB_ADDR                                 = 0x1B,
	EULER_R_LSB_ADDR                                 = 0x1C,
	EULER_R_MSB_ADDR                                 = 0x1D,
	EULER_P_LSB_ADDR                                 = 0x1E,
	EULER_P_MSB_ADDR                                 = 0x1F,

	/* Quaternion data registers */
	QUATERNION_DATA_W_LSB_ADDR                       = 0x20,
	QUATERNION_DATA_W_MSB_ADDR                       = 0x21,
	QUATERNION_DATA_X_LSB_ADDR                       = 0x22,
	QUATERNION_DATA_X_MSB_ADDR                       = 0x23,
	QUATERNION_DATA_Y_LSB_ADDR                       = 0x24,
	QUATERNION_DATA_Y_MSB_ADDR                       = 0x25,
	QUATERNION_DATA_Z_LSB_ADDR                       = 0x26,
	QUATERNION_DATA_Z_MSB_ADDR                       = 0x27,

	/* Linear acceleration data registers */
	LINEAR_ACCEL_DATA_X_LSB_ADDR                     = 0x28,
	LINEAR_ACCEL_DATA_X_MSB_ADDR                     = 0x29,
	LINEAR_ACCEL_DATA_Y_LSB_ADDR                     = 0x2A,
	LINEAR_ACCEL_DATA_Y_MSB_ADDR                     = 0x2B,
	LINEAR_ACCEL_DATA_Z_LSB_ADDR                     = 0x2C,
	LINEAR_ACCEL_DATA_Z_MSB_ADDR                     = 0x2D,

	/* Gravity data registers */
	GRAVITY_DATA_X_LSB_ADDR                          = 0x2E,
	GRAVITY_DATA_X_MSB_ADDR                          = 0x2F,
	GRAVITY_DATA_Y_LSB_ADDR                          = 0x30,
	GRAVITY_DATA_Y_MSB_ADDR                          = 0x31,
	GRAVITY_DATA_Z_LSB_ADDR                          = 0x32,
	GRAVITY_DATA_Z_MSB_ADDR                          = 0x33,

	/* Temperature data register */
	TEMP_ADDR                                        = 0x34,

	/* Status registers */
	CALIB_STAT_ADDR                                  = 0x35,
	SELFTEST_RESULT_ADDR                             = 0x36,
	INTR_STAT_ADDR                                   = 0x37,

	SYS_CLK_STAT_ADDR                                = 0x38,
	SYS_STAT_ADDR                                    = 0x39,
	SYS_ERR_ADDR                                     = 0x3A,

	/* Unit selection register */
	UNIT_SEL_ADDR                                    = 0x3B,
	DATA_SELECT_ADDR                                 = 0x3C,

	/* Mode registers */
	OPR_MODE_ADDR                                    = 0x3D,
	PWR_MODE_ADDR                                    = 0x3E,

	SYS_TRIGGER_ADDR                                 = 0x3F,
	TEMP_SOURCE_ADDR                                 = 0x40,

	/* Axis remap registers */
	AXIS_MAP_CONFIG_ADDR                             = 0x41,
	AXIS_MAP_SIGN_ADDR                               = 0x42,

	/* SIC registers */
	SIC_MATRIX_0_LSB_ADDR                            = 0x43,
	SIC_MATRIX_0_MSB_ADDR                            = 0x44,
	SIC_MATRIX_1_LSB_ADDR                            = 0x45,
	SIC_MATRIX_1_MSB_ADDR                            = 0x46,
	SIC_MATRIX_2_LSB_ADDR                            = 0x47,
	SIC_MATRIX_2_MSB_ADDR                            = 0x48,
	SIC_MATRIX_3_LSB_ADDR                            = 0x49,
	SIC_MATRIX_3_MSB_ADDR                            = 0x4A,
	SIC_MATRIX_4_LSB_ADDR                            = 0x4B,
	SIC_MATRIX_4_MSB_ADDR                            = 0x4C,
	SIC_MATRIX_5_LSB_ADDR                            = 0x4D,
	SIC_MATRIX_5_MSB_ADDR                            = 0x4E,
	SIC_MATRIX_6_LSB_ADDR                            = 0x4F,
	SIC_MATRIX_6_MSB_ADDR                            = 0x50,
	SIC_MATRIX_7_LSB_ADDR                            = 0x51,
	SIC_MATRIX_7_MSB_ADDR                            = 0x52,
	SIC_MATRIX_8_LSB_ADDR                            = 0x53,
	SIC_MATRIX_8_MSB_ADDR                            = 0x54,

	/* Accelerometer Offset registers */
	ACCEL_OFFSET_X_LSB_ADDR                                 = 0x55,
	ACCEL_OFFSET_X_MSB_ADDR                                 = 0x56,
	ACCEL_OFFSET_Y_LSB_ADDR                                 = 0x57,
	ACCEL_OFFSET_Y_MSB_ADDR                                 = 0x58,
	ACCEL_OFFSET_Z_LSB_ADDR                                 = 0x59,
	ACCEL_OFFSET_Z_MSB_ADDR                                 = 0x5A,

	/* Magnetometer Offset registers */
	MAG_OFFSET_X_LSB_ADDR                                   = 0x5B,
	MAG_OFFSET_X_MSB_ADDR                                   = 0x5C,
	MAG_OFFSET_Y_LSB_ADDR                                   = 0x5D,
	MAG_OFFSET_Y_MSB_ADDR                                   = 0x5E,
	MAG_OFFSET_Z_LSB_ADDR                                   = 0x5F,
	MAG_OFFSET_Z_MSB_ADDR                                   = 0x60,

	/* Gyroscope Offset register s*/
	GYRO_OFFSET_X_LSB_ADDR                                  = 0x61,
	GYRO_OFFSET_X_MSB_ADDR                                  = 0x62,
	GYRO_OFFSET_Y_LSB_ADDR                                  = 0x63,
	GYRO_OFFSET_Y_MSB_ADDR                                  = 0x64,
	GYRO_OFFSET_Z_LSB_ADDR                                  = 0x65,
	GYRO_OFFSET_Z_MSB_ADDR                                  = 0x66,

	/* Radius registers */
	ACCEL_RADIUS_LSB_ADDR                                   = 0x67,
	ACCEL_RADIUS_MSB_ADDR                                   = 0x68,
	MAG_RADIUS_LSB_ADDR                                     = 0x69,
	MAG_RADIUS_MSB_ADDR                                     = 0x6A

} IMU_Reg_t;

typedef enum
{
	POWER_MODE_NORMAL                                       = 0x00,
	POWER_MODE_LOWPOWER                                     = 0x01,
	POWER_MODE_SUSPEND                                      = 0x02
} IMU_Power_Mode_t;

typedef enum
{
	/* Operation mode settings*/
	OPERATION_MODE_CONFIG                                   = 0x00,
	OPERATION_MODE_ACCONLY                                  = 0x01,
	OPERATION_MODE_MAGONLY                                  = 0x02,
	OPERATION_MODE_GYRONLY                                  = 0x03,
	OPERATION_MODE_ACCMAG                                   = 0x04,
	OPERATION_MODE_ACCGYRO                                  = 0x05,
	OPERATION_MODE_MAGGYRO                                  = 0x06,
	OPERATION_MODE_AMG                                      = 0x07,
	OPERATION_MODE_IMUPLUS                                  = 0x08,
	OPERATION_MODE_COMPASS                                  = 0x09,
	OPERATION_MODE_M4G                                      = 0x0A,
	OPERATION_MODE_NDOF_FMC_OFF                             = 0x0B,
	OPERATION_MODE_NDOF                                     = 0x0C
} IMU_Op_Mode_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define IMU_ADDRESS_DEF (0x29 << 1)
#define IMU_ADDRESS_ALT (0x28 << 1)
#define IMU_SLAVE_ADDR (0x40)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
uint8_t BIST(void);
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

	if (HAL_I2C_IsDeviceReady(&hi2c1, IMU_ADDRESS_ALT, 2, 10) == HAL_OK)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
	HAL_Delay(500);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	HAL_Delay(500);
	
	uint8_t txData[2] = {SYS_TRIGGER_ADDR, SELFTEST_RESULT_ADDR}; 
	uint8_t rxData = 0;
	
	uint8_t bistData = 0;
	
	HAL_I2C_Master_Transmit(&hi2c1, IMU_ADDRESS_ALT, &txData[1], 1, 10);
	HAL_I2C_Master_Receive( &hi2c1, IMU_ADDRESS_ALT, &rxData,    1, 10);
		
		
	rxData &= 0x0F;
	if (rxData == 0x0F)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // Turn on
	}
	
	HAL_Delay(500);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // Turn off
	HAL_Delay(500);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		HAL_Delay(1);
		bistData = BIST();
	//	uint8_t i = 0;
		
		bistData &= 0x07; //(0x01 << (i++ % 3));
		if (bistData == 0x07)
		{
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		}
		
		HAL_Delay(500);
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

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
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

uint8_t BIST(void)
{
	IMU_Op_Mode_t mode = OPERATION_MODE_CONFIG;
	uint8_t setConfig[2] = {OPR_MODE_ADDR, mode};
	
	HAL_I2C_Master_Transmit(&hi2c1, IMU_ADDRESS_ALT, &setConfig[0], 2, 10);
	
	IMU_Reg_t sysTrigReg = SYS_TRIGGER_ADDR;
	uint8_t setSelfTestTrig[2] = {sysTrigReg, 0x1};
	
	HAL_I2C_Master_Transmit(&hi2c1, IMU_ADDRESS_ALT, &setSelfTestTrig[0], 2, 10);
	
	sysTrigReg = SELFTEST_RESULT_ADDR;
	uint8_t getSelfTestResult[1] = {sysTrigReg};
	uint8_t readTest = 0;
	
	HAL_I2C_Master_Transmit(&hi2c1, IMU_ADDRESS_ALT, &getSelfTestResult[0], 1, 10);
	HAL_I2C_Master_Receive( &hi2c1, IMU_ADDRESS_ALT, &readTest,   1, 10);
	
	HAL_Delay(30);
	return readTest;	
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

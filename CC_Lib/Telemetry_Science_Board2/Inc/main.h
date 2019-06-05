/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI_FRAM_CS_Pin GPIO_PIN_1
#define SPI_FRAM_CS_GPIO_Port GPIOB
#define SPI_FRAM_LOCK_Pin GPIO_PIN_12
#define SPI_FRAM_LOCK_GPIO_Port GPIOB
#define SPI_FRAM_SCK_Pin GPIO_PIN_13
#define SPI_FRAM_SCK_GPIO_Port GPIOB
#define SPI_FRAM_MISO_Pin GPIO_PIN_14
#define SPI_FRAM_MISO_GPIO_Port GPIOB
#define SPI_FRAM_MOSI_Pin GPIO_PIN_15
#define SPI_FRAM_MOSI_GPIO_Port GPIOB
#define SP_CC_RESET_Pin GPIO_PIN_7
#define SP_CC_RESET_GPIO_Port GPIOC
#define Science_Event_Pin GPIO_PIN_8
#define Science_Event_GPIO_Port GPIOC
#define SPI_CC_CS_Pin GPIO_PIN_9
#define SPI_CC_CS_GPIO_Port GPIOA
#define Kill_to_PModes_Int_Pin GPIO_PIN_10
#define Kill_to_PModes_Int_GPIO_Port GPIOA
#define SPI_FRAM_IN2_Pin GPIO_PIN_11
#define SPI_FRAM_IN2_GPIO_Port GPIOA
#define SPI_FRAM_IN1_Pin GPIO_PIN_12
#define SPI_FRAM_IN1_GPIO_Port GPIOA
#define SPI_CC_SCK_Pin GPIO_PIN_3
#define SPI_CC_SCK_GPIO_Port GPIOB
#define SPI_CC_MISO_Pin GPIO_PIN_4
#define SPI_CC_MISO_GPIO_Port GPIOB
#define SPI_CC_MOSI_Pin GPIO_PIN_5
#define SPI_CC_MOSI_GPIO_Port GPIOB
#define I2C_FRAM_SCL_Pin GPIO_PIN_8
#define I2C_FRAM_SCL_GPIO_Port GPIOB
#define I2C_FRAM_SDA_Pin GPIO_PIN_9
#define I2C_FRAM_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

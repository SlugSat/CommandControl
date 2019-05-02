/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#define BOARD_LED_Pin GPIO_PIN_5
#define BOARD_LED_GPIO_Port GPIOA
#define ULOW_Pin GPIO_PIN_6
#define ULOW_GPIO_Port GPIOA
#define LOW_Pin GPIO_PIN_7
#define LOW_GPIO_Port GPIOA
#define BATT_INT_Pin GPIO_PIN_2
#define BATT_INT_GPIO_Port GPIOB
#define BATT_INT_EXTI_IRQn EXTI2_IRQn
#define DIE_INT_Pin GPIO_PIN_10
#define DIE_INT_GPIO_Port GPIOB
#define DIE_INT_EXTI_IRQn EXTI15_10_IRQn
#define ECLIPSE_Pin GPIO_PIN_7
#define ECLIPSE_GPIO_Port GPIOC
#define SCI_INT_Pin GPIO_PIN_8
#define SCI_INT_GPIO_Port GPIOA
#define SCI_INT_EXTI_IRQn EXTI9_5_IRQn
#define DETUMBLE_Pin GPIO_PIN_9
#define DETUMBLE_GPIO_Port GPIOA
#define STABLE_INT_Pin GPIO_PIN_3
#define STABLE_INT_GPIO_Port GPIOB
#define STABLE_INT_EXTI_IRQn EXTI3_IRQn
#define SOLAR_INT_Pin GPIO_PIN_4
#define SOLAR_INT_GPIO_Port GPIOB
#define SOLAR_INT_EXTI_IRQn EXTI4_IRQn
#define NORMAL_Pin GPIO_PIN_6
#define NORMAL_GPIO_Port GPIOB
#define DEAD_Pin GPIO_PIN_8
#define DEAD_GPIO_Port GPIOB
#define SCILOG_Pin GPIO_PIN_9
#define SCILOG_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

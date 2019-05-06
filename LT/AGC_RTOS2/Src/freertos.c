/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "agc.h"
#include "i2c.h"
#include "usart.h"

#include "FRAM_Lib.h"
#include "FRAM_Tests.h"
#include <stdlib.h>
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
/* USER CODE BEGIN Variables */
//osThreadId AGCTaskHandle;
/* USER CODE END Variables */
osThreadId AGCTaskHandle;
osThreadId ScienceTaskHandle;
osMutexId UART_MutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
//void StartAGCTask(void const * argument);
/* USER CODE END FunctionPrototypes */

void StartAGCTask(void const * argument);
void StartScienceTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* Create the mutex(es) */
  /* definition and creation of UART_Mutex */
  osMutexDef(UART_Mutex);
  UART_MutexHandle = osMutexCreate(osMutex(UART_Mutex));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of ScienceTask */
	osThreadDef(ScienceTask, StartScienceTask, osPriorityNormal, 0, 128);
  ScienceTaskHandle = osThreadCreate(osThread(ScienceTask), NULL);
  /* definition and creation of AGCTask */
  osThreadDef(AGCTask, StartAGCTask, osPriorityNormal, 0, 128);
  AGCTaskHandle = osThreadCreate(osThread(AGCTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  //osThreadDef(AGCTask, StartAGCTask, osPriorityNormal, 0, 128);
  //AGCTaskHandle = osThreadCreate(osThread(AGCTask), NULL);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* USER CODE BEGIN Header_StartAGCTask */
/**
  * @brief  Function implementing the AGCTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartAGCTask */
//void StartAGCTask(void const * argument)
//{

//  /* USER CODE BEGIN StartAGCTask */
//  /* Infinite loop */
//  for(;;)
//  {
//    osDelay(1);
//  }
//  /* USER CODE END StartAGCTask */
//}

/* USER CODE BEGIN Header_StartScienceTask */
/**
* @brief Function implementing the ScienceTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartScienceTask */
void StartScienceTask(void const * argument)
{
  /* USER CODE BEGIN StartScienceTask */
	uint8_t SCIENCE[8] = "Science\n";
	xSemaphoreTake(UART_MutexHandle, portMAX_DELAY);
	HAL_UART_Transmit(&huart2,SCIENCE,8,10);
	xSemaphoreGive(UART_MutexHandle);
	FRAM_Test(&hi2c1,&huart2);
	
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartScienceTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void StartAGCTask(void const * argument) {
	
	uint8_t AGC[8] = "AGC\n";
	xSemaphoreTake(UART_MutexHandle, portMAX_DELAY);
	HAL_UART_Transmit(&huart2,AGC,4,10);
	xSemaphoreGive(UART_MutexHandle);
	
	// initialize AGC stuff
	xSemaphoreTake(UART_MutexHandle, portMAX_DELAY);
	AGC_Init();
	xSemaphoreGive(UART_MutexHandle);
	
	for (;;) {
		// handle events as they come
		xSemaphoreTake(UART_MutexHandle, portMAX_DELAY);
		AGC_DoEvent();
		xSemaphoreGive(UART_MutexHandle);
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

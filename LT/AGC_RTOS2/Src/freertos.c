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
osThreadId AGCTaskHandle;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId ScienceTaskHandle;
osThreadId TelemSciTaskHandle;
osMutexId UART_MutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void StartAGCTask(void const * argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartScienceTask(void const * argument);
void StartDataRetrival(void const * argument);

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
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of ScienceTask */
  osThreadDef(ScienceTask, StartScienceTask, osPriorityIdle, 0, 128);
  ScienceTaskHandle = osThreadCreate(osThread(ScienceTask), NULL);

  /* definition and creation of TelemSciTask */
  osThreadDef(TelemSciTask, StartDataRetrival, osPriorityIdle, 0, 128);
  TelemSciTaskHandle = osThreadCreate(osThread(TelemSciTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  osThreadDef(AGCTask, StartAGCTask, osPriorityNormal, 0, 128);
  AGCTaskHandle = osThreadCreate(osThread(AGCTask), NULL);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

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
	
	FRAM_IO_Init(&hi2c1);
	FRAM_Write_Headers(&hi2c1);
	
  /* Infinite loop */
  for(;;)
  {
		//Generate Data
		
		//Enter it
		
    osDelay(1);
  }
  /* USER CODE END StartScienceTask */
}

/* USER CODE BEGIN Header_StartDataRetrival */
/**
* @brief Function implementing the TelemSciTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDataRetrival */
void StartDataRetrival(void const * argument)
{
  /* USER CODE BEGIN StartDataRetrival */
	uint8_t SUCCESS[9] = "Success\n";
	uint8_t ERROR[6] = "Fail\n";
	uint8_t NO_TIME[20] = "Time not found\n";
	uint8_t END_SEARCH[14] = "Search Over\n";
	uint8_t StrOut[50];
	struct ScienceDataPackage TestData;
	
	FRAM_Return SearchOut;
	FRAM_Return SearchReturn;
	
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000000);
		//Search and transmit some data
		
	uint32_t TestStart = 1000;
	uint32_t TestEnd = 10000;
	
	SearchReturn = FRAM_IO_Search_Start(&hi2c1, TestStart, TestEnd, &huart2);
	if(SearchReturn == FRAM_ERROR){
		HAL_UART_Transmit(&huart2,ERROR,5,10);
	}
	else if(SearchReturn == FRAM_TIME_NOT_FOUND){
		HAL_UART_Transmit(&huart2,NO_TIME,15,10);		
	}else{
		HAL_UART_Transmit(&huart2,SUCCESS,8,10);
	}
	
	uint16_t i = 0;
	while(1){
		SearchOut = FRAM_IO_Search_GetNextItem(&hi2c1, &TestData, &huart2);
		if(SearchOut == FRAM_ERROR){
			HAL_UART_Transmit(&huart2,ERROR,5,10);
		}
		if(SearchOut ==FRAM_SEARCH_ENDED){
			HAL_UART_Transmit(&huart2,END_SEARCH,12,10);
			break;
		}
		sprintf(StrOut,"%4d: Time: %5d, Energy: %2d\n",i , TestData.Time, TestData.Energy);
		HAL_UART_Transmit(&huart2,StrOut,strlen(StrOut),10);
		i++;
		
		if(i>1000){
			break;
		}
	}
		
  }
  /* USER CODE END StartDataRetrival */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void StartAGCTask(void const * argument) {
	// initialize AGC stuff
	AGC_Init();
	
	for (;;) {
		// handle events as they come
		AGC_DoEvent();
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

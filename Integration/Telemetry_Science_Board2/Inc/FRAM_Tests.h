/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"
#include "FRAM_Lib.h"
#include <stdio.h>
#include <stdlib.h>

void FRAM_Test(I2C_HandleTypeDef *i2c_handler, UART_HandleTypeDef *uart_handler);

void GenerateTestData(uint8_t *energy, uint32_t *time);
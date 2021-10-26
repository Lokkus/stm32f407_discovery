/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "system.h"
#include "logger.h"
#include <string.h> // test
#include <sstream>
#include "logger_test.h"


STM32F407::System stm_system;
using STM32F407::Leds;
Logger logger(USART1, DMA2_Stream7, DMA2_Stream5, DMA2);


void tests()
{

    logger_test_receive_data_DMA_2();
}



int main(void)
{
    tests();
    while (1)
    {
        GPIOD->ODR |= (1<<15);
    }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

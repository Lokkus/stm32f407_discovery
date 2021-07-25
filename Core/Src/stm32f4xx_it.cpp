/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f4xx_it.h"
#include "logger.h"
#include <string>

extern Logger logger;
uint32_t counter;
uint32_t rec_count;
std::string s;

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{

}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART1 global interrupt.
  */


void USART1_IRQHandler(void)
{
    if (logger.getUSARTinstance()->SR & USART_SR_TXE and logger.getUSARTinstance()->CR1 & USART_CR1_TXEIE)
    {
        try
        {
            logger.getUSARTinstance()->DR = logger.getDataToSend().at(counter++);
            while(logger.getUSARTinstance()->SR & USART_SR_TXE);
        }
        catch(std::out_of_range& oor)
        {
            while(!(logger.getUSARTinstance()->SR & USART_SR_TXE));
            counter = 0;
            logger.getDataToSend().clear();
            logger.clearFlagTransferStatus();
            logger.disableTransmission();
        }
    }

    // receive data
    if (logger.getUSARTinstance()->SR & USART_SR_RXNE)
    {
        GPIOD->ODR |=(1<<12);
        char c = logger.getUSARTinstance()->DR;
        logger.getReceivedData_IT().append(1, c);
        logger.getReceivedDataCounter()++;
        while(logger.getUSARTinstance()->SR & USART_SR_RXNE);

    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

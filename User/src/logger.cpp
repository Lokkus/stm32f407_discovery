/*
 * logger.cpp
 *
 *  Created on: May 18, 2021
 *      Author: mk
 */

#include "logger.h"
#include <string.h>
#include <string>
extern std::string s;
extern uint32_t rec_count;

Logger::Logger(USART_TypeDef* usart)
{
    usartInstance = usart;
    usartInstance->CR1 |= USART_CR1_UE;
    usartInstance->CR1 |= USART_CR1_TE;

    usartInstance->CR1 |= USART_CR1_RE;
    usartInstance->CR1 |= USART_CR1_RXNEIE;
}

void Logger::disableTransmission()
{
    usartInstance->CR1 &=~USART_CR1_TXEIE;
}

void Logger::sendData_IT(std::string stringToSend)
{
    this->controlFlag = true;
    this->dataToTransfer.clear();
    this->dataToTransfer = stringToSend;
    usartInstance->CR1 |= USART_CR1_TXEIE;
    while(controlFlag);
}

bool Logger::getTransferStatusFlag()
{
    return this->controlFlag;
}

bool Logger::getReceiveStatusFlag()
{
    return this-receivedDataFlag;
}

void Logger::setReveivedStatusFlag()
{
    this->receivedDataFlag = true;
}

void Logger::clearFlagTransferStatus()
{
    this->controlFlag = false;
}

std::string& Logger::getDataToSend()
{
    return this->dataToTransfer;
}

std::string& Logger::getReceivedData_IT()
{
    return this->receivedData;
}

USART_TypeDef* Logger::getUSARTinstance()
{
    return usartInstance;
}

uint32_t& Logger::getReceivedDataCounter()
{
    return receivedDataCounter;
}

void Logger::receiveData_IT(uint32_t counter)
{

    while(receivedDataCounter != counter);
}



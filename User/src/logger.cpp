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



Logger::Logger(USART_TypeDef* USART_instance, DMA_Stream_TypeDef* DMA_stream_tx_instance,
        DMA_Stream_TypeDef* DMA_stream_rx_instance, DMA_TypeDef* DMA_instance)
{

    this->USART_instance = USART_instance;
    this->DMA_stream_tx_instance = DMA_stream_tx_instance;
    this->DMA_stream_rx_instance = DMA_stream_rx_instance;
    this->DMA_instance = DMA_instance;

    _init_HW();

}

void Logger::_init_HW()
{
    // init usart part
    USART_instance->CR1 |=  USART_CR1_UE | USART_CR1_TE |
                            USART_CR1_RE | USART_CR1_RXNEIE;

    DMA_stream_rx_instance->NDTR = DMA_BUFFER;
    DMA_stream_rx_instance->CR |= DMA_SxCR_CIRC;

    DMA_stream_rx_instance->M0AR = reinterpret_cast<uint32_t>(dma_receive_buffer);
}

/******************************************************************************
 * these methods are invoked in stm32f4xx_it.cpp
 *****************************************************************************/
void Logger::USART_send_IRQ_handler()
{
    if(USART_instance->SR & USART_SR_TXE and USART_instance->CR1 & USART_CR1_TXEIE)
    {
        try
        {
            USART_instance->DR = data_to_transfer.at(send_data_counter++);
            while(USART_instance->SR & USART_SR_TXE);
        }
        catch(std::out_of_range& oor)
        {
            while(!(USART_instance->SR & USART_SR_TXE));
            send_data_counter = 0;
            data_to_transfer.clear();
            transfer_it_status_flag = false;
            USART_instance->CR1 &= ~USART_CR1_TXEIE;
        }
    }
}

void Logger::USART_receive_IRQ_handler()
{
    if(USART_instance->SR & USART_SR_RXNE and USART_instance->CR1 & USART_CR1_RXNEIE)
    {
        received_data_buffer.append(1, USART_instance->DR);
        received_data_counter++;
        while(USART_instance->SR & USART_SR_RXNE);
    }
}

void Logger::USART_idle_IRQ_handler()
{
    if (USART_instance->SR & USART_SR_IDLE and USART_instance->CR1 & USART_CR1_IDLEIE)
    {
        GPIOD->ODR |= (1<<12);
        volatile uint32_t tmp;
        tmp = USART_instance->SR;
        tmp = USART_instance->DR;
        received_data_flag = true;
        DMA_stream_rx_instance->CR &= ~DMA_SxCR_EN;
        tmp = tmp;
    }
}

void Logger::DMA_send_IRQ_handler()
{
    if (DMA_instance->HISR & DMA_HISR_TCIF7)
    {
        while(!(USART_instance->SR & USART_SR_TC));
        DMA_instance->HIFCR |= DMA_HIFCR_CTCIF7;
    }
}

void Logger::DMA_receive_IRQ_Handler()
{
    if(DMA_instance->HISR & DMA_HISR_TCIF5) // check if interrupt source is transfer complete
    {
        DMA_instance->HIFCR |= DMA_HIFCR_CTCIF5; // clear transfer complete flag
        uint32_t length = DMA_BUFFER - DMA_stream_rx_instance->NDTR-1;
        if (received_data_flag == false)
        {
            received_data_DMA.append(dma_receive_buffer, DMA_BUFFER);
        }
        else
        {
            received_data_DMA.append(dma_receive_buffer, length);
            DMA_stream_rx_instance->NDTR = DMA_BUFFER;
            DMA_stream_rx_instance->CR |= DMA_SxCR_EN;
        }
    }
}


/******************************************************************************/

void Logger::send_data_IT(std::string stringToSend)
{
    this->transfer_it_status_flag = true;
    this->data_to_transfer.clear();
    this->data_to_transfer = stringToSend;
    this->USART_instance->CR1 |= USART_CR1_TXEIE;
    while(transfer_it_status_flag);
}

std::string& Logger::receive_data_IT(uint32_t counter)
{
    received_data.clear();
    //USART_instance->CR1 &=~USART_CR1_IDLEIE;
    while(received_data_counter != counter);
    received_data = received_data_buffer;
    received_data_buffer.clear();
    received_data_counter = 0;
    //USART_instance->CR1 |= USART_CR1_IDLEIE;
    return received_data;
}


void Logger::send_data_DMA(const std::string& stringToSend)
{
    DMA_stream_tx_instance->M0AR = (uint32_t)stringToSend.data();
    DMA_stream_tx_instance->NDTR = stringToSend.length();
    DMA_stream_tx_instance->CR |= DMA_SxCR_EN;
}

std::string& Logger::receive_data_DMA()
{
    DMA_stream_rx_instance->CR |= DMA_SxCR_EN;
    while(received_data_flag == false);
    GPIOD->ODR |= (1<<13);
    received_data_flag = false;
    return received_data_DMA;
}


void Logger::clear_received_DMA_data()
{
    received_data_DMA.clear();
}

void Logger::remove_endline_from_data(std::string& data)
{
    if(! data.empty())
    {
        data.pop_back();
        data.pop_back();
    }
}

std::string& Logger::get_received_DMA_data()
{
    return received_data_DMA;
}




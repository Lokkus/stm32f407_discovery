/*
 * logger.h
 *
 *  Created on: May 18, 2021
 *      Author: mk
 */

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_
#include "main.h"

class Logger
{
    static constexpr uint32_t DMA_BUFFER = 5;

    USART_TypeDef* USART_instance;
    DMA_Stream_TypeDef* DMA_stream_tx_instance;
    DMA_Stream_TypeDef* DMA_stream_rx_instance;
    DMA_TypeDef* DMA_instance;

    std::string data_to_transfer;
    std::string received_data;
    std::string received_data_buffer;
    std::string received_data_DMA;
    char dma_receive_buffer[DMA_BUFFER];
    bool transfer_it_status_flag = true;
    bool received_data_flag = false;
    bool received_data_flag_DMA = false;

    uint32_t received_data_counter = 0;
    uint32_t send_data_counter = 0;


    void _init_HW();

public:
    Logger(USART_TypeDef* USART_instance,
            DMA_Stream_TypeDef* DMA_stream_tx_instance,
            DMA_Stream_TypeDef* DMA_stream_rx_instance,
            DMA_TypeDef* DMA_instance);

    void send_data_IT(std::string stringToSend);
    std::string& receive_data_IT(uint32_t counter);
    void send_data_DMA(const std::string& stringToSend);
    std::string& receive_data_DMA();


    std::string& getDataToSend();
    //std::string& get_received_data_IT();
    std::string& getReceivedData_DMA();
    USART_TypeDef* getUSARTinstance();
    DMA_Stream_TypeDef* getDMAstream_TX_Instence();
    DMA_Stream_TypeDef* getDMAstream_RX_Instence();
    DMA_TypeDef* getDMAinstance();
    void clear_received_DMA_data();
    void remove_endline_from_data(std::string& data);
    std::string& get_received_DMA_data();


    void USART_send_IRQ_handler();
    void USART_receive_IRQ_handler();
    void USART_idle_IRQ_handler();
    void DMA_send_IRQ_handler();
    void DMA_receive_IRQ_Handler();
};



#endif /* INC_LOGGER_H_ */

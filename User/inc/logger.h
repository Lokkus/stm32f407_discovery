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
    std::string dataToTransfer;
    std::string receivedData;
    bool controlFlag = true;
    bool receivedDataFlag = false;
    USART_TypeDef* usartInstance;
    uint32_t receivedDataCounter = 0;
public:
    Logger(USART_TypeDef* usart);
    void sendData_IT(std::string stringToSend);
    void receiveData_IT(uint32_t counter);
    void sendData_DMA(std::string stringToSend);
    std::string& getDataToSend();
    std::string& getReceivedData_IT();
    USART_TypeDef* getUSARTinstance();
    void disableTransmission();
    bool getTransferStatusFlag();
    bool getReceiveStatusFlag();
    void setReveivedStatusFlag();
    void clearFlagTransferStatus();
    uint32_t& getReceivedDataCounter();
    void setReceivedDataCounter();

};



#endif /* INC_LOGGER_H_ */

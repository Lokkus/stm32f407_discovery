/*
 * logger_test.cpp
 *
 *  Created on: Aug 25, 2021
 *      Author: mk
 */


#include "logger_test.h"
#include "logger.h"

extern Logger logger;

void logger_test_receive_it()
{
    std::string s = logger.receive_data_IT(10);
    logger.send_data_IT(s+"\n\r");
    s = logger.receive_data_IT(20);
    logger.send_data_IT(s+"\n\r");
}

void logger_test_send_data_DMA(const std::string s)
{
    std::stringstream ss;
    ss<<"I have "<<std::hex<<"0x"<<255<<" dogs and "<<std::fixed<<std::setprecision(6)<<1234.567899<<" hairs\n\r";
    logger.send_data_DMA(ss.str());
}

void logger_test_receive_data_DMA()
{
    std::string str = logger.receive_data_DMA();
    str+="Some string\n\r";
    logger.send_data_DMA(str);
    logger.clear_received_DMA_data();
    str = logger.receive_data_DMA();
    logger.remove_endline_from_data(str);
    logger.send_data_DMA(str+" add:)))\n\r");
}

void logger_test_receive_data_DMA_2()
{
    std::string str;
    while((str = logger.receive_data_DMA()) != "end")
    {
        str += " - Additional\n\r";
        logger.send_data_DMA(str);
        logger.clear_received_DMA_data();
    }
    logger.send_data_DMA("Finish\n\r");
}



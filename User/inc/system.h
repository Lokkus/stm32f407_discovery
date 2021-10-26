/*
 * system.h
 *
 *  Created on: May 18, 2021
 *      Author: mk
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include "main.h"


/*
 * This file contain all peripheral configuration used in project
 */

namespace STM32F407
{
    enum class Leds
    {
        LED_GREEN = 12,
        LED_ORANGE,
        LED_RED,
        LED_BLUE
    };

    class System
    {
        static constexpr uint32_t SYS_FREQ = 168000000;
        static constexpr uint32_t BAUD = 115200;
        static constexpr uint32_t APB2_FREQ = 84000000;

        void Perpiph_Config();
        void Clock_Config();
        void USART1_Config();
        void GPIOD_Config();
        void USART1_DMA_Config();

    public:
        System();
        void EnableLED(Leds led);

    };
}



#endif /* INC_SYSTEM_H_ */

/*
 * system.cpp
 *
 *  Created on: May 18, 2021
 *      Author: mk
 */


#include "system.h"

namespace STM32F407
{
    void System::Clock_Config(void)
    {
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

        LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
        while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_5)
        {
        }
        LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
        LL_RCC_HSE_Enable();

         /* Wait till HSE is ready */
        while(LL_RCC_HSE_IsReady() != 1)
        {

        }
        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLP_DIV_2);
        LL_RCC_PLL_Enable();

         /* Wait till PLL is ready */
        while(LL_RCC_PLL_IsReady() != 1)
        {

        }
        LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
        LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
        LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
        LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

         /* Wait till System clock is ready */
        while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
        {

        }
        LL_Init1msTick(SYS_FREQ);
        LL_SetSystemCoreClock(SYS_FREQ);

        NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    }

    void System::USART1_Config()
    {
        uint16_t pin_tx = 6;
        uint16_t pin_rx = 7;
        uint16_t alternative_mode = 2;
        uint16_t bits_in_field = 2;
        uint16_t very_high_speed = 3;
        uint16_t alternative_function = 7;

        // USART1 clock enable
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

        // GPIOB clock enable
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

        // Setting GPIOB pins for USART TX and RX
        GPIOB->MODER |= (alternative_mode << (bits_in_field * pin_tx)) |
                (alternative_mode << (bits_in_field * pin_rx));

        // speed max
        GPIOB->OSPEEDR |= (very_high_speed << (bits_in_field * pin_tx)) | 
                (very_high_speed << (bits_in_field * pin_rx));

        // alternate function for PB6 and PB7
        bits_in_field = 4;
        GPIOB->AFR[0] |= (alternative_function << (bits_in_field * pin_tx)) |
                (alternative_function << (bits_in_field * pin_rx));

        // USART1 config
        // set baudrate
        USART1->BRR = APB2_FREQ/BAUD;

        // interrrupts configuration
        NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 0));
        NVIC_EnableIRQ(USART1_IRQn);
    }

    void System::GPIOD_Config()
    {

        // enable clock for GPIOD
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

        // set pins as outputs
        GPIOD->MODER |= (1 << (2 * (uint16_t)(Leds::LED_GREEN))) |
                (1 << (2 * (uint16_t)(Leds::LED_ORANGE))) |
                (1 << (2 * (uint16_t)(Leds::LED_RED))) |
                (1 << (2 * (uint16_t)(Leds::LED_BLUE)));

        // max speed
        GPIOD->OSPEEDR |= (3 << (2 * (uint16_t)(Leds::LED_GREEN))) |
                (3 << (2 * (uint16_t)(Leds::LED_ORANGE))) |
                (3 * (2 * (uint16_t)(Leds::LED_RED))) |
                (3 * (2 * (uint16_t)(Leds::LED_BLUE)));
    }

    void System::EnableLED(Leds led)
    {
        GPIOD->ODR |= 1 << (uint32_t)(led);
    }

    void System::Perpiph_Config()
    {
        GPIOD_Config();
        USART1_Config();
    }

    System::System()
    {
        Clock_Config();
        Perpiph_Config();
    }
}


/*
 * main.h
 *
 *  Created on: Feb 18, 2024
 *      Author: rezk
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>


#define TRUE      1
#define FALSE     0


#define RCC_SYSCLK_50_MHZ     50
#define RCC_SYSCLK_84_MHZ     84
#define RCC_SYSCLK_120_MHZ    120

/********************** LEDs ************************/

#define GREAN_LED_PIN            GPIO_PIN_12
#define GREAN_LED_PORT           GPIOD

#define ORANGE_LED_PIN           GPIO_PIN_13
#define ORANGE_LED_PORT          GPIOD

#define RED_LED_PIN              GPIO_PIN_14
#define RED_LED_PORT             GPIOD

#define BLUE_LED_PIN             GPIO_PIN_15
#define BLUE_LED_PORT            GPIOD

#endif /* INC_MAIN_H_ */

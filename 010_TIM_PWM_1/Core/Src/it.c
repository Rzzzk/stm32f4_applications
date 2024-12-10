/*
 * it.c
 *
 *  Created on: Feb 18, 2024
 *      Author: rezk
 */
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef timer2_hn;


void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}


void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&timer2_hn);
}


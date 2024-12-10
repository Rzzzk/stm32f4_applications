/*
 * it.c
 *
 *  Created on: Feb 18, 2024
 *      Author: rezk
 */
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef timer6_hn;


void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&timer6_hn);
}

/*
 * msp.c
 *
 *  Created on: Feb 18, 2024
 *      Author: rezk
 */


#include "main.h"



/*
 * LOW LEVEL PROCESSOR INITIALIZATION
 */

void HAL_MspInit(void)
{
	/*
	 * 1. configure the priority group of the processor
	 */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/*
	 * 2. Enable the required system exceptions
	 */
	SCB->SHCSR |= 0x7 << 16; //enable memMan, usage fault and bus fault


	/*
	 * 3. configure the priority of system exception
	 */
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);


	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
}




/*
 * LOW LEVEL PERIPHERALS INITIALIZATION
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM6_CLK_ENABLE();

	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);
}

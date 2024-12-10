/*
 * msp.c
 *
 *  Created on: Feb 18, 2024
 *      Author: rezk
 */


#include "stm32f4xx_hal.h"



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
}




/*
 * LOW LEVEL PERIPHERALS INITIALIZATION
 */


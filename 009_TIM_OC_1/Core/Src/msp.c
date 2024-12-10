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
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{

	/*CH1 CH2 CH3 &CH4*/
	GPIO_InitTypeDef tim2ChxPin = {0};
	tim2ChxPin.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
	tim2ChxPin.Pull = GPIO_NOPULL;
	tim2ChxPin.Speed = GPIO_SPEED_LOW;
	tim2ChxPin.Mode = GPIO_MODE_AF_PP;
	tim2ChxPin.Alternate = GPIO_AF1_TIM2;

	HAL_GPIO_Init(GPIOA, &tim2ChxPin);

	__HAL_RCC_TIM2_CLK_ENABLE();

	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
}

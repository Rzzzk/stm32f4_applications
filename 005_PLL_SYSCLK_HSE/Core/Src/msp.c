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

/*
 * Low level UART2 initialization
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uert2;

	/*
	 * 1. Enable the peripheral clock
	 */
	__HAL_RCC_USART2_CLK_ENABLE();

	/*
	 * 2. Configure the pin muxing
	 */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio_uert2.Pin  = GPIO_PIN_2;
	gpio_uert2.Mode = GPIO_MODE_AF_PP;
	gpio_uert2.Pull = GPIO_PULLUP;
	gpio_uert2.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uert2.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &gpio_uert2);

	gpio_uert2.Pin  = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio_uert2);

	/*
	 * 3. Enable the IRQ and configure the priority (NVIC)
	 */
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}


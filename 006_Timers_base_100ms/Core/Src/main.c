/*
 * main.c
 *
 *  Created on: Feb 18, 2024
 *      Author: rezk
 */


#include "main.h"


void SystemClock_Config();


int main(void)
{

	/*
	 * 1. HAL Initializations
	 */
	HAL_Init();

	/*
	 * 2. System Clock Configurations
	 */
	SystemClock_Config();

	/*
	 * 3. Peripherals High Level Initializations
	 */

	while(1)
	{




	}



	return 0;
}

void SystemClock_Config()
{

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}







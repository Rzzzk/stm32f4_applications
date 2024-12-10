/*
 * main.c
 *
 *  Created on: Feb 18, 2024
 *      Author: rezk
 */


#include "stm32f4xx_hal.h"
#include "main.h"
#include <string.h>
#include <stdio.h>


#define TRUE      1
#define FALSE     0

void SystemClock_Config(void);
void Error_Handler(void);
void PrintRCCData(void);



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


	printf("Hello World!\n");

	/*
	 * 3. Peripherals High Level Initializations
	 */


	PrintRCCData();


	while(1)
	{

	}

	return 0;
}


void SystemClock_Config(void)
{

	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clock_init;

	memset(&osc_init,0,sizeof(osc_init));
	memset(&clock_init,0,sizeof(clock_init));


	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_ON;

	if (HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_Handler();
	}


	clock_init.ClockType  = RCC_CLOCKTYPE_SYSCLK |\
			RCC_CLOCKTYPE_HCLK   |\
			RCC_CLOCKTYPE_PCLK1  |\
			RCC_CLOCKTYPE_PCLK2;


	clock_init.SYSCLKSource   = RCC_SYSCLKSOURCE_HSE;
	clock_init.AHBCLKDivider  = RCC_SYSCLK_DIV2;
	clock_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clock_init.APB2CLKDivider = RCC_HCLK_DIV2;

	if ( HAL_RCC_ClockConfig(&clock_init, FLASH_ACR_LATENCY_0WS) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_RCC_HSI_DISABLE();

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}



/*
 * High level UART2 initialization
 */


void Error_Handler(void)
{
	while(1);
}


void PrintRCCData(void)
{
	char clock[100];

	memset(clock,0,sizeof(clock));
	sprintf(clock ,"SYSCLK : %ld  \r\n", HAL_RCC_GetSysClockFreq());


	memset(clock,0,sizeof(clock));
	sprintf(clock ,"HCLK   : %ld  \r\n", HAL_RCC_GetHCLKFreq());


	memset(clock,0,sizeof(clock));
	sprintf(clock ,"PCLK1  : %ld  \r\n", HAL_RCC_GetPCLK1Freq());


	memset(clock,0,sizeof(clock));
	sprintf(clock ,"PCLK2  : %ld  \r\n", HAL_RCC_GetPCLK2Freq());

}




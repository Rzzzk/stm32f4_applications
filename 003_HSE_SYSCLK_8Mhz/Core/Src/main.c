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
void UART2_Init(void);
void Error_Handler(void);
void UART2_PrintRCCData(void);



UART_HandleTypeDef huart2;
uint16_t len;


char *uartData = "The application is running!\r\n";
char *uartHAL_OK = "HAL_OK !\r\n";
char *uartHAL_NOT_OK = "HAL_NOT_OK !\r\n";


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
	UART2_Init();

	len = strlen(uartData);
	HAL_UART_Transmit(&huart2,(uint8_t *)uartData, len , HAL_MAX_DELAY);

	UART2_PrintRCCData();


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
	else
	{
		len = strlen(uartHAL_OK);
		HAL_UART_Transmit(&huart2,(uint8_t *)uartHAL_OK, len , HAL_MAX_DELAY);
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
void UART2_Init(void)
{

	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}

}

void Error_Handler(void)
{
	while(1);
}


void UART2_PrintRCCData(void)
{
	char clock[100];

	memset(clock,0,sizeof(clock));

	sprintf(clock ,"SYSCLK : %ld  \r\n", HAL_RCC_GetSysClockFreq());
	len = strlen(clock);
	HAL_UART_Transmit(&huart2,(uint8_t *)clock, len , HAL_MAX_DELAY);

	memset(clock,0,sizeof(clock));
	sprintf(clock ,"HCLK   : %ld  \r\n", HAL_RCC_GetHCLKFreq());
	len = strlen(clock);
	HAL_UART_Transmit(&huart2,(uint8_t *)clock, len , HAL_MAX_DELAY);

	memset(clock,0,sizeof(clock));
	sprintf(clock ,"PCLK1  : %ld  \r\n", HAL_RCC_GetPCLK1Freq());
	len = strlen(clock);
	HAL_UART_Transmit(&huart2,(uint8_t *)clock, len , HAL_MAX_DELAY);

	memset(clock,0,sizeof(clock));
	sprintf(clock ,"PCLK2  : %ld  \r\n", HAL_RCC_GetPCLK2Freq());
	len = strlen(clock);
	HAL_UART_Transmit(&huart2,(uint8_t *)clock, len , HAL_MAX_DELAY);
}




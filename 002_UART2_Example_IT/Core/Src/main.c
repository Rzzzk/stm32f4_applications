/*
 * main.c
 *
 *  Created on: Feb 18, 2024
 *      Author: rezk
 */


#include "stm32f4xx_hal.h"
#include "main.h"
#include <string.h>


#define TRUE      1
#define FALSE     0

void SystemClock_Config(void);
static void UART2_Init(void);
static void Error_Handler(void);


UART_HandleTypeDef huart2;

char *uartData = "The application is running!!!\r\n";
uint8_t convertToCapital(uint8_t data);


uint8_t  receivedData;
uint8_t  dataBuffer[100];
uint32_t counter = 0;

uint8_t aUART2_Flag = FALSE;






int main(void)
{

	HAL_Init();
	SystemClock_Config();
	UART2_Init();

	uint16_t len = strlen(uartData);

	if ( HAL_UART_Transmit(&huart2,(uint8_t *)uartData, len , HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}

	while(aUART2_Flag == FALSE)
	HAL_UART_Receive_IT(&huart2, &receivedData, 1);


	while(1)
	{




	}



	return 0;
}


void SystemClock_Config(void)
{

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

uint8_t convertToCapital(uint8_t data)
{
	if(data >= 'a' && data <= 'z')
	{
		data -= ('a' - 'A');
	}

	return data;
}


void Error_Handler(void)
{
	while(1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(receivedData == '\r')
	{

		aUART2_Flag = TRUE;


		dataBuffer[counter++] = '\r';
	    dataBuffer[counter++] = '\n';

	    HAL_UART_Transmit(&huart2,(uint8_t *)dataBuffer, counter , HAL_MAX_DELAY);

	}
	else
	{
		dataBuffer[counter++] = convertToCapital(receivedData);
	}
}








/*
 * main.c
 *
 *  Created on: Feb 18, 2024
 *      Author: rezk
 */


#include "main.h"


#define TRUE      1
#define FALSE     0


void SystemClock_Config(uint8_t sysClk);


void UART2_Init(void);
void Error_Handler(void);
void UART2_PintRCCData(void);



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
	SystemClock_Config(RCC_SYSCLK_84_MHZ);

	/*
	 * 3. Peripherals High Level Initializations
	 */
	UART2_Init();

	len = strlen(uartData);
	HAL_UART_Transmit(&huart2,(uint8_t *)uartData, len , HAL_MAX_DELAY);

	UART2_PintRCCData();


	while(1)
	{




	}



	return 0;
}

void SystemClock_Config(uint8_t sysClk)
{
	RCC_OscInitTypeDef Osc_init;
	RCC_ClkInitTypeDef clock_init;
	uint8_t FlashLatncy = 0;

	Osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	Osc_init.HSEState =RCC_HSE_BYPASS;

	Osc_init.PLL.PLLState =RCC_PLL_ON;
	Osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	switch(sysClk)
	{
	case RCC_SYSCLK_50_MHZ:
	{
		Osc_init.PLL.PLLM = 8;
		Osc_init.PLL.PLLN = 100;
		Osc_init.PLL.PLLP = 2;
		Osc_init.PLL.PLLQ = 2;

		clock_init.ClockType  = RCC_CLOCKTYPE_SYSCLK |\
				RCC_CLOCKTYPE_HCLK   |\
				RCC_CLOCKTYPE_PCLK1  |\
				RCC_CLOCKTYPE_PCLK2;


		clock_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

		clock_init.AHBCLKDivider  = RCC_SYSCLK_DIV1;

		clock_init.APB1CLKDivider = RCC_HCLK_DIV2;

		clock_init.APB2CLKDivider = RCC_HCLK_DIV2;

		FlashLatncy = FLASH_ACR_LATENCY_1WS;

		break;
	}
	case RCC_SYSCLK_84_MHZ:
	{
		Osc_init.PLL.PLLM = 8;
		Osc_init.PLL.PLLN = 168;
		Osc_init.PLL.PLLP = 2;
		Osc_init.PLL.PLLQ = 2;

		clock_init.ClockType  = RCC_CLOCKTYPE_SYSCLK |\
				RCC_CLOCKTYPE_HCLK   |\
				RCC_CLOCKTYPE_PCLK1  |\
				RCC_CLOCKTYPE_PCLK2;


		clock_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

		clock_init.AHBCLKDivider  = RCC_SYSCLK_DIV1;

		clock_init.APB1CLKDivider = RCC_HCLK_DIV2;

		clock_init.APB2CLKDivider = RCC_HCLK_DIV2;

		FlashLatncy = FLASH_ACR_LATENCY_2WS;


		break;

	}
	case RCC_SYSCLK_120_MHZ:
	{
		Osc_init.PLL.PLLM = 8;
		Osc_init.PLL.PLLN = 240;
		Osc_init.PLL.PLLP = 2;
		Osc_init.PLL.PLLQ = 2;

		clock_init.ClockType  = RCC_CLOCKTYPE_SYSCLK |\
				RCC_CLOCKTYPE_HCLK   |\
				RCC_CLOCKTYPE_PCLK1  |\
				RCC_CLOCKTYPE_PCLK2;


		clock_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

		clock_init.AHBCLKDivider  = RCC_SYSCLK_DIV1;

		clock_init.APB1CLKDivider = RCC_HCLK_DIV4;

		clock_init.APB2CLKDivider = RCC_HCLK_DIV2;

		FlashLatncy = FLASH_ACR_LATENCY_3WS;


		break;
	}
	default :
		break;
	}



	if (HAL_RCC_OscConfig(&Osc_init) != HAL_OK)
	{
		Error_Handler();
	}

	if ( HAL_RCC_ClockConfig(&clock_init,FlashLatncy) != HAL_OK)
	{
		Error_Handler();
	}


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
	len = strlen(uartHAL_NOT_OK);
	HAL_UART_Transmit(&huart2,(uint8_t *)uartHAL_NOT_OK, len , HAL_MAX_DELAY);
	while(1);
}


void UART2_PintRCCData(void)
{
	char clock[100];

	memset(&clock,0,sizeof(clock));

	sprintf(clock ,"SYSCLK : %ld  \r\n", HAL_RCC_GetSysClockFreq());
	len = strlen(clock);
	HAL_UART_Transmit(&huart2,(uint8_t *)clock, len , HAL_MAX_DELAY);


	sprintf(clock ,"HCLK   : %ld  \r\n", HAL_RCC_GetHCLKFreq());
	len = strlen(clock);
	HAL_UART_Transmit(&huart2,(uint8_t *)clock, len , HAL_MAX_DELAY);


	sprintf(clock ,"PCLK1  : %ld  \r\n", HAL_RCC_GetPCLK1Freq());
	len = strlen(clock);
	HAL_UART_Transmit(&huart2,(uint8_t *)clock, len , HAL_MAX_DELAY);

	sprintf(clock ,"PCLK2  : %ld  \r\n", HAL_RCC_GetPCLK2Freq());
	len = strlen(clock);
	HAL_UART_Transmit(&huart2,(uint8_t *)clock, len , HAL_MAX_DELAY);

}




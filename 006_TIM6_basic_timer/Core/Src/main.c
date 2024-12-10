/*
 * main.c
 *
 *  Created on: Feb 18, 2024
 *      Author: rezk
 */



#include "main.h"



#define TRUE      1
#define FALSE     0

void SystemClock_Config(void);
void PrintSystemClockInfo(void);

void TIMER6_Init(void);

void MAIN_Init(void);
void Error_Handler(void);
void LED_Init(void);

TIM_HandleTypeDef timer6_hn = {0};


/**********************************************  [main]  *****************************************/

int main(void)
{
	/*Application initializations*/
	MAIN_Init();

	HAL_TIM_Base_Start(&timer6_hn);

	while(1)
	{
		/*
		 * this is not good because the CPU does not go into sleep mode
		 * so, the solution is using interrupt mode not polling mode
		 */
		while( !(TIM6->SR & TIM_SR_UIF_Msk) ); //wait on the flag
		TIM6->SR &= ~(1 << 0); //clear the flag
		HAL_GPIO_TogglePin(RED_LED_PORT, RED_LED_PIN);
	}

	return 0;
}








/*************************************************************************************************/
void MAIN_Init(void)
{
	// HAL Initializations
	HAL_Init();

	// System Clock Configurations
	SystemClock_Config();

	// Peripherals High Level Initializations

	PrintSystemClockInfo();

	TIMER6_Init();

	LED_Init();
}


void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK |\
			RCC_CLOCKTYPE_HCLK   |\
			RCC_CLOCKTYPE_PCLK1  |\
			RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}



void PrintSystemClockInfo(void)
{
	char clock[100];

	memset(clock,0,sizeof(clock));
	sprintf(clock ,"SYSCLK : %ld  \r\n", HAL_RCC_GetSysClockFreq());
	printf("%s",clock);

	memset(clock,0,sizeof(clock));
	sprintf(clock ,"HCLK   : %ld  \r\n", HAL_RCC_GetHCLKFreq());
	printf("%s",clock);

	memset(clock,0,sizeof(clock));
	sprintf(clock ,"PCLK1  : %ld  \r\n", HAL_RCC_GetPCLK1Freq());
	printf("%s",clock);

	memset(clock,0,sizeof(clock));
	sprintf(clock ,"PCLK2  : %ld  \r\n", HAL_RCC_GetPCLK2Freq());
	printf("%s",clock);
}

void TIMER6_Init(void)
{
	timer6_hn.Instance = TIM6;

	timer6_hn.Init.CounterMode       = TIM_COUNTERMODE_UP;
	timer6_hn.Init.Prescaler         = 24;
	timer6_hn.Init.Period            = 64000-1;

	if(HAL_TIM_Base_Init(&timer6_hn) != HAL_OK)
	{
		Error_Handler();
	}
}


void LED_Init(void)
{
	GPIO_InitTypeDef led = {0};

	led.Mode = GPIO_MODE_OUTPUT_PP;
	led.Pin = RED_LED_PIN;
	led.Speed = GPIO_SPEED_FAST;
	led.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(RED_LED_PORT, &led);
}

void Error_Handler(void)
{
	while(1);
}




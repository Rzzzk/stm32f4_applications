/*
 * main.c
 *
 *  Created on: Feb 18, 2024
 *      Author: rezk
 */



#include "main.h"


void SystemClock_Config(void);
void PrintSystemClockInfo(void);

void MAIN_Init(void);
void LED_Init(void);

void TIMER2_Init(void);


void Error_Handler(void);


GPIO_InitTypeDef led = {0};
TIM_HandleTypeDef timer2_hn = {0};




/**********************************************  [main]  *****************************************/

int main(void)
{
	/*Application initializations*/
	MAIN_Init();




	while(1)
	{


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

	TIMER2_Init();

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
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;


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


void TIMER2_Init(void)
{
	TIM_OC_InitTypeDef timer2OC_cfg = {0};

	timer2_hn.Instance = TIM2;
	timer2_hn.Init.CounterMode = TIM_COUNTERMODE_UP;
	timer2_hn.Init.Period = 0xFFFFFFFF;
	timer2_hn.Init.Prescaler = 0;

	if(HAL_TIM_PWM_Init(&timer2_hn) != HAL_OK)
	{
		Error_Handler();
	}

	timer2OC_cfg.OCMode = TIM_OCMODE_PWM1;
	timer2OC_cfg.OCNPolarity = TIM_OCPOLARITY_HIGH; //non inverted

	//timer2OC_cfg.Pulse = ;

	if( HAL_TIM_PWM_ConfigChannel(&timer2_hn, &timer2OC_cfg, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

}



void LED_Init(void)
{
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

uint32_t compareRegValue;

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		compareRegValue = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, compareRegValue+pulseValueOC[0]);
	}

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		compareRegValue = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, compareRegValue+pulseValueOC[1]);
	}

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		compareRegValue = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, compareRegValue+pulseValueOC[2]);
	}

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		compareRegValue = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, compareRegValue+pulseValueOC[3]);
	}
}





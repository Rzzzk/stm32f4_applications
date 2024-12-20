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
void TIMER6_Init(void);

void Error_Handler(void);
void SystemClock_Config_2(uint8_t sysClk);

GPIO_InitTypeDef led = {0};
TIM_HandleTypeDef timer2_hn = {0};
TIM_HandleTypeDef timer6_hn = {0};


uint32_t capturedTickCount[3] = {0}, frequency=0;
uint8_t counter=1, ICCaptFlag=FALSE;

TIM_IC_InitTypeDef timer2IC_cfg = {0};

/**********************************************  [main]  *****************************************/

int main(void)
{
	uint32_t captDefference1 = 0;
	uint32_t captDefference2 = 0;

	double timer2CntFreq=0;
	double timer2Res=0;
	double userSignalPeriod=0;
	double userSignalfreq=0;
	double userSignalDuty=0;
	double userSignalTimeOn=0;

	/*Application initializations*/
	MAIN_Init();

	/*start timer 2 IC*/
	HAL_TIM_IC_Start_IT(&timer2_hn, TIM_CHANNEL_1);


	HAL_TIM_Base_Start_IT(&timer6_hn);

	//counter frequency
	timer2CntFreq = ((HAL_RCC_GetPCLK1Freq()*2)/(timer2_hn.Init.Prescaler+1));

	//tick time
	timer2Res = 1/timer2CntFreq;


	while(1)
	{
		if(ICCaptFlag)
		{
			ICCaptFlag = FALSE;

			/* get the tick count between two rising edge*/
			if(capturedTickCount[1] > capturedTickCount[0])
			{
				captDefference1 = capturedTickCount[1] - capturedTickCount[0];
			}
			else
			{
				captDefference1 = capturedTickCount[1] + (0xFFFFFFFF - capturedTickCount[0]);
			}

			/* get the tick count between second rising edge and the next falling*/
			if(capturedTickCount[2] > capturedTickCount[1])
			{
				captDefference2 = capturedTickCount[2] - capturedTickCount[1];
			}
			else
			{
				captDefference2 = capturedTickCount[2] + (0xFFFFFFFF - capturedTickCount[1]);

			}


			//period time = (tick count between two rising edge)*tick time
			userSignalPeriod = captDefference1 * timer2Res;

			//frequency = 1/period time
			userSignalfreq = 1/userSignalPeriod;


			//time ON = (tick count between second rising edge and the next falling)*tick time
			userSignalTimeOn = captDefference2 * timer2Res;


			// Duty cycle = time ON/period time
			userSignalDuty = (userSignalTimeOn/userSignalPeriod) * 100 ;


			printf("the signal frequency  : %lf Hz\n",userSignalfreq);
			printf("the signal period     : %lf s\n",userSignalPeriod);
			printf("the signal Duty Cycle : %lf \n",userSignalDuty);
			printf("------------------\n\n");

		}

	}

	return 0;
}

/*************************************************************************************************/
void MAIN_Init(void)
{
	// HAL Initializations
	HAL_Init();

	// System Clock Configurations
	SystemClock_Config_2(RCC_SYSCLK_120_MHZ);

	// Peripherals High Level Initializations
	PrintSystemClockInfo();

	TIMER2_Init();

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


void SystemClock_Config_2(uint8_t sysClk)
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


void LED_Init(void)
{
	led.Mode = GPIO_MODE_OUTPUT_PP;
	led.Pin = RED_LED_PIN;
	led.Speed = GPIO_SPEED_FAST;
	led.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(RED_LED_PORT, &led);
}


void TIMER2_Init(void)
{


	timer2_hn.Instance = TIM2;
	timer2_hn.Init.CounterMode = TIM_COUNTERMODE_UP;
	timer2_hn.Init.Period = 0xFFFFFFFF;
	timer2_hn.Init.Prescaler = 0;

	if(HAL_TIM_IC_Init(&timer2_hn) != HAL_OK)
	{
		Error_Handler();
	}

	timer2IC_cfg.ICPolarity  = TIM_ICPOLARITY_RISING;
	timer2IC_cfg.ICPrescaler = TIM_ICPSC_DIV1;
	timer2IC_cfg.ICFilter    = 0;
	timer2IC_cfg.ICSelection = TIM_ICSELECTION_DIRECTTI;

	if( HAL_TIM_IC_ConfigChannel(&timer2_hn, &timer2IC_cfg, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

}


void TIMER6_Init(void)
{
	timer6_hn.Instance = TIM6;

	timer6_hn.Init.CounterMode       = TIM_COUNTERMODE_UP;
	timer6_hn.Init.Prescaler         = 15999;
	timer6_hn.Init.Period            = 50-1;

	if(HAL_TIM_Base_Init(&timer6_hn) != HAL_OK)
	{
		Error_Handler();
	}
}


void Error_Handler(void)
{
	while(1);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	if(counter == 1)
	{
		/*get tick count at first rising*/
		capturedTickCount[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);

		/*increment the counter*/
		++counter;

	}
	else if(counter == 2)
	{
		/*get tick count at second rising*/
		capturedTickCount[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);



		/*change the edge to falling edge*/
		timer2IC_cfg.ICPolarity  = TIM_ICPOLARITY_FALLING;
		timer2IC_cfg.ICPrescaler = TIM_ICPSC_DIV1;
		timer2IC_cfg.ICFilter    = 0;
		timer2IC_cfg.ICSelection = TIM_ICSELECTION_DIRECTTI;

		HAL_TIM_IC_ConfigChannel(&timer2_hn, &timer2IC_cfg, TIM_CHANNEL_1);


		/*increment the counter*/
		++counter;

	}
	else if(counter == 3)
	{
		/*get tick count at falling*/
		capturedTickCount[2] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);

		/*change the edge to rising edge*/
		timer2IC_cfg.ICPolarity  = TIM_ICPOLARITY_RISING;

		HAL_TIM_IC_ConfigChannel(&timer2_hn, &timer2IC_cfg, TIM_CHANNEL_1);

		/*reset the counter*/
		counter = 1;

		/*set the capture flag*/
		ICCaptFlag = TRUE;
	}
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(RED_LED_PORT, led.Pin);
}



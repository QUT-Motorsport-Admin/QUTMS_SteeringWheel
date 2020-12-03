/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "Lcd/stm32_adafruit_lcd.h"
#include "Lcd/lcd.h"
#include "QUTMS.h"
#include "stdbool.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void logo()
{
	BSP_LCD_DrawRGB16Image(0, 120, QUTMS.width, QUTMS.height, (uint16_t*)QUTMS.pixel_data);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	rtd = false;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	/** Create CAN Filter & Apply it to &CANBUS4, &CANBUS2 */
	CAN_FilterTypeDef  sFilterConfig;

	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0001;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_CAN_Start(&hcan) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		Error_Handler();
	}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	uint8_t LCD_Status = BSP_LCD_Init();
	//	HAL_Delay(100);
	BSP_LCD_DisplayOff();
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	//	logo();
	BSP_LCD_DisplayOn();

	int i = 0;
	char* msg[9][3] = {
			{
					{" Fear it, run from it "},
					{"    ROBOTEQ arrives   "},
					{"     all the same     "}

			},
			{
					{"                      "},
					{"       Vry Nice       "},
					{"                      "}
			},
			{
					{"                      "},
					{"    #RacingForTom     "},
					{"                      "}
			},
			{
					{"      Jesus Take      "},
					{"      The Wheel      "},
					{"      Literally      "}
			},
			{
					{"       Drive it       "},
					{"       Like You       "},
					{"       Stole it       "}
			},
			{
					{" Praise be the elders "},
					{"  Sam of Current and  "},
					{"    Isaac of Beard    "}
			},
			{
					{"  Imagine not having  "},
					{"     10PM access      "},
					{"                      "}
			},
			{
					{"  Don't Mean to Flex  "},
					{"   But We Have 300    "},
					{"Followers on LinkedIn "}
			},
			{
					{"                      "},
					{"     QEV3 - LANDO     "},
					{"                      "}
			}


	};

	int ctr = 0;
	rtd = true;
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	BSP_LCD_DrawHLine(0, 415, BSP_LCD_GetYSize());
	BSP_LCD_DrawRect(105, 0, 320, 138);

	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_DisplayStringAt(10, 57, "VOLTS", LEFT_MODE);
	BSP_LCD_DisplayStringAt(10, 57 + 138, "IN. C", LEFT_MODE);
	BSP_LCD_DisplayStringAt(10, 57 + 138 * 2, "CC", LEFT_MODE);
	BSP_LCD_DrawRect(105, 137, 320, 138);
	BSP_LCD_DrawRect(105, 274, 320, 138);
	BSP_LCD_FillRect(105, 411, 320, 4);
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(rtd == false)
		{
			BSP_LCD_Scroll(i--, 1, 1);
			HAL_Delay(12);
		} else {

			ctr++;
			BSP_LCD_SetFont(&Font20);
			BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			if(ctr % 100000 == 0)
			{
				int val = rand() % 9;
				BSP_LCD_DisplayStringAtLine(21, msg[val][0]);
				BSP_LCD_DisplayStringAtLine(22, msg[val][1]);
				BSP_LCD_DisplayStringAtLine(23, msg[val][2]);
			}
			if(ctr % 5000 == 0)
			{
				char x[10];
				int len = snprintf(x, 10, "%li", rand() % 120);
				BSP_LCD_DisplayStringAt(215/2 + 105 - (strlen(x)/2 * 17), 57, x, LEFT_MODE);
				len = snprintf(x, 10, "%li", rand() % 130);
				BSP_LCD_DisplayStringAt(215/2 + 105 - (strlen(x)/2 * 17), 57 + 138, x, LEFT_MODE);
				len = snprintf(x, 10, "%f", ((float)ctr) / 321566.0f);
				BSP_LCD_DisplayStringAt(215/2 + 105 - (strlen(x)/2 * 17), 57 + 138 * 2, x, LEFT_MODE);
			}
		}

	}
	//		BSP_LCD_Scroll(i--, 1, 1);
	//		HAL_Delay(12);
	//	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

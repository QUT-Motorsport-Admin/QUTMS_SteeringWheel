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

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	HAL_Delay(500);
	uint8_t LCD_Status = BSP_LCD_Init();
	HAL_Delay(500);
	uint8_t buffer[30];
	sprintf(buffer, "Status = %X\r\n", LCD_Status);
	HAL_UART_Transmit(&huart1, buffer, sizeof(buffer), HAL_MAX_DELAY);
	HAL_Delay(100);
	uint8_t LCD_COLOUR =BSP_LCD_GetBackColor();
	sprintf(buffer, "COLOUR = %X\r\n", LCD_COLOUR);
	HAL_UART_Transmit(&huart1, buffer, sizeof(buffer), HAL_MAX_DELAY);
	BSP_LCD_Clear(LCD_COLOR_BLUE);
	BSP_LCD_SetBackColor(LCD_COLOR_RED);

		//VAR Default Sets
		int LAPCALC = 1;
		int BATTPERC = 75;
		int RACECALC = 5;
		char TRQCODE = '2';
		char RGNCODE = '50';
		int DRVMODE = 1;
		char DRVMODES [3] = {"START", "RACE - DRY", "RACE - WET" };
		int ERRORCODE = 10;
		char ERRORCODES[10] = {"CHASSIS CONTROLER ERROR", "PDM ERROR", "BMS ERROR", "AMS ERROR" "INVERTER ERROR",
				"HV ERROR", "LIMP MODE","INTIALISATION ERROR"  "UNKOWN ERROR", "NOMINAL"};





		//OUTER BOXES
		BSP_LCD_DrawRect(60 , 70 , 40 , 220 ); 						//LAP OUTER BOX
		BSP_LCD_DrawRect(140, 70 , 40 , 220 ); 						//BATT OUTER BOX
		BSP_LCD_DrawRect(240, 70 , 40 , 220 ); 						//RACE OUTER BOX

		//TEXT
		BSP_LCD_SetFont(&Font24);
		BSP_LCD_DisplayStringAt(60 , 40, "LAP" , LEFT_MODE); 		//LAP TEXT
		BSP_LCD_DisplayStringAt(140, 40, "BATT", LEFT_MODE);		//BATT TEXT
		BSP_LCD_DisplayStringAt(240, 40, "RACE", LEFT_MODE);		//RACE TEXT

		//AUX
		BSP_LCD_DrawRect(60 , 180 , 220 , 1 ); 						//MID LINE (ZEROING LINE)

		//VALUE BOXES / BARS
		BSP_LCD_DrawRect(61 , 180, 38 , LAPCALC ); 													//LAP BATT DELTA CALC (MID BASED)
		BSP_LCD_DrawRect(141, (220 - (((100 - BATTPERC)/100) * 220)), 38 , (((100 - BATTPERC)/100) * 220));			//BATT PERCENT (BOTTOM BASED) CHECK BATT PERC VALUE (% OR /100 )
		BSP_LCD_DrawRect(241, 180, 38 , RACECALC);													//RACE BATT DELTA CALC (MID BASED)

		//LOWER SETTINGS BOXES
		BSP_LCD_DrawRect(60 , 320, 90  , 65);						//TRQ VECT MODE BOX
		BSP_LCD_DrawRect(180, 320, 90  , 65);						//RGN BRK MODE BOX
		BSP_LCD_DrawRect(100, 390, 130 , 40);						//DRIVE MODE BOX
		BSP_LCD_DrawRect(100, 440, 130 , 40);						//ERROR CODE BOX - NOT IN INITIAL DESIGN

		//LOWER SETTINGS HEADER TEXT
		BSP_LCD_SetFont(&Font16);
		BSP_LCD_DisplayStringAt(65  ,325,"TRQ VEC", LEFT_MODE);		//TRQ VEC TEXT
		BSP_LCD_DisplayStringAt(185 ,325,"RGN BRK", LEFT_MODE);		//RGN BRK TEXT


		//LOWER SETTINGS MODE TEXT
		BSP_LCD_DisplayStringAt(65 , 350, "TRQCODE", CENTER_MODE); 		//TRQ VEC CODE - REPLACE "TRQCODE" WITH CORRISPONDING VARIABLE
		BSP_LCD_DisplayStringAt(185, 350, "RGNCODE+%", LEFT_MODE);	//RBN BRK CODE - REPLACE "RGNCODE" WITH CORRISPONDING VARIABLE
		BSP_LCD_DisplayStringAt(120, 400, DRVMODES[DRVMODE], LEFT_MODE);		//DRV MODE - REPLACE "DRVMODE" WITH CORRISPONDING VARIABLE
		BSP_LCD_DisplayStringAt(120, 4550, ERRORCODES[ERRORCODE],LEFT_MODE);		//ERROR CODE DISPLAY - REPLACE "ERRORCODE" WITH CORRISPONDING VARIABLE



 		//OLD
	//BSP_LCD_SetBackColor(RC(0x001F));
	//BSP_LCD_DrawRect(10,10,10,10);
	//BSP_LCD_DrawCircle(10,10,10);

	//HAL_Delay(1000);

	//BSP_LCD_SetFont(&Font24);
	//BSP_LCD_DisplayStringAt(20,20, "James", CENTER_MODE);
	//BSP_LCD_SetFont(&Font16);
	//BSP_LCD_DisplayStringAt(0,70, "test123", LEFT_MODE);

	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_Delay(1000);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
		HAL_Delay(1000);


	}
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

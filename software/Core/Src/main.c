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
#include "stm32_adafruit_lcd.h"
#include "lcd.h"
#include "QUTMS.h"
#include "stdbool.h"
#include "gui.h"

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

uint8_t selected_menu_option;
uint8_t max_menu_option;
UI_Screen current_screen = CAR_CONFIGURATION_SCREEN;
float accumul_volts = 20, accumul_temp = 40, gearbox_temp = 40, inverter_temp = 20,
	  motor_temp = 49, accumul_charge = 0, accumul_delta = 1;
uint8_t total_laps, current_lap;
Drive_Mode current_drive_mode = STATIC_MODE;
extern Driver_Profile drivers[4];
Driver_Profile current_driver;
extern Event_Profile events[4];
Event_Profile current_event;

// Input Controls
volatile bool activate_btn_pressed = false;
volatile uint8_t active_btn_state = 0;
volatile bool back_btn_pressed = false;
volatile uint8_t back_btn_state = 0;
uint16_t raw;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void logo() {
	BSP_LCD_DrawRGB16Image(0, 120, QUTMS.width, QUTMS.height,
			(uint16_t*) QUTMS.pixel_data);
}
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
	/** Create CAN Filter & Apply it to &CANBUS4, &CANBUS2 */
	CAN_FilterTypeDef sFilterConfig;

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

	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_CAN_Start(&hcan) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING)
			!= HAL_OK) {
		Error_Handler();
	}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	BSP_LCD_Init();
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	//drawStartupScreen();
	//HAL_Delay(1000);

	// Show first screen
	drawScreen(current_screen);
  while (1)
  {
	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */

	// TODO Update state off CAN messages

	// Get ADC value
	//HAL_ADC_Start(&hadc1);
	//HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	//raw = HAL_ADC_GetValue(&hadc1);

	  if(!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5))
	  {
		  activate_btn_pressed = true;
	  }

	  if(!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4))
	  {
		  back_btn_pressed = true;
	  }

	accumul_delta -= 0.05f;

	// Update screen
	if (current_screen == RTD_SCREEN) {
		// Update screen
		updateRTDScreen();
	}
	else if (current_screen == SM_SCREEN) {
		// Update screen
		//updateSMScreen();

		// Handle navigation
		updateMenuScroll();

		// Handle activate of settings
		if (activate_btn_pressed) {
			switch (selected_menu_option) {
			case 0:
				drawScreen(DRIVER_SELECTION_SCREEN);
				break;
			case 1:
				drawScreen(EVENT_SELECTION_SCREEN);
				break;
			case 2:
				drawScreen(CAR_CONFIGURATION_SCREEN);
				break;
			case 3:
				//drawAdvancedScreen();
				break;
			}
			activate_btn_pressed = false;
		}
	}
	else if (current_screen == DRIVER_SELECTION_SCREEN) {
		// Update screen
		updateDriverSelectionScreen();

		// Handle navigation
		updateMenuScroll();

		// Handle activate of settings
		if (activate_btn_pressed) {
			changeDriver(drivers[selected_menu_option]);
		}

		if (back_btn_pressed) {
			drawScreen(SM_SCREEN);
			back_btn_pressed = false;
		}
	}
	else if (current_screen == EVENT_SELECTION_SCREEN) {
		// Update screen
		updateEventSelectionScreen();

		// Handle navigation
		updateMenuScroll();

		// Handle activate of settings
		if (activate_btn_pressed) {
			changeEvent(events[selected_menu_option]);
			activate_btn_pressed = false;
		}

		if (back_btn_pressed) {
			drawScreen(SM_SCREEN);
			back_btn_pressed = false;
		}
	}
	else if (current_screen == CAR_CONFIGURATION_SCREEN) {
		// Update screen
		updateCarConfigurationScreen();

		// Handle navigation
		updateMenuScroll();

		// Handle activate/value change of settings
//        if(pot_incremented){
//        	update_car_configuration();
//			activate_btn_pressed = false;
//		}

		if (back_btn_pressed) {
			drawScreen(SM_SCREEN);
			back_btn_pressed = false;
		}

	}

	  /* Update can_log file */
//      string can_log_buffer;
//      while (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) > 0){
//    	  CAN_MSG_Generic_t msg;
//    	  HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &(msg.header), msg.data);
//    	  can_log_buffer += /*msg.header + " " + */ msg.data + "\n";
//      }
//      update_sd_file("can_log.txt", can_log_buffer);

	  // Handle back button

	  HAL_Delay(200);

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

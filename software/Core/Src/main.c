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
#include <string.h>
#include "../lvgl/lvgl.h"

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
// Car configuration
Drive_Mode current_drive_mode = STATIC_MODE;
extern Event_Profile current_event;
uint8_t selected_menu_option, max_menu_option;
UI_Screen current_screen = SM_SCREEN;
float accumul_volts = 50.0f, accumul_temp = 40.0f, gearbox_temp = 1.0f,
		inverter_temp = 20.0f, motor_temp = 49.0f, accumul_charge = 0.5f,
		accumul_delta = 0.0f;
float drawn_accumul_volts = 0.0f, drawn_accumul_temp = 0.0f,
		drawn_gearbox_temp = 0.0f, drawn_inverter_temp = 0.0f,
		drawn_motor_temp = 0.0f, drawn_accumul_charge = 0.0f,
		drawn_accumul_delta = 0.0f;
uint8_t total_laps = 0, current_lap = 1;
extern Driver_Profile drivers[4], current_driver;
extern Event_Profile events[4];

// Inputs
volatile bool activate_btn_pressed, back_btn_pressed;
volatile uint8_t active_btn_state = 0, back_btn_state = 0;
extern bool menu_pot_incremented, menu_pot_decremented, menu_pot_pressed;
uint8_t prev_pot_value, current_pot_value;
uint16_t raw;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
void my_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

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

  /* Initialise graphics library----------------------------------------------*/
  lv_init();

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

  /* Initialize interrupts */
  MX_NVIC_Init();
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
	

//	drawStartupScreen();
//	HAL_Delay(2000);

	/*******************************
     *    DISPLAY DRIVERS DEMO     *
     ******************************/ 

    /*A static or global variable to store the buffers*/
    static lv_disp_buf_t disp_buf;

    /*Static or global buffer(s). The second buffer is optional*/
    static lv_color_t buf_1[320 * 10];
    //static lv_color_t buf_2[320];

    /*Initialize `disp_buf` with the buffer(s) */
    lv_disp_buf_init(&disp_buf, buf_1, NULL, 320 * 5);


    lv_disp_drv_t disp_drv;                 /*A variable to hold the drivers. Can be local variable*/
    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
    disp_drv.buffer = &disp_buf;            /*Set an initialized buffer*/
    disp_drv.flush_cb = my_flush_cb;        /*Set a flush callback to draw to the display*/
    lv_disp_t * disp;
    disp = lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/

	screen_create();

	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    lv_task_handler();
	
		// // TODO Update state off CAN messages
		// // Get ADC value
		// //HAL_ADC_Start(&hadc1);
		// //HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		// //raw = HAL_ADC_GetValue(&hadc1);
		// // Check if activate button was pressed
		// if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)) {
		// 	activate_btn_pressed = true;
		// }

		// // Check if back button was pressed
		// if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)) {
		// 	back_btn_pressed = true;
		// }

		// // Check if menu pot changed
		// if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)) {
		// 	menu_pot_pressed = true;
		// }



		// // Update screen
		// if (current_screen == RTD_SCREEN) {
		// 	updateRTDScreen();
		// } else if (current_screen == SM_SCREEN) {
		// 	updateSMScreen();

		// 	if(menu_pot_incremented || menu_pot_pressed){
		// 		BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
		// 		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		// 		BSP_LCD_FillRect(0, 0, 100, 100);
		// 	}

		// 	if (activate_btn_pressed) {
		// 		switch (selected_menu_option) {
		// 		case 0:
		// 			drawScreen(DRIVER_SELECTION_SCREEN);
		// 			break;
		// 		case 1:
		// 			drawScreen(EVENT_SELECTION_SCREEN);
		// 			break;
		// 		case 2:
		// 			drawScreen(CAR_CONFIGURATION_SCREEN);
		// 			break;
		// 		}
		// 	}
		// } else if (current_screen == DRIVER_SELECTION_SCREEN) {
		// 	updateDriverSelectionScreen();

		// 	if (activate_btn_pressed) {
		// 		changeDriver(drivers[selected_menu_option]);
		// 	}

		// 	if (back_btn_pressed) {
		// 		drawScreen(SM_SCREEN);
		// 	}
		// } else if (current_screen == EVENT_SELECTION_SCREEN) {
		// 	updateEventSelectionScreen();

		// 	if (activate_btn_pressed) {
		// 		changeEvent(events[selected_menu_option]);
		// 	}

		// 	if (back_btn_pressed) {
		// 		drawScreen(SM_SCREEN);
		// 	}
		// } else if (current_screen == CAR_CONFIGURATION_SCREEN) {
		// 	updateCarConfigurationScreen();

		// 	// Handle activate/value change of settings
		// 	if (menu_pot_pressed) {
		// 		updateCarConfiguration();
		// 	}

		// 	if (back_btn_pressed) {
		// 		drawScreen(SM_SCREEN);
		// 	}
		// }

		// // Reset inputs
		// back_btn_pressed = false;
		// activate_btn_pressed = false;
		// menu_pot_pressed = false;

		// // Refresh Delay
		// HAL_Delay(100);
	}
  /* USER CODE END 3 */
}

void my_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/
    int32_t x, y;
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            ili9488_WritePixel(x, y, *color_p);
            color_p++;
        }
    }

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
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

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* EXTI1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	// When pulse A goes low, is pulse b low or high, if its high, then its a clockwise movement otherwise it is anti clockwise


//	if(GPIO_Pin == 1){
//		BSP_LCD_SetBackColor(LCD_COLOR_RED);
//		BSP_LCD_SetTextColor(LCD_COLOR_RED);
//		BSP_LCD_FillRect(0, 0, 100, 100);
//	}
//	else if(GPIO_Pin == 2){
//		BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
//		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
//		BSP_LCD_FillRect(0, 0, 100, 100);
//	}
}


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

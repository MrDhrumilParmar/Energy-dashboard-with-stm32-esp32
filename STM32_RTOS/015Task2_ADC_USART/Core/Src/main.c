/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdlib.h>
#include <stdio.h>
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#define F_REF				60.0f

// 	USE ANYONE AT A TIME
//	SEGGER		:	1
// 	Tera Term 	:	0

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define DWT_CTRL        (*(volatile uint32_t*)0XE0001000)
#define MAX_LEN		32
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
TaskHandle_t task1_handle; // Gen task 1
TaskHandle_t task2_handle; // Gen task 2
TaskHandle_t task3_handle; // Parameter calc from ADC Handler
TaskHandle_t task4_handle; // Print on uart handler
QueueHandle_t q_print;

ADC_Struct_t adc;

int cnt = 0, btn_pressed = 0;
volatile int adc_isr_cnt = 0;
volatile float power = 0;
volatile float vrms = 0;
volatile float irms = 0;
volatile float temperature = 0;
volatile uint32_t power_int = 0;
volatile uint32_t vrms_int = 0;
volatile uint32_t irms_int = 0;
volatile uint32_t temperature_int = 0;

char *str_g = "\n";	//"860\n";
float ang = 0, ang2 = 0;
int32_t val = 0, val2 = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

static void LED_Green_Handler(void *parameters);
static void LED_Red_Handler(void *parameters);
static void Para_Calc_Handler(void *param);
static void Print_Handler(void *param);

extern void SEGGER_UART_init(uint32_t);

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

	BaseType_t status;

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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

	//***********************************************************************
	//							RTOS Init
	//***********************************************************************
	{

		// Enable the CYCCNT counter
		DWT_CTRL |= (1 << 0);
	}

	//***********************************************************************
	//					RTOS : Task Creation/ Queue define
	//***********************************************************************
	{
		//<-----------------------Task Creation---------------------->
		status = xTaskCreate(LED_Green_Handler, "LED_Green_Task", 200, NULL, 2,
				&task1_handle);
		configASSERT(status == pdPASS);

		status = xTaskCreate(LED_Red_Handler, "LED_Red_Task", 200, NULL, 2,
				&task2_handle);
		configASSERT(status == pdPASS);

		status = xTaskCreate(Para_Calc_Handler, "Para_Calc_Task", 500, NULL, 2,
				&task3_handle);
		configASSERT(status == pdPASS);

		//Printing On UART 1: ESP32
		status = xTaskCreate(Print_Handler, "Print_Task", 500, NULL, 2,
				&task4_handle);
		configASSERT(status == pdPASS);

		//<-----------------------Queue Creation---------------------->

		//		q_adc = xQueueCreate(1, sizeof(ADC_Struct_t));

		q_print = xQueueCreate(8, MAX_LEN);
		configASSERT(q_print != NULL);

	}
	//***********************************************************************
	//							 HAL Init
	//***********************************************************************
	{
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*) (adc.ADC1_Val), 4);
		HAL_ADC_Start_DMA(&hadc2, (uint32_t*) (adc.ADC2_Val), 2);
		//		HAL_ADC_Start_DMA(&hadc3, (uint32_t*) &(adc.ADC3_Val), 8);

//		HAL_UART_Receive_IT(&huart2, (uint8_t*)&user_data, 1);
	}
	//***********************************************************************
	//							RTOS : Task Scheduler Starts
	//***********************************************************************
	{
		//<-----------------------Task Scheduler Starts---------------------->
		vTaskStartScheduler();
	}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void LED_Green_Handler(void *param) {

	char tx[MAX_LEN];
	int counter = 0;

	while (1) {
		ang += 0.3f;
		if (ang >= 6.27f) {
			ang = 0;
		}
		val = (int32_t) (1000.0f * sin(ang));
		// convert integer to string safely
		snprintf(tx, sizeof(tx), "SV:%d\r\n", val);

		xQueueSend(q_print, &tx, portMAX_DELAY);

//		sprintf(str_green, "%03d\n", (uint32_t) (val * 10.0f));
//		sprintf((char*) str_g, "%03.0f\n", val); // /* , |%04.0f : sysVar.nMotor*/

//		xQueueSend(q_print, &buffer, portMAX_DELAY);

		vTaskDelay(pdMS_TO_TICKS(50));
	}

}

void LED_Red_Handler(void *param) {
	char tx2[MAX_LEN];
	while (1) {
		ang2 += 0.3f;
		if (ang2 >= 6.27f) {
			ang2 = 0;
		}
		val2 = (int32_t) (1000.0f * sin(ang2 + 0.1f));
		// convert integer to string safely
		snprintf(tx2, sizeof(tx2), "SI:%d\r\n", val2);

		xQueueSend(q_print, &tx2, portMAX_DELAY);
		vTaskDelay(pdMS_TO_TICKS(50));
	}

}
//void LED_Red_Handler(void *param) {
//	const char *str = "102\n";
//	while (1) {
//
////		HAL_GPIO_TogglePin(LEDRed_GPIO_Port, LEDRed_Pin);
////		Print_Handler(str);
//
//		xQueueSend(q_print, &str, portMAX_DELAY);
//		vTaskDelay(pdMS_TO_TICKS(1000));
//	}
//
//}

void Print_Handler(void *param) {
	char rx[MAX_LEN] = "start\n";
	while (1) {

		xQueueReceive(q_print, rx, portMAX_DELAY);
		size_t len = strnlen(rx, MAX_LEN); // safe check
		if (len > 0) {
			HAL_UART_Transmit(&huart2, (uint8_t*) rx, len,
			HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart1, (uint8_t*) rx, len,
			HAL_MAX_DELAY);
		}
	}

}
// Backup
//void Print_Handler(void *param) {
//	char *print_str = "start\n";
//	while (1) {
//
//		xQueueReceive(q_print, (void*) &print_str, portMAX_DELAY);
//		HAL_UART_Transmit(&huart2, (uint8_t*) print_str,
//				strlen((char*) print_str), HAL_MAX_DELAY);
//	}
//
//}

static void Para_Calc_Handler(void *param) {
	char tx3[MAX_LEN];
	while (1) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		vrms = (float) (adc.ADC1_Val[0]) * 500.0f / 4096.0f;
		irms = (float) (adc.ADC1_Val[1]) * 20.0f / 4096.0f;
		power = vrms * irms;
		temperature = (float) (adc.ADC2_Val[0]) * 200.0f / 4096.0f;
		vrms_int = (uint32_t) vrms;
		irms_int = (uint32_t) irms;
		power_int = (uint32_t) power;
		temperature_int = (uint32_t) temperature;

		// convert integer to string safely
		snprintf(tx3, sizeof(tx3), "NV:%d %d \n", vrms_int, irms_int);
		xQueueSend(q_print, &tx3, portMAX_DELAY);
		snprintf(tx3, sizeof(tx3), "NPT:%d %d \n", power_int, temperature_int);
		xQueueSend(q_print, &tx3, portMAX_DELAY);

	}
}
//***********************************************************************
//								ISR
//***********************************************************************
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	adc_isr_cnt++;
	if (adc_isr_cnt > 1000) {
		adc_isr_cnt = 0;
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(task3_handle, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

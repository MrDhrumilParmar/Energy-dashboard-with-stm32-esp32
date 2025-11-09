#define _PROCESS_

#include <main.h>
#include <math.h>
#include <process.h>

BaseType_t status;

TaskHandle_t task1_handle; // Gen task 1
TaskHandle_t task2_handle; // Gen task 2
TaskHandle_t task3_handle; // Parameter calc from ADC Handler
TaskHandle_t task4_handle; // Print on uart handler
QueueHandle_t q_print;

int cnt = 0, btn_pressed = 0;
int adc_isr_cnt = 0;
float power = 0;
float vrms = 0;
float irms = 0;
float temperature = 0;
uint32_t power_int = 0;
uint32_t vrms_int = 0;
uint32_t irms_int = 0;
uint32_t temperature_int = 0;

char *str_g = "\n";	//"860\n";
float ang = 0, ang2 = 0;
int32_t val = 0, val2 = 0;

Adc_Buffer_Struct adc;

void parameterCalculations(void) {
}
void setInitVariable(void) {
}
void RTOSOneTimeInit(void) {
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
}

void ProcessOneTimeInit(void) {

	//***********************************************************************
	//							 One Time Init
	//***********************************************************************
	{
		adcBufferInit(&adc);
	}
	//***********************************************************************
	//							 HAL Init
	//***********************************************************************
	{
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*) (adc.ADC1_Val), ADC1_BUFF_SIZE);
		HAL_ADC_Start_DMA(&hadc2, (uint32_t*) (adc.ADC2_Val), ADC2_BUFF_SIZE);

		//		HAL_ADC_Start_DMA(&hadc3, (uint32_t*) &(adc.ADC3_Val), 8);

		//		HAL_UART_Receive_IT(&huart2, (uint8_t*)&user_data, 1);
	}
}

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

	// --------------------------------------------------------------------------
	//                  ADC Avg Value Calculation with Averaging
	// --------------------------------------------------------------------------
	{
		paraCalcAdc1(&adc);
		paraCalcAdc2(&adc);
	}

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	vTaskNotifyGiveFromISR(task3_handle, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}


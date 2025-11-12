#define _PROCESS_

#include <main.h>
#include <math.h>
#include <process.h>

BaseType_t status;

TaskHandle_t task1_handle; // Gen task 1
TaskHandle_t task2_handle; // Gen task 2
TaskHandle_t ParaCalcTaskHandle; // Parameter calc from ADC Handler
TaskHandle_t ParaCalcTaskHandle; // Parameter calc from ADC Handler
TaskHandle_t task4_handle; // Print on uart handler
QueueHandle_t q_print;

Motor_t m1, m2, m3;
Fault_e fault_no;

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
		status = xTaskCreate(Para_Calc_Handler, "Para_Calc_Task", 500, NULL, 2,
				&ParaCalcTaskHandle);
		configASSERT(status == pdPASS);
		status = xTaskCreate(Fault_Task_Handler, "Fault_Task", 500, NULL, 2,
				&FaultTaskHandle);
		configASSERT(status == pdPASS);
		status = xTaskCreate(Supervisor_Task_Handler, "HeartBeat_Task", 500, NULL, 2,
				&SupervisorTaskHandle);
		configASSERT(status == pdPASS);



		status = xTaskCreate(LED_Green_Handler, "LED_Green_Task", 200, NULL, 2,
				&task1_handle);
		configASSERT(status == pdPASS);

		status = xTaskCreate(LED_Red_Handler, "LED_Red_Task", 200, NULL, 2,
				&task2_handle);
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
		m1.Vdc = (float) (adc.ADC_AVG[0]) * 500.0f / 4096.0f;
		m1.Idc = (float) (adc.ADC_AVG[1]) * 500.0f / 4096.0f;
		m1.Temprature = (float) (adc.ADC_AVG[2]) * 500.0f / 4096.0f;
		m1.Pow = motor.m1.Vdc * motor.m1.Idc;

		m2.Vdc = (float) (adc.ADC_AVG[3]) * 500.0f / 4096.0f;
		m2.Idc = (float) (adc.ADC_AVG[4]) * 500.0f / 4096.0f;
		m2.Temprature = (float) (adc.ADC_AVG[5]) * 500.0f / 4096.0f;
		m2.Pow = motor.m1.Vdc * motor.m1.Idc;

		m3.Vdc = (float) (adc.ADC_AVG[6]) * 500.0f / 4096.0f;
		m3.Idc = (float) (adc.ADC_AVG[7]) * 500.0f / 4096.0f;
		m3.Temprature = (float) (adc.ADC_AVG[8]) * 500.0f / 4096.0f;
		m3.Pow = motor.m1.Vdc * motor.m1.Idc;

//		vrms = (float) (adc.ADC1_Val[0]) * 500.0f / 4096.0f;
//		irms = (float) (adc.ADC1_Val[1]) * 20.0f / 4096.0f;
//		power = vrms * irms;
//		temperature = (float) (adc.ADC2_Val[0]) * 200.0f / 4096.0f;
//		vrms_int = (uint32_t) vrms;
//		irms_int = (uint32_t) irms;
//		power_int = (uint32_t) power;
//		temperature_int = (uint32_t) temperature;

		// Fault Checking

		fault_no = checkMotorPara(&m1);
		if (fault_no != HEALTHY) {

		}
		fault_no = checkMotorPara(&m2);
		if (fault_no != HEALTHY) {
			// FAULT OCCURED
		}
		fault_no = checkMotorPara(&m3);
		if (fault_no != HEALTHY) {
			// FAULT OCCURED
		}

		// command to stop immediately
		Command_t cmd = { .code = CMD_STOP, .param = 0 };
		xQueueSend(commandQueue, &cmd, 0);

		// Example threshold logic
		if (ch_avg[0] > OVERCURRENT_THRESHOLD) {
			xEventGroupSetBits(eventGroupHandle, FAULT_OVERCURRENT);
		} else {
			xEventGroupClearBits(eventGroupHandle, FAULT_OVERCURRENT);
		}

		// Enqueue telemetry
		Telemetry_t t;
		t.seq = seq;
		t.avg0 = ch_avg[0];
		xQueueSend(telemetryQueue, &t, 0);

		// Send commands if required
		Command_t cmd = decide_command_from(ch_avg);
		xQueueSend(commandQueue, &cmd, 0);

		// Heartbeat
		xTaskNotify(supervisorHandle, CONTROL_HEARTBEAT_BIT, eSetBits);

		// convert integer to string safely
		snprintf(tx3, sizeof(tx3), "NV:%d %d \n", vrms_int, irms_int);
		xQueueSend(q_print, &tx3, portMAX_DELAY);
		snprintf(tx3, sizeof(tx3), "NPT:%d %d \n", power_int, temperature_int);
		xQueueSend(q_print, &tx3, portMAX_DELAY);

	}
}

Fault_e checkMotorPara(Motor_t *m,) {
	if ((*m->Vdc) < MOTOR_UNDER_VDC_LIM) {
		xEventGroupSetBits(eventGroupHandle, FAULT_UNDER_VDC_EVENTBIT);

		return FLT_UNDER_VOLT;
	}
	if ((*m->Idc) < MOTOR_UNDER_IDC_LIM) {
		xEventGroupSetBits(eventGroupHandle, FAULT_UNDER_IDC_EVENTBIT);
		return FLT_UNDER_CURR;
	}
	if ((*m->Pow) < MOTOR_UNDER_POW_LIM) {
		xEventGroupSetBits(eventGroupHandle, FAULT_UNDER_POW_EVENTBIT);
		return FLT_UNDER_POW;
	}
	if ((*m->Temprature) < MOTOR_UNDER_TEMPRATURE_LIM) {
		xEventGroupSetBits(eventGroupHandle, FAULT_UNDER_TEMPRATURE_EVENTBIT);
		return FLT_UNDER_TEMP;
	}

	if ((*m->Vdc) > MOTOR_OVER_VDC_LIM) {
		xEventGroupSetBits(eventGroupHandle, FAULT_OVER_VDC_EVENTBIT);
		return FLT_OVER_VOLT;
	}
	if ((*m->Idc) > MOTOR_OVER_IDC_LIM) {
		xEventGroupSetBits(eventGroupHandle, FAULT_OVER_IDC_EVENTBIT);
		return FLT_OVER_CURR;
	}
	if ((*m->Pow) > MOTOR_OVER_POW_LIM) {
		xEventGroupSetBits(eventGroupHandle, FAULT_OVER_POW_EVENTBIT);
		return FLT_OVER_POW;
	}
	if ((*m->Temprature) > MOTOR_OVER_TEMPRATURE_LIM) {
		xEventGroupSetBits(eventGroupHandle, FAULT_OVER_TEMPRATURE_EVENTBIT);
		return FLT_OVER_TEMP;
	}
	return HEALTHY;

}

static void FaultTaskHandle(void *param) {

	EventBits_t uxBits;
	uint32_t fault_no;

	for (;;) {
		fault_no = FAULT_OVER_VDC_EVENTBIT | FAULT_OVER_IDC_EVENTBIT
				| FAULT_OVER_POW_EVENTBIT | FAULT_OVER_TEMP_EVENTBIT
				| FAULT_UNDER_VDC_EVENTBIT | FAULT_UNDER_IDC_EVENTBIT
				| FAULT_UNDER_POW_EVENTBIT | FAULT_UNDER_TEMP_EVENTBIT;

		uxBits = xEventGroupWaitBits(eventGroupHandle, fault_no,
		pdTRUE,  // clear on exit
				pdFALSE, // wait for any
				portMAX_DELAY);

		if (bits & FAULT_OVER_VDC_EVENTBIT) // extract "FAULT_OVER_VDC_EVENTBIT" bit from "bits" and check if 1 or 0
		{
			// Log the Fault and send it to queue
			LogEntry_t log = make_log(FAULT_OVER_VDC_EVENTBIT, get_timestamp());
			xQueueSend(flashQueue, &log, portMAX_DELAY);
			// Notify Communication
		}
		if (bits & FAULT_OVER_IDC_EVENTBIT)
		{	// Log the Fault and send it to queue
			LogEntry_t log = make_log(FAULT_OVER_IDC_EVENTBIT, get_timestamp());
			xQueueSend(flashQueue, &log, portMAX_DELAY);
			// Notify Communication
		}
		if (bits & FAULT_OVER_POW_EVENTBIT)
		{	// Log the Fault and send it to queue
			LogEntry_t log = make_log(FAULT_OVER_POW_EVENTBIT, get_timestamp());
			xQueueSend(flashQueue, &log, portMAX_DELAY);
			// Notify Communication
		}
		if (bits & FAULT_OVER_TEMP_EVENTBIT)
		{	// Log the Fault and send it to queue
			LogEntry_t log = make_log(FAULT_OVER_TEMP_EVENTBIT, get_timestamp());
			xQueueSend(flashQueue, &log, portMAX_DELAY);
			// Notify Communication
		}
		if (bits & FAULT_UNDER_VDC_EVENTBIT)
		{	// Log the Fault and send it to queue
			LogEntry_t log = make_log(FAULT_UNDER_VDC_EVENTBIT, get_timestamp());
			xQueueSend(flashQueue, &log, portMAX_DELAY);
			// Notify Communication
		}
		if (bits & FAULT_UNDER_IDC_EVENTBIT)
		{	// Log the Fault and send it to queue
			LogEntry_t log = make_log(FAULT_UNDER_IDC_EVENTBIT, get_timestamp());
			xQueueSend(flashQueue, &log, portMAX_DELAY);
			// Notify Communication
		}
		if (bits & FAULT_UNDER_POW_EVENTBIT)
		{	// Log the Fault and send it to queue
			LogEntry_t log = make_log(FAULT_UNDER_POW_EVENTBIT, get_timestamp());
			xQueueSend(flashQueue, &log, portMAX_DELAY);
			// Notify Communication
		}
		if (bits & FAULT_UNDER_TEMP_EVENTBIT)
		{	// Log the Fault and send it to queue
			LogEntry_t log = make_log(FAULT_UNDER_TEMP_EVENTBIT, get_timestamp());
			xQueueSend(flashQueue, &log, portMAX_DELAY);
			// Notify Communication
		}
	}

}

// ****************************************************************************************************
//                                      Logging Task
// ****************************************************************************************************

///* ---------- Event Payload ---------- */
//typedef struct {
//	FaultCode_e type;
//	float vdc;
//	float idc;
//	float power;
//	float temperature;
//	uint32_t tick; /* FreeRTOS tick when queued */
//} FaultEvent_t;
//
//QueueHandle_t faultQ = NULL;
//TaskHandle_t task = NULL;
//
//FaultEvent_t ring[FAULT_RING_SIZE];
//SemaphoreHandle_t ringLock = NULL; /* protects ring + counters */
//
//uint32_t ringHead = 0; /* write index */
//uint32_t ringCount = 0; /* number of valid entries (<= size) */
//uint32_t totalLogged = 0; /* total events successfully logged */
//uint32_t dropped = 0; /* queue full drops */
//
//faultQ = xQueueCreate(FAULT_QUEUE_LEN, sizeof(FaultEvent_t));
//ringLock = xSemaphoreCreateMutex();
//xTaskCreate(FaultLog_Task, "FaultLog", FAULT_TASK_STACK, NULL, FAULT_TASK_PRIO, & task);
//
//void ring_push(const FaultEvent_t *e) {
//	xSemaphoreTake(ringLock, portMAX_DELAY);
//	ring[ringHead] = *e;
//	ringHead = (ringHead + 1u) % FAULT_RING_SIZE;
//	if (ringCount < FAULT_RING_SIZE) {
//		ringCount++;
//	}
//	totalLogged++;
//	xSemaphoreGive(ringLock);
//}
//void FaultLog_Task(void *arg) {
//	(void) arg;
//	FaultEvent_t evt;
//
//	for (;;) {
//		if (xQueueReceive(faultQ, &evt, portMAX_DELAY) == pdTRUE) {
//
//			ring_push(&evt);
//
//			/* 2) Print/log (swap printf with your logger) */
//			printf(
//					"[FAULT] t=%lu type=%s vdc=%.3f idc=%.3f power=%.3f temp=%.3f\r\n",
//					(unsigned long) evt.tick, fault_str(evt.type),
//					(double) evt.vdc, (double) evt.idc, (double) evt.power,
//					(double) evt.temperature);
//
//			/* 3) Hook point: add any action you need (e.g., persist to flash/SD, raise alarm, etc.) */
//			/* FaultPersist_Append(&evt);  // <-- your persistence function */
//		}
//	}
//}
//
//bool Fault_Log(FaultCode_e type, float vdc, float idc, float power,
//		float temperature) {
//	if (faultQ == NULL)
//		return false;
//
//	FaultEvent_t evt = { .type = type, .vdc = vdc, .idc = idc, .power = power,
//			.temperature = temperature, .tick = xTaskGetTickCount() };
//
//	if (xQueueSend( faultQ, &evt, 0) != pdTRUE) {
//		/* Queue full: count drop */
//		xSemaphoreTake(ringLock, portMAX_DELAY);
//		dropped++;
//		xSemaphoreGive(ringLock);
//		return false;
//	}
//	return true;
//}


// ****************************************************************************************************
//                                      Supervisor Task
// ****************************************************************************************************

static void SupervisorTask(void *pv) {
    const TickType_t check_period = pdMS_TO_TICKS(HEARTBEAT_WINDOW_MS);
    const TickType_t timeout_ticks = pdMS_TO_TICKS(HEARTBEAT_TIMEOUT_MS);

    /* Initialize last heartbeat timestamps as "now" so system doesn't alarm on boot */
    TickType_t now = xTaskGetTickCount();
    last_heartbeat_control = now;
    last_heartbeat_comm    = now;
    last_heartbeat_flash   = now;

    for (;;) {
        uint32_t notif_value = 0;

        /* Wait for notifications from any monitored task for up to check_period.
           We use xTaskNotifyWait to receive bits set by other tasks (eSetBits).
           If no notification arrives within check_period, we will still run the health check.
        */
        (void)xTaskNotifyWait(0x00,          /* dont clear on entry */
                              0xFFFFFFFF,    /* clear all bits on exit */
                              &notif_value,  /* value containing bits set by notifying tasks */
                              check_period); /* block for check_period ticks */

        now = xTaskGetTickCount();

        /* If tasks sent heartbeat bits, update their last heartbeat timestamp(s) */
        if (notif_value & CONTROL_HEARTBEAT_BIT) {
            last_heartbeat_control = now;
        }
        if (notif_value & COMM_HEARTBEAT_BIT) {
            last_heartbeat_comm = now;
        }
        if (notif_value & FLASH_HEARTBEAT_BIT) {
            last_heartbeat_flash = now;
        }

        /* Health check: compare last heartbeat timestamps with timeout */
        bool control_ok = ((now - last_heartbeat_control) <= timeout_ticks);
        bool comm_ok    = ((now - last_heartbeat_comm)    <= timeout_ticks);
        bool flash_ok   = ((now - last_heartbeat_flash)   <= timeout_ticks);

        if (control_ok && comm_ok && flash_ok) {
            /* System healthy: refresh watchdog */
            refresh_watchdog();
            /* Clear any previously raised stall fault if present */
            if (eventGroupHandle != NULL) {
                xEventGroupClearBits(eventGroupHandle, FAULT_TASK_STALLED);
            }
        } else {
            /* One or more tasks stalled: set FAULT bit and take escalation actions */
            if (eventGroupHandle != NULL) {
                xEventGroupSetBits(eventGroupHandle, FAULT_TASK_STALLED);
            }

            /* Log the condition (optional) */
            if (!control_ok) log_supervisor_event("Supervisor: CONTROL task stalled");
            if (!comm_ok)    log_supervisor_event("Supervisor: COMM task stalled");
            if (!flash_ok)   log_supervisor_event("Supervisor: FLASH task stalled");

            /* Escalation policy:
               - Could be: try to restart tasks, try a soft reset, or trigger hardware reset.
               - Here we call a user hook to perform a hardware reset if desired.
               - WARNING: trigger_hardware_reset() may not return.
            */
            trigger_hardware_reset();

            /* If trigger_hardware_reset() returns, delay a bit before next check to avoid spamming */
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}


void SupervisorTask(void *pv) {
    TickType_t lastWake = xTaskGetTickCount();
    uint32_t last_control_seq = 0;
    for (;;) {
        // Wait for heartbeats in a loop; simplistic example: block for total window
        if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(200)) == 0) {
            // no heartbeat within 200 ms window -> escalate
            xEventGroupSetBits(eventGroupHandle, FAULT_TASK_STALLED);
        } else {
            refresh_watchdog();
        }
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(200));
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
	vTaskNotifyGiveFromISR(ParaCalcTaskHandle, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}


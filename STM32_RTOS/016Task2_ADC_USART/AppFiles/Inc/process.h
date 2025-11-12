
#include "measurement.h"



//**********************************************
// 					Notes
//***********************************************
/*
 * ADC NOTES :
 * -> HCLK = (180Mhz)
 * APB2 Prescaler = 16 thus , PCLK2 = 180Mhz / 16 = 11.25Mhz = (88.88 nSec)
 * ADC prescaler = 8 thus adc clock = 11.25Mhz / 8 = 1.40Mhz = (714.28 nSec)
 * ADC Sample Cycle = 480 cycles on each channel
 * ADC has intotal 9 Channels
 * DMA buffer is of size = (8 * (9 Channels))
 *
 * total time to fill dma buffer = (714.28 nSec) * 480cycles * 9channels * 8timesdmabufsize = 24.68mSec = 40.5Hz
 *
 * */


#define F_REF				60.0f

#define DWT_CTRL        (*(volatile uint32_t*)0XE0001000)
#define MAX_LEN		32


typedef struct {
	uint32_t Vdc;
	uint32_t Idc;
	uint32_t Pow;
	uint32_t Temprature;
} Motor_t;

typedef enum {
	HEALTHY = 0,
	FLT_UNDER_VOLT,
	FLT_UNDER_CURR,
	FLT_UNDER_POW,
	FLT_UNDER_TEMP,
	FLT_OVER_VOLT,
	FLT_OVER_CURR,
	FLT_OVER_POW,
	FLT_OVER_TEMP
} FaultCode_e;

// Fault Bits for Event Group
#define FAULT_OVER_VDC_EVENTBIT (1 << 0)
#define FAULT_OVER_IDC_EVENTBIT (1 << 1)
#define FAULT_OVER_POW_EVENTBIT (1 << 2)
#define FAULT_OVER_TEMP_EVENTBIT (1 << 3)

#define FAULT_UNDER_VDC_EVENTBIT (1 << 4)
#define FAULT_UNDER_IDC_EVENTBIT (1 << 5)
#define FAULT_UNDER_POW_EVENTBIT (1 << 6)
#define FAULT_UNDER_TEMP_EVENTBIT (1 << 7)


#ifdef _PROCESS_

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern void SEGGER_UART_init(uint32_t);


static void LED_Green_Handler(void *parameters);
static void LED_Red_Handler(void *parameters);
static void Para_Calc_Handler(void *param);
static void Print_Handler(void *param);

void parameterCalculations(void);
void RTOSOneTimeInit(void);
void ProcessOneTimeInit(void);

#else
extern void RTOSOneTimeInit(void);
extern void ProcessOneTimeInit(void);
#endif

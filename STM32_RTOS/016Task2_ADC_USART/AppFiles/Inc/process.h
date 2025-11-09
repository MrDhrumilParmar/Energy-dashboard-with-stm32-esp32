
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

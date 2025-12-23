
#define ADC_SAMPLE_SIZE         	8  // no fo sample in one cycle of tim1
#define ADC_SAMPLE_IN_POW_2			3

#if ((1 << ADC_SAMPLE_IN_POW_2) != ADC_SAMPLE_SIZE)
#error "ADC_SAMPLE_SIZE must be a power of 2 when using bit shift averaging!"
#endif


#if ADC_SAMPLE_SIZE > 0
	#define INV_ADC_SAMPLE_SIZE			(1.0f/ADC_SAMPLE_SIZE)
#endif

#define ADC1_CHN                	6
#define ADC1_BUFF_SIZE 				(ADC1_CHN * ADC_SAMPLE_SIZE)

#define ADC2_CHN                	3
#define ADC2_BUFF_SIZE 				(ADC2_CHN * ADC_SAMPLE_SIZE)

#define ADC3_CHN                	0
#define ADC3_BUFF_SIZE 				ADC3_CHN * ADC_SAMPLE_SIZE

#define TOTAL_ADC_PARAMETERS 			(ADC1_CHN + ADC2_CHN + ADC3_CHN)

typedef struct {
	uint16_t ADC1_Val[ADC1_BUFF_SIZE];
	uint16_t ADC2_Val[ADC2_BUFF_SIZE];

	uint32_t ADC_SUM[TOTAL_ADC_PARAMETERS];
	uint32_t ADC_AVG[TOTAL_ADC_PARAMETERS];

} Adc_Buffer_Struct;

typedef struct {
	float input;
	float tempSum;
	float output;
	float outputUnfltr;
	float cnt;
} RmsStructure;

#ifdef _MEASUREMENT_

void adcBufferInit(Adc_Buffer_Struct *adcTemp);
void paraCalcAdc1(Adc_Buffer_Struct *adcTemp);
void paraCalcAdc2(Adc_Buffer_Struct *adcTemp);
void rmsCalulation(RmsStructure *kTemp, float sampleFreq, float fundamentalFreq,
		float lpfFactor);
#else
void adcBufferInit(Adc_Buffer_Struct *adcTemp);
void paraCalcAdc1(Adc_Buffer_Struct *adcTemp);
void paraCalcAdc2(Adc_Buffer_Struct *adcTemp);
void rmsCalulation(RmsStructure *kTemp, float sampleFreq, float fundamentalFreq,
		float lpfFactor);
#endif

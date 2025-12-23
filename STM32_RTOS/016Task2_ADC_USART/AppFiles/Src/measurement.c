/* Buffer locations and respective quantities
 ADC_AVG[0];   PA0 -> VDC 1
 ADC_AVG[1];   PA1 -> Va  1
 ADC_AVG[2];   PA2 -> Vb  1
 ADC_AVG[3];   PA3 -> Vc  1

 ADC_AVG[4];   PC0 -> ILa 3
 ADC_AVG[5];   PC1 -> VLa 3
 ADC_AVG[6];   PC2 -> VLb 3
 ADC_AVG[7];   PC3 -> VLc 3

 ADC_AVG[1];   PC4 -> ILa 2
 ADC_AVG[9];   PC5 -> ILc 2
 ADC_AVG[10];  PA6 -> ILb 2
 */
#define _MEASUREMENT_

#include <main.h>

#include "measurement.h"

void adcBufferInit(Adc_Buffer_Struct *adcTemp) {

	memset(adcTemp->ADC1_Val, 0, sizeof(adcTemp->ADC1_Val));
	memset(adcTemp->ADC2_Val, 0, sizeof(adcTemp->ADC2_Val));
	memset(adcTemp->ADC_SUM, 0, sizeof(adcTemp->ADC_SUM));
	memset(adcTemp->ADC_AVG, 0, sizeof(adcTemp->ADC_AVG));

//	// Zero whole ADC1 buffer
//	for (int i = 0; i < ADC1_BUFF_SIZE; i++) {
//		adcTemp->ADC1_Val[i] = 0;
//	}
//
//	// Zero whole ADC2 buffer
//	for (int i = 0; i < ADC2_BUFF_SIZE; i++) {
//		adcTemp->ADC2_Val[i] = 0;
//	}
//
//	// Zero summary / avg entries
//	for (int i = 0; i < TOTAL_ADC_PARAMETERS; i++) {
//		adcTemp->ADC_SUM[i] = 0;
//		adcTemp->ADC_AVG[i] = 0;
//	}

}
/*Function description: Function to calculate Sum and  Average of ADC2 Parameters*/

void paraCalcAdc1(Adc_Buffer_Struct *adcTemp) {

//    adcTemp->ADC_SUM[0] = 0;
//    adcTemp->ADC_SUM[1] = 0;
//    adcTemp->ADC_SUM[2] = 0;
//    adcTemp->ADC_SUM[3] = 0;
//    adcTemp->ADC_SUM[4] = 0;
//    adcTemp->ADC_SUM[5] = 0;
//    adcTemp->ADC_SUM[6] = 0; // wrong
	for (int i = 0; i < ADC1_CHN; i++) {
		adcTemp->ADC_SUM[i] = 0;
	}

	for (int i = 0; i < ADC_SAMPLE_SIZE; i++) {
//        adcTemp->ADC_SUM[0] += adcTemp->ADC2_Val[(i * ADC2_CHN) + 0];  //PC4 -> ILb
//        adcTemp->ADC_SUM[1] += adcTemp->ADC2_Val[(i * ADC2_CHN) + 1];  //PC4 -> ILb
//        adcTemp->ADC_SUM[2] += adcTemp->ADC2_Val[(i * ADC2_CHN) + 2];  //PC4 -> ILb
//        adcTemp->ADC_SUM[3] += adcTemp->ADC2_Val[(i * ADC2_CHN) + 3];  //PC4 -> ILb
//        adcTemp->ADC_SUM[4] += adcTemp->ADC2_Val[(i * ADC2_CHN) + 4];  //PC4 -> ILb
//        adcTemp->ADC_SUM[5] += adcTemp->ADC2_Val[(i * ADC2_CHN) + 5];  //PC4 -> ILb
//        adcTemp->ADC_SUM[6] += adcTemp->ADC2_Val[(i * ADC2_CHN) + 6];  //PC4 -> ILb  // wrong
		for (int j = 0; j < ADC1_CHN; j++) {
			adcTemp->ADC_SUM[j] += adcTemp->ADC1_Val[(i * ADC1_CHN) + j]; //PC4 -> ILb
		}
	}

//    adcTemp->ADC_AVG[0] = (adcTemp->ADC_SUM[0] * INV_ADC_SAMPLE_SIZE);  //PC4 -> ILb
//    adcTemp->ADC_AVG[1] = (adcTemp->ADC_SUM[1] * INV_ADC_SAMPLE_SIZE);  //PC4 -> ILb
//    adcTemp->ADC_AVG[2] = (adcTemp->ADC_SUM[2] * INV_ADC_SAMPLE_SIZE);  //PC4 -> ILb
//    adcTemp->ADC_AVG[3] = (adcTemp->ADC_SUM[3] * INV_ADC_SAMPLE_SIZE);  //PC4 -> ILb
//    adcTemp->ADC_AVG[4] = (adcTemp->ADC_SUM[4] * INV_ADC_SAMPLE_SIZE);  //PC4 -> ILb
//    adcTemp->ADC_AVG[5] = (adcTemp->ADC_SUM[5] * INV_ADC_SAMPLE_SIZE);  //PC4 -> ILb
//    adcTemp->ADC_AVG[6] = (adcTemp->ADC_SUM[6] * INV_ADC_SAMPLE_SIZE);  //PC4 -> ILb // wrong
	for (int i = 0; i < ADC1_CHN; i++) {
		adcTemp->ADC_AVG[i] = (adcTemp->ADC_SUM[i] >> ADC_SAMPLE_IN_POW_2); //PC4 -> ILb
	}
}

void paraCalcAdc2(Adc_Buffer_Struct *adcTemp) {

//	adcTemp->ADC_SUM[6] = 0;
//	adcTemp->ADC_SUM[7] = 0;
//	adcTemp->ADC_SUM[8] = 0;
	for (int i = ADC1_CHN; i < TOTAL_ADC_PARAMETERS; i++) {
		adcTemp->ADC_SUM[i] = 0;
	}

	for (int i = 0; i < ADC_SAMPLE_SIZE; i++) {
//		adcTemp->ADC_SUM[6] += adcTemp->ADC1_Val[(i * ADC1_CHN) + 0];
//		adcTemp->ADC_SUM[7] += adcTemp->ADC1_Val[(i * ADC1_CHN) + 1];
//		adcTemp->ADC_SUM[8] += adcTemp->ADC1_Val[(i * ADC1_CHN) + 2];
		for (int j = ADC1_CHN; j < TOTAL_ADC_PARAMETERS; j++) {
			adcTemp->ADC_SUM[j] += adcTemp->ADC2_Val[(i * ADC2_CHN)
					+ (j - ADC1_CHN)]; //PC4 -> ILb
		}

	}

	for (int i = ADC1_CHN; i < TOTAL_ADC_PARAMETERS; i++) {
		adcTemp->ADC_AVG[i] = (adcTemp->ADC_SUM[i] >> ADC_SAMPLE_IN_POW_2); //PC4 -> ILb
	}
//	adcTemp->ADC_AVG[6] = (adcTemp->ADC_SUM[6] * INV_ADC_SAMPLE_SIZE); //PC4 -> ILb
//	adcTemp->ADC_AVG[7] = (adcTemp->ADC_SUM[7] * INV_ADC_SAMPLE_SIZE); //PC4 -> ILb
//	adcTemp->ADC_AVG[8] = (adcTemp->ADC_SUM[8] * INV_ADC_SAMPLE_SIZE); //PC4 -> ILb
}

void rmsCalulation(RmsStructure *kTemp, float sampleFreq, float fundamentalFreq,
		float lpfFactor) {

	if (fundamentalFreq <= 1.0f)
		return;
	kTemp->outputUnfltr = 0.0f;
	kTemp->cnt++;
	kTemp->tempSum = kTemp->tempSum + (float) kTemp->input * kTemp->input;
	// control entered at every 1 mSec in side this
	if (kTemp->cnt > (float) sampleFreq / fundamentalFreq) {
		kTemp->outputUnfltr = sqrtf(kTemp->tempSum / kTemp->cnt);

		kTemp->cnt = 0;
		kTemp->tempSum = 0;
		//			if (kTemp->output<1){
		//				kTemp->output=1;
		//			}

		//			lpf on output
		kTemp->output = kTemp->outputUnfltr
				+ (lpfFactor * (kTemp->output - kTemp->outputUnfltr));
	}
}

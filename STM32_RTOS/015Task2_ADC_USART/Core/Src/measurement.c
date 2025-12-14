/* Buffer locations and respective quantities
 adcAvgVal[0];   PA0 -> VDC 1
 adcAvgVal[1];   PA1 -> Va  1
 adcAvgVal[2];   PA2 -> Vb  1
 adcAvgVal[3];   PA3 -> Vc  1

 adcAvgVal[4];   PC0 -> ILa 3
 adcAvgVal[5];   PC1 -> VLa 3
 adcAvgVal[6];   PC2 -> VLb 3
 adcAvgVal[7];   PC3 -> VLc 3

 adcAvgVal[1];   PC4 -> ILa 2
 adcAvgVal[9];   PC5 -> ILc 2
 adcAvgVal[10];  PA6 -> ILb 2
 */

#include "measurement.h"
#include "math.h"

void rmsCalulation (RmsStructure* kTemp, float sampleFreq, float fundamentalFreq, float lpfFactor) {

    if (fundamentalFreq <= 1.0f)
        return;
    kTemp->outputUnfltr = 0.0f;
    kTemp->cnt++;
    kTemp->tempSum = kTemp->tempSum + (float)kTemp->input * kTemp->input;
    // control entered at every 1 mSec in side this
    if (kTemp->cnt > (float)sampleFreq / fundamentalFreq) {
        kTemp->outputUnfltr = sqrtf (kTemp->tempSum / kTemp->cnt);

        kTemp->cnt = 0;
        kTemp->tempSum = 0;
        //			if (kTemp->output<1){
        //				kTemp->output=1;
        //			}

        //			lpf on output
        kTemp->output = kTemp->outputUnfltr + (lpfFactor * (kTemp->output - kTemp->outputUnfltr));
    }
}

#include "main.h"

typedef struct {
	float input;
	float tempSum;
	float output;
	float outputUnfltr;
	float cnt;
} RmsStructure;

void rmsCalulation(RmsStructure *kTemp, float sampleFreq, float fundamentalFreq,
		float lpfFactor);

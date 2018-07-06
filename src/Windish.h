#ifndef __Windish_H
#define __Windish_H

#include "arm_math.h"
#include "arm_const_structs.h"
#include <math.h>
#include "main.h"


#define FFT_IQ_imbalanse_size 128

#define uint unsigned int
#define NUM_FRAMES (48000/(5*FFT_IQ_imbalanse_size))//Время накопления 100мс
#define THRESHOLD	1000//Порог 20дБ
#define TRUE (int)1
#define FALSE (int)0
#define Mask0Hz (int)1
#define MISMATCH_COUNT (2*FFT_FILTER_SIZE/FFT_IQ_imbalanse_size)
#define Limit_CNT SAMPLE_RATE
#define I_START_AM_FM 1
#define I_START_SSB (FFT_IQ_imbalanse_size/4)
#define NUM_FRAMES_FOR_UPDATE (5*NUM_FRAMES)
#define NUM_FRAMES_FOR_COLLECTION (NUM_FRAMES_FOR_UPDATE*3/4)
#define NUM_BLOCKS_FOR_RESET 5//Время забывания 2с

void mismatch_init (void);
void IQ_imbalance_compensation (int mode);




#endif
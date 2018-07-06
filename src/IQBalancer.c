
#include "main.h"
#include "IQBalancer.h"

#include "arm_math.h"
#include "arm_const_structs.h"
#include "arm_common_tables.h"
/*
#include "stm32f4xx.h"
#include "sdr.h"
#include "IQBalancer.h"
#include <math.h>
#include <stdlib.h>

#include "arm_math.h"
#include "arm_const_structs.h"
#include "arm_common_tables.h"
*/

#define FFTBins 128
//const int FFTBins = 1024;
const float DcTimeConst = 0.00001f;
const float BaseIncrement = 0.001f;
const float PowerThreshold = 20.0f; // in dB

int _maxAutomaticPasses = 10;//Utils.GetIntSetting("automaticIQBalancePasses", 10);
int _autoBalanceIQ;
float _gain = 1.0f;
float _phase=0.0f;
float _averagePower;
float _powerRange;
COMPLEX_float* _iqPtr;

float _windowPtr[ FFTBins ];
arm_cfft_instance_f32* CFFT_IQBalancer_init_struct;
//------------------------------------------------------------------------------
void IQBalancer_init( void ){
  _gain = 1.0f;
  _phase=0.0f;


  float  dTmpFloat = 2.0 * M_PI / (FFTBins);
  for (int i = 0; i < FFTBins; i++)
  {
    _windowPtr[i]= (float)(0.54 - 0.46 * cosf(dTmpFloat * i));
  }

  //CFFT_IQBalancer_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len128);

  int tmp = FFTBins;
  switch (tmp)
  {
    case 2048:
     CFFT_IQBalancer_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len2048);
    break;
    case 1024:
     CFFT_IQBalancer_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len1024);
    break;
    case 512:
     CFFT_IQBalancer_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len512);
    break;
    case 256:
    CFFT_IQBalancer_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len256);
    break;
    case 128:
     CFFT_IQBalancer_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len128);
    break;
    case 64:
     CFFT_IQBalancer_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len64);
    break;
    case 32:
     CFFT_IQBalancer_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len32);
    break;
    case 16:
     CFFT_IQBalancer_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len16);
    break;
  }
}

//------------------------------------------------------------------------------
void Process(COMPLEX_float* iq, int length){

  //RemoveDC(iq, length);
  _iqPtr = iq;
  EstimateImbalance();
  Adjust(iq, length, _phase, _gain);

}
//------------------------------------------------------------------------------
void EstimateImbalance( void ){

  EstimatePower();
  if (_powerRange < PowerThreshold)
  {
      //return;
  }

  float currentUtility = Utility(_phase, _gain);

  for (int count = 0; count < _maxAutomaticPasses; count++)
  {
      float phaseIncrement = BaseIncrement * GetRandomDirection();
      float gainIncrement = BaseIncrement * GetRandomDirection();

      float candidatePhase = _phase + phaseIncrement;
      float candidateGain = _gain + gainIncrement;
      float candidateUtility = Utility(candidatePhase, candidateGain);

      if (candidateUtility > currentUtility)
      {
          currentUtility = candidateUtility;
          _gain = candidateGain;
          _phase = candidatePhase;
      }
  }
}
//------------------------------------------------------------------------------
COMPLEX_float rawFFTU[ FFTBins ];
float rawSpectrumPtrU[ FFTBins ];
float Utility(float phase, float gain)
{

    //Utils.Memcpy(fftPtr, _iqPtr, FFTBins * sizeof(Complex));
    for(int i=0; i<FFTBins; ++i ){
      rawFFTU[i].re = _iqPtr[i].re;
      rawFFTU[i].im = _iqPtr[i].im;
    }

    Adjust(rawFFTU, FFTBins, phase, gain);

    //Fourier.ApplyFFTWindow(fftPtr, _windowPtr, FFTBins);
    for(int i=0; i<FFTBins; ++i ){
      rawFFTU[i].re *= _windowPtr[i];
      rawFFTU[i].im *= _windowPtr[i];
    }

    //Fourier.ForwardTransform(fftPtr, FFTBins);
    arm_cfft_f32(CFFT_IQBalancer_init_struct, (float*)&rawFFTU,0,1);

    //Fourier.SpectrumPower(fftPtr, spectrumPtr, FFTBins);
    #define offset 0
    for (int i = 0; i < FFTBins; i++)
    {
        float m = rawFFTU[i].re * rawFFTU[i].re + rawFFTU[i].im * rawFFTU[i].im;
        float strength = (float)(10.0 * log10f(1e-60 + m)) + offset;
        rawSpectrumPtrU[i] = strength;
    }

    float result = 0.0f;
    const int halfBins = FFTBins / 2;
    for (int i = 0; i < halfBins; i++)
    {
        int distanceFromCenter = halfBins - i;

        if (distanceFromCenter > 0.05f * halfBins && distanceFromCenter < 0.95f * halfBins)
        {
            int j = FFTBins - 2 - i;
            if (rawSpectrumPtrU[i] - _averagePower > PowerThreshold || rawSpectrumPtrU[j] - _averagePower > PowerThreshold)
            {
                float distance = rawSpectrumPtrU[i] - rawSpectrumPtrU[j];
                result += distance * distance;
            }
        }
    }

    return result;
}
//------------------------------------------------------------------------------
float GetRandomDirection( void ){
  float rnd = rand()&0xFF / 0xFF;
  return rnd > 0.5 ? 1.0 : -1.0;
  //return -1.0;
}
//------------------------------------------------------------------------------

COMPLEX_float rawFFTIQB[ FFTBins ];
float rawSpectrumPtrIQB[ FFTBins ];

void EstimatePower( void )
{
   /* var rawFFTPtr = stackalloc byte[FFTBins * sizeof(Complex) + 16];
    var fftPtr = (Complex*) (((long) rawFFTPtr + 15) & ~15);
    var rawSpectrumPtr = stackalloc byte[FFTBins * sizeof(float) + 16];
    var spectrumPtr = (float*) (((long) rawSpectrumPtr + 15) & ~15);

    Utils.Memcpy(fftPtr, _iqPtr, FFTBins * sizeof(Complex));
    Fourier.ApplyFFTWindow(fftPtr, _windowPtr, FFTBins);
    Fourier.ForwardTransform(fftPtr, FFTBins);
    Fourier.SpectrumPower(fftPtr, spectrumPtr, FFTBins);*/


    for(int i=0; i<FFTBins; ++i ){
      rawFFTIQB[i].re = _iqPtr[i].re*_windowPtr[i];
      rawFFTIQB[i].im = _iqPtr[i].im*_windowPtr[i];
    }
    arm_cfft_f32(CFFT_IQBalancer_init_struct, (float*)&rawFFTIQB,0,1);

    #define offset 0
    for (int i = 0; i < FFTBins; i++)
    {
        float m = rawFFTIQB[i].re * rawFFTIQB[i].re + rawFFTIQB[i].im * rawFFTIQB[i].im;
        float strength = (float)(10.0 * log10f(1e-60 + m)) + offset;//2.4ms-128bins
        rawSpectrumPtrIQB[i] = strength;
    }


    const int halfBins = FFTBins / 2;
    float max = -3.4e38;//float.NegativeInfinity;
    float avg = 0.0f;
    int count = 0;
    for (int i = 0; i < halfBins; i++)
    {
        int distanceFromCenter = halfBins - i;

        if (distanceFromCenter > 0.05f * halfBins && distanceFromCenter < 0.95f * halfBins)
        {
            int j = FFTBins - 2 - i;

            if (rawSpectrumPtrIQB[i] > max)
                max = rawSpectrumPtrIQB[i];

            if (rawSpectrumPtrIQB[j] > max)
                max = rawSpectrumPtrIQB[j];

            avg += rawSpectrumPtrIQB[i] + rawSpectrumPtrIQB[j];
            count += 2;
        }
    }
    avg /= count;

    _powerRange = max - avg;
    _averagePower = avg;
}
//------------------------------------------------------------------------------
#if ACCURACY_PRIVILEGED

        void Adjust(COMPLEX_float* buffer, int length, float phase, float gain)
        {
            for (int i = 0; i < length; i++)
            {
                buffer[i].im = (float)(buffer[i].im * (1.0 + gain) * cosf(phase));
                buffer[i].re = (float)(buffer[i].re - buffer[i].im * tanf(phase));
            }
        }

#else

        void Adjust(COMPLEX_float* buffer, int length, float phase, float gain)
        {
            for (int i = 0; i < length; i++)
            {
                buffer[i].re = buffer[i].re * gain + phase * buffer[i].im;
            }
        }

#endif
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------



//#include "main.h"

#include "lmadf.h"
#include "math.h"

#pragma section = "CCM_RAM"

_lmsstate lms_buf@"CCM_RAM";
LMSR lms@"CCM_RAM";
REAL adaptive_filter[128]@"CCM_RAM";
REAL delay_line[512]@"CCM_RAM";
//------------------------------------------------------------------------------
void new_lmsr_init( COMPLEX *signal, int signal_size,
	  int delay,
	  REAL adaptation_rate,
	  REAL leakage, int adaptive_filter_size, int filter_type){

  lms =   &lms_buf;

  lms->signal = signal;
  lms->signal_size = signal_size;//CXBsize (lms->signal);
  lms->delay = delay;
  lms->size = 512;
  lms->mask = lms->size - 1;
  lms->delay_line = delay_line;//newvec_REAL (lms->size, "lmsr delay");
  lms->adaptation_rate = adaptation_rate;
  lms->leakage = leakage;
  lms->adaptive_filter_size = adaptive_filter_size;
  lms->adaptive_filter = adaptive_filter;//newvec_REAL (128, "lmsr filter");
  lms->filter_type = filter_type;
  lms->delay_line_ptr = 0;
          
 }
//------------------------------------------------------------------------------
void lmsr_adapt(/*LMSR lms*/void) {
  switch (lms->filter_type) {
 /* case LMADF_NOISE:
    lmsr_adapt_n(lms);
    break;*/
  case LMADF_INTERFERENCE:
    lmsr_adapt_i(/*lms*/);
    break;
  }
}
//------------------------------------------------------------------------------
REAL
sqr (REAL x)
{
  return x * x;
}
//------------------------------------------------------------------------------
void lmsr_adapt_i (/*LMSR lms*/void)
{
  int i, j, k, k_ptr, afs=lms->adaptive_filter_size;
  REAL sum_sq=0, scl1=0, scl2=0;
  REAL accum=0, error=0;

  scl1 = (REAL) (1.0 - rate * leak);

  for (i = 0; i < ssiz; i++)
    {
      lms->delay_line[lms->delay_line_ptr] = lms->signal[i].re;
      
      accum = 0.0;
      sum_sq = 0.0;

      k_ptr = lms->delay + lms->delay_line_ptr;
      
      for (j = 0; j < afs; j++)//3.6ms
	{
          k=((j + k_ptr) & lms->mask);
          sum_sq += lms->delay_line[k]*lms->delay_line[k];
          accum += lms->adaptive_filter[j] * lms->delay_line[k];
	}

      error = lms->signal[i].re - accum;
      lms->signal[i].im = lms->signal[i].re = error;
      scl2 =  (lms->adaptation_rate / (sum_sq + 1e-10));
      error *= scl2;

      for (j = 0; j < afs; j++)//7.2ms
	{
          k=((j + k_ptr) & lms->mask);
          lms->adaptive_filter[j] = lms->adaptive_filter[j]*scl1 + error*lms->delay_line[k];
	}
      lms->delay_line_ptr = ((lms->delay_line_ptr + lms->mask) & (lms->mask));
    }
}
//------------------------------------------------------------------------------
void lmsr_adapt_n (void)
{
  int i, j, k;
  REAL sum_sq, scl1, scl2;
  REAL accum, error;

  scl1 = (REAL) (1.0 - rate * leak);

  for (i = 0; i < ssiz; i++)
    {

      dlay (dptrf) = ssig (i);
      accum = 0.0;
      sum_sq = 0.0;

      for (j = 0; j < asiz; j++)
	{
	  k = wrap (j);
	  sum_sq += sqr (dlay (k));
	  accum += afil (j) * dlay (k);
	}

      error = ssig (i) - accum;
      ssig_i (i) = ssig (i) = accum;

      scl2 = (REAL) (rate / (sum_sq + 1e-10));
      error *= scl2;
      for (j = 0; j < asiz; j++)
	{
	  k = wrap (j);
	  afil (j) = afil (j) * scl1 + error * dlay (k);
	}

      dptrf = bump (dptrf);
    }
}
//------------------------------------------------------------------------------

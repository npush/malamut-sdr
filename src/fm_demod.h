#ifndef __FM_DEMOD_H
#define __FM_DEMOD_H

void Cmplx_sdr_fm (float x, float y);
float ATAN2_fm(COMPLEX_float * input);
void init_pll (float samprate, float freq, float lofreq, float hifreq, float bandwidth);
void pll_func ( COMPLEX_float * input);
void Get_FM_PLL_demodulation (COMPLEX_float * in_spec_buff, float * out_buff);

float fast_atan2 (float y, float x);

void fm_demod( COMPLEX_float *in, float *out, int size );

#endif
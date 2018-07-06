#include "main.h"
//------------------------------------------------------------------------------
#define NB_MAX_HANG 7

COMPLEX_float nb_delay1[NB_MAX_HANG +1];
COMPLEX_float nb_average_sig1;
float nb_average_mag1=1;
float nb_hangtime11=0;
float nb_threshold1=4;
int nb_sigindex1=0;
int nb_delayindex1=2;


COMPLEX_float nb_delay[NB_MAX_HANG +1];
COMPLEX_float nb_average_sig;
float nb_average_mag=1;
float nb_hangtime1=0;
float nb_threshold=4;
int nb_sigindex=0;
int nb_delayindex=2;


void noiseblanker( COMPLEX_float * input, int input_size, int type, int level) {

  //float   nb_threshold_value = 1500;//nb_threshold_value=300+(notch_value(0..3000)/2);

  if( type == 0 )
    return;

  int i=0;
  float  cmag;
  if ( (type&0x01) == 0x01)
  {
    //nb_threshold1=(float)((15-level)/10.0)+1.0;
    nb_threshold1 = 3.3;
    for (i = 0; i < input_size; i++)
    {
      cmag = input[i].re*input[i].re+input[i].im*input[i].im;
      nb_delay1[nb_sigindex1].re = input[i].re;
      nb_delay1[nb_sigindex1].im = input[i].im;
      nb_average_mag1 = (0.999f * nb_average_mag1) + (0.001f * cmag);
      if ((nb_hangtime1 == 0) && (cmag > (nb_threshold1 * nb_average_mag1)))
        nb_hangtime1 = NB_MAX_HANG ;
      if (nb_hangtime1 > 0)
      {
        input[i].re=input[i].im=0;
        nb_hangtime1--;
      }
      else
      {
        input[i].re = nb_delay1[nb_delayindex1].re;
        input[i].im = nb_delay1[nb_delayindex1].im;
      }
      nb_sigindex1 = (nb_sigindex1 + NB_MAX_HANG ) & NB_MAX_HANG ;
      nb_delayindex1 = (nb_delayindex1+ NB_MAX_HANG ) & NB_MAX_HANG ;
    }
  }


  if ( (type&0x2) == 0x02)//SDROM
  {
    //nb_threshold=(float)((15-level)/10.0)+1.0;
    nb_threshold = 2.5;

    for (i = 0; i < input_size; i++)
    {
      cmag = input[i].re*input[i].re+input[i].im*input[i].im;
      nb_average_sig.re = (nb_average_sig.re*0.75f)+(input[i].re*0.25f);
      nb_average_sig.im = (nb_average_sig.im*0.75f)+(input[i].im*0.25f);
      nb_average_mag = (0.999f * nb_average_mag) + (0.001f * cmag);
      if (cmag > nb_threshold * nb_average_mag)
      {
        input[i].re=nb_average_sig.re;
        input[i].im=nb_average_sig.im;
      }

    }
  }
}
//------------------------------------------------------------------------------

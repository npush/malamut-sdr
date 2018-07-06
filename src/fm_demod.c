#include "main.h"
//------------------------------------------------------------------------------

COMPLEX_float z,sig;
typedef struct common_pll_
{
  float f;
  float l;
  float h;
  float phs;
  COMPLEX_float delay;
  float iir_alpha;
  float alpha;
  float beta;
}common_pll;

typedef struct common_fm_
{
  float lock;
  float afc;
  float cvt;
}common_fm;

common_pll pll;
common_fm fm;

void Cmplx_fm (float x, float y)
{
  z.re = x, z.im = y;
}
float ATAN2_fm(COMPLEX_float * input)
{
  float tmp;
  //Special handling to avoid divide by 0
  //Alternative is to set re = tiny amount and continue to use atan()
  if ( input->re !=0)
  {
    tmp = atanf((float)(input->im /input->re));
//    tmp = atan2f((float)input->im ,(float)input->re);
    //float tmp1 = atan(im/re);
    //Correct for errors if re and im are negative
    if (input->re < 0 && input->im < 0)
      tmp -= Pi; //Subtract 180deg
    else
      if (input->re < 0 && input->im >= 0)
        tmp += Pi;
  }
  else
    tmp=0;

  return tmp;
}
void init_pll (float samprate, float freq, float lofreq, float hifreq, float bandwidth)
{
  float fac = (float) (2*3.1415926535897932384626433832795f / samprate);
  pll.f = freq * fac;
  pll.l = lofreq * fac;
  pll.h = hifreq * fac;
  pll.phs = 0.0;
  pll.delay.re = 0;
  pll.delay.im = 0;
  pll.iir_alpha = bandwidth * fac;	/* arm filter */
  pll.alpha = pll.iir_alpha * 0.3f;	/* pll bandwidth */
  pll.beta = pll.alpha * pll.alpha * 0.25f;	/* second order term */
  fm.lock = 0.5;
  fm.afc = 0.0;
  fm.cvt = (float) (0.45 * samprate / (3.1415926535897932384626433832795f * bandwidth));


}
//------------------------------------------------------------------------------

 void  pll_func ( COMPLEX_float * input)
{
  volatile  float diff;
  sig.re=input->re;
  sig.im=input->im;
  Cmplx_fm ((float) cosf (pll.phs), (float) sinf (pll.phs));
  pll.delay.re = z.re * sig.re + z.im * sig.im;
  pll.delay.im = -z.im * sig.re + z.re * sig.im;

  //diff = ATAN2_fm (&pll.delay);
  diff = fast_atan2 ( pll.delay.im, pll.delay.re );

  pll.f += pll.beta * diff;
  if (pll.f < pll.l)
    pll.f = pll.l;
  if (pll.f > pll.h)
    pll.f = pll.h;
  pll.phs += pll.f + pll.alpha * diff;
  while (pll.phs >= TWOPI)
    pll.phs -= (float) TWOPI;
  while (pll.phs < 0)
    pll.phs += (float) TWOPI;
}
//------------------------------------------------------------------------------
 // static int tmpinit_pll=0xffff;
void Get_FM_PLL_demodulation (COMPLEX_float * in_spec_buff, float * out_buff)
{
  int i;

  /*if (tmpinit_pll!=1)
  {
    float hifreq=10500.0f, lowfreq=-10500.0f, bandwidth=9000.0f, samprate=(float)Exchange.Sample_Rate;
    //float hifreq=3000.0f, lowfreq=-3000.0f, bandwidth=1200.0f, samprate=(float)SAMPLE_RATE;
    init_pll (samprate, 0.0, lowfreq, hifreq, bandwidth);
    tmpinit_pll=1;
  }*/


 // for (i=FFT_FILTER_SIZE/2;i<FFT_FILTER_SIZE;i++)
  for (i=SdrStr.SdrSetup.FFT_Size/2;i<SdrStr.SdrSetup.FFT_Size;i++)
 {
    pll_func ((COMPLEX_float *)&in_spec_buff[i]);
   // fm.afc = (0.9 * fm.afc + 0.1 * pll.f);
    fm.afc = (0.9999 * fm.afc + 0.0001 * pll.f);
//    out_buff[i-FFT_FILTER_SIZE/2]=(pll.f - fm.afc) * fm.cvt*8388607.0f;//сигнал масштабируется
    out_buff[i-SdrStr.SdrSetup.FFT_Size/2]=(pll.f - fm.afc) * fm.cvt*8388607.0f;//сигнал масштабируется
  }
}
//------------------------------------------------------------------------------

/***************************************************************************/

/* Constant definitions                                                    */

/***************************************************************************/

#define TAN_MAP_RES     0.003921569	/* (smallest non-zero value in table) */

#define RAD_PER_DEG     0.017453293

#define TAN_MAP_SIZE    256

#pragma section = "CCM_RAM"

/* arctangents from 0 to pi/4 radians */
 float fast_atan_table[257]@"CCM_RAM" = {
	(float)0.000000e+00,(float) 3.921549e-03,(float) 7.842976e-03,(float) 1.176416e-02,
	(float)1.568499e-02,(float) 1.960533e-02,(float) 2.352507e-02,(float) 2.744409e-02,
	(float)3.136226e-02,(float) 3.527947e-02,(float) 3.919560e-02,(float) 4.311053e-02,
	(float)4.702413e-02,(float) 5.093629e-02,(float) 5.484690e-02,(float) 5.875582e-02,
	(float)6.266295e-02,(float) 6.656816e-02,(float) 7.047134e-02,(float) 7.437238e-02,
	(float)7.827114e-02,(float) 8.216752e-02,(float) 8.606141e-02,(float) 8.995267e-02,
	(float)9.384121e-02,(float) 9.772691e-02,(float) 1.016096e-01,(float) 1.054893e-01,
	(float)1.093658e-01,(float) 1.132390e-01,(float) 1.171087e-01,(float) 1.209750e-01,
	(float)1.248376e-01,(float) 1.286965e-01,(float) 1.325515e-01,(float) 1.364026e-01,
	(float)1.402496e-01,(float) 1.440924e-01,(float) 1.479310e-01,(float) 1.517652e-01,
	(float)1.555948e-01,(float) 1.594199e-01,(float) 1.632403e-01,(float) 1.670559e-01,
	(float)1.708665e-01,(float) 1.746722e-01,(float) 1.784728e-01,(float) 1.822681e-01,
	(float)1.860582e-01,(float) 1.898428e-01,(float) 1.936220e-01,(float) 1.973956e-01,
	(float)2.011634e-01,(float) 2.049255e-01,(float) 2.086818e-01,(float) 2.124320e-01,
	(float)2.161762e-01,(float) 2.199143e-01,(float) 2.236461e-01,(float) 2.273716e-01,
	(float)2.310907e-01,(float) 2.348033e-01,(float) 2.385093e-01,(float) 2.422086e-01,
	(float)2.459012e-01,(float) 2.495869e-01,(float) 2.532658e-01,(float) 2.569376e-01,
	(float)2.606024e-01,(float) 2.642600e-01,(float) 2.679104e-01,(float) 2.715535e-01,
	(float)2.751892e-01,(float) 2.788175e-01,(float) 2.824383e-01,(float) 2.860514e-01,
	(float)2.896569e-01,(float) 2.932547e-01,(float) 2.968447e-01,(float) 3.004268e-01,
	(float)3.040009e-01,(float) 3.075671e-01,(float) 3.111252e-01,(float) 3.146752e-01,
	(float)3.182170e-01,(float) 3.217506e-01,(float) 3.252758e-01,(float) 3.287927e-01,
	(float)3.323012e-01,(float) 3.358012e-01,(float) 3.392926e-01,(float) 3.427755e-01,
	(float)3.462497e-01,(float) 3.497153e-01,(float) 3.531721e-01,(float) 3.566201e-01,
	(float)3.600593e-01,(float) 3.634896e-01,(float) 3.669110e-01,(float) 3.703234e-01,
	(float)3.737268e-01,(float) 3.771211e-01,(float) 3.805064e-01,(float) 3.838825e-01,
	(float)3.872494e-01,(float) 3.906070e-01,(float) 3.939555e-01,(float) 3.972946e-01,
	(float)4.006244e-01,(float) 4.039448e-01,(float) 4.072558e-01,(float) 4.105574e-01,
	(float)4.138496e-01,(float) 4.171322e-01,(float) 4.204054e-01,(float) 4.236689e-01,
	(float)4.269229e-01,(float) 4.301673e-01,(float) 4.334021e-01,(float) 4.366272e-01,
	(float)4.398426e-01,(float) 4.430483e-01,(float) 4.462443e-01,(float) 4.494306e-01,
	(float)4.526070e-01,(float) 4.557738e-01,(float) 4.589307e-01,(float) 4.620778e-01,
	(float)4.652150e-01,(float) 4.683424e-01,(float) 4.714600e-01,(float) 4.745676e-01,
	(float)4.776654e-01,(float) 4.807532e-01,(float) 4.838312e-01,(float) 4.868992e-01,
	(float)4.899573e-01,(float) 4.930055e-01,(float) 4.960437e-01,(float) 4.990719e-01,
	(float)5.020902e-01,(float) 5.050985e-01,(float) 5.080968e-01,(float) 5.110852e-01,
	(float)5.140636e-01,(float) 5.170320e-01,(float) 5.199904e-01,(float) 5.229388e-01,
	(float)5.258772e-01,(float) 5.288056e-01,(float) 5.317241e-01,(float) 5.346325e-01,
	(float)5.375310e-01,(float) 5.404195e-01,(float) 5.432980e-01,(float) 5.461666e-01,
	(float)5.490251e-01,(float) 5.518738e-01,(float) 5.547124e-01,(float) 5.575411e-01,
	(float)5.603599e-01,(float) 5.631687e-01,(float) 5.659676e-01,(float) 5.687566e-01,
	(float)5.715357e-01,(float) 5.743048e-01,(float) 5.770641e-01,(float) 5.798135e-01,
	(float)5.825531e-01,(float) 5.852828e-01,(float) 5.880026e-01,(float) 5.907126e-01,
	(float)5.934128e-01,(float) 5.961032e-01,(float) 5.987839e-01,(float) 6.014547e-01,
	(float)6.041158e-01,(float) 6.067672e-01,(float) 6.094088e-01,(float) 6.120407e-01,
	(float)6.146630e-01,(float) 6.172755e-01,(float) 6.198784e-01,(float) 6.224717e-01,
	(float)6.250554e-01,(float) 6.276294e-01,(float) 6.301939e-01,(float) 6.327488e-01,
	(float)6.352942e-01,(float) 6.378301e-01,(float) 6.403565e-01,(float) 6.428734e-01,
	(float)6.453808e-01,(float) 6.478788e-01,(float) 6.503674e-01,(float) 6.528466e-01,
	(float)6.553165e-01,(float) 6.577770e-01,(float) 6.602282e-01,(float) 6.626701e-01,
	(float)6.651027e-01,(float) 6.675261e-01,(float) 6.699402e-01,(float) 6.723452e-01,
	(float)6.747409e-01,(float) 6.771276e-01,(float) 6.795051e-01,(float) 6.818735e-01,
	(float)6.842328e-01,(float) 6.865831e-01,(float) 6.889244e-01,(float) 6.912567e-01,
	(float)6.935800e-01,(float) 6.958943e-01,(float) 6.981998e-01,(float) 7.004964e-01,
	(float)7.027841e-01,(float) 7.050630e-01,(float) 7.073330e-01,(float) 7.095943e-01,
	(float)7.118469e-01,(float) 7.140907e-01,(float) 7.163258e-01,(float) 7.185523e-01,
	(float)7.207701e-01,(float) 7.229794e-01,(float) 7.251800e-01,(float) 7.273721e-01,
	(float)7.295557e-01,(float) 7.317307e-01,(float) 7.338974e-01,(float) 7.360555e-01,
	(float)7.382053e-01,(float) 7.403467e-01,(float) 7.424797e-01,(float) 7.446045e-01,
	(float)7.467209e-01,(float) 7.488291e-01,(float) 7.509291e-01,(float) 7.530208e-01,
	(float)7.551044e-01,(float) 7.571798e-01,(float) 7.592472e-01,(float) 7.613064e-01,
	(float)7.633576e-01,(float) 7.654008e-01,(float) 7.674360e-01,(float) 7.694633e-01,
	(float)7.714826e-01,(float) 7.734940e-01,(float) 7.754975e-01,(float) 7.774932e-01,
	(float)7.794811e-01,(float) 7.814612e-01,(float) 7.834335e-01,(float) 7.853983e-01,
	(float) 7.853983e-01
};

        
inline float fast_atan2 (float y, float x)
{
	float x_abs, y_abs, z_fm;
	float alpha, angle, base_angle;
	int index;

	/* don't divide by zero! */
	if ((y == 0.0) && (x == 0.0))
		angle = 0.0;
	else
	{
		/* normalize to +/- 45 degree range */
		y_abs = (float)fabs (y);
		x_abs = (float)fabs (x);
		//z = (y_abs < x_abs ? y_abs / x_abs : x_abs / y_abs);
		if (y_abs < x_abs)
			z_fm = y_abs / x_abs;
		else
			z_fm = x_abs / y_abs;
		/* when ratio approaches the table resolution, the angle is */
		/*      best approximated with the argument itself...       */
		if (z_fm < TAN_MAP_RES)
			base_angle = z_fm;
		else
		{
			/* find index and interpolation value */
			alpha = z_fm * (float) (TAN_MAP_SIZE - .5);
			index = (int) alpha;
			alpha -= (float) index;
			/* determine base angle based on quadrant and */
			/* add or subtract table value from base angle based on quadrant */
			base_angle = fast_atan_table[index];
			base_angle +=
				(fast_atan_table[index + 1] - fast_atan_table[index]) * alpha;
		}

		if (x_abs > y_abs)
		{			/* -45 -> 45 or 135 -> 225 */
			if (x >= 0.0)
			{			/* -45 -> 45 */
				if (y >= 0.0)
					angle = base_angle;	/* 0 -> 45, angle OK */
				else
					angle = -base_angle;	/* -45 -> 0, angle = -angle */
			}
			else
			{			/* 135 -> 180 or 180 -> -135 */
				angle = (float)3.14159265358979323846;
				if (y >= 0.0)
					angle -= base_angle;	/* 135 -> 180, angle = 180 - angle */
				else
					angle = base_angle - angle;	/* 180 -> -135, angle = angle - 180 */
			}
		}
		else
		{			/* 45 -> 135 or -135 -> -45 */
			if (y >= 0.0)
			{			/* 45 -> 135 */
				angle = (float)1.57079632679489661923;
				if (x >= 0.0)
					angle -= base_angle;	/* 45 -> 90, angle = 90 - angle */
				else
					angle += base_angle;	/* 90 -> 135, angle = 90 + angle */
			}
			else
			{			/* -135 -> -45 */
				angle = (float)-1.57079632679489661923;
				if (x >= 0.0)
					angle += base_angle;	/* -90 -> -45, angle = -90 + angle */
				else
					angle -= base_angle;	/* -135 -> -90, angle = -90 - angle */
			}
		}
	}
#ifdef ZERO_TO_TWOPI
	if (angle < 0)
		return (angle + TWOPI);
	else
		return (angle);
#else
	return (angle);
#endif
}
//------------------------------------------------------------------------------
#define	FM_DEMOD_COEFF1		PI/4			// Factors used in arctan approximation used in FM demodulator
#define	FM_DEMOD_COEFF2		PI*0.75
#define FM_RX_LPF_ALPHA		0.05			// For FM demodulator:  "Alpha" (low-pass) factor to result in -6dB "knee" at approx. 270 Hz
#define FM_RX_HPF_ALPHA		0.96			// For FM demodulator:  "Alpha" (high-pass) factor to result in -6dB "knee" at approx. 180 Hz

#define	FM_RX_SCALING_2K5		1e-10//33800			// Amplitude scaling factor of demodulated FM audio (normalized for +/- 2.5 kHz deviation at 1 kHZ)
#define FM_RX_SCALING_5K	(FM_RX_SCALING_2K5/2)	// Amplitude scaling factor of demodulated FM audio (normalized for +/- 5 kHz deviation at 1 kHz)

void fm_demod( COMPLEX_float *in, float *out, int size ){
  float x,y,abs_y, r,angle, a,b;
  static float i_prev, q_prev, lpf_prev, hpf_prev_a, hpf_prev_b;		// used in FM detection and low/high pass processing
  for(int i = 0; i < size/2; i++){
    y = (i_prev * in[i].re) - (in[i].im * q_prev);
    x = (i_prev * in[i].im) + (in[i].re * q_prev);
    
    abs_y = fabsf(y) + 2e-16;
    
    if(x >= 0){					// Quadrant 1 or 4
      r = (x - abs_y) / (x + abs_y);
      angle = FM_DEMOD_COEFF1 - FM_DEMOD_COEFF1 * r;
    }
    else{					// Quadrant 2 or 3
      r = (x + abs_y) / abs_y - x;
      angle = FM_DEMOD_COEFF2 - FM_DEMOD_COEFF1 * r;
    }

    if (y < 0)					// Quadrant 3 or 4 - flip sign
      angle = -angle;


    // Now do integrating low-pass filter to do FM de-emphasis
    a = lpf_prev + (FM_RX_LPF_ALPHA * (angle - lpf_prev));	
    lpf_prev = a;			        // save "[n-1]" sample for next iteration
    
    b = FM_RX_HPF_ALPHA * (hpf_prev_b + a - hpf_prev_a);	// do differentiation
    hpf_prev_a = a;		// save "[n-1]" samples for next iteration
    hpf_prev_b = b;
    
    out[i] = b*FM_RX_SCALING_2K5;
    
    q_prev = in[i].re;		// save "previous" value of each channel to allow detection of the change of angle in next go-around
    i_prev = in[i].im;
  }
}
#ifndef __LMADF_H_
#define __LMADF_H_


#define LMADF_INTERFERENCE 0
#define LMADF_NOISE 1
#define LMADF_NOSIG (-1)
#define LMADF_NOLINE (-2)
#define LMADF_NOFILT (-3)

typedef float REAL;

#pragma pack (1)
typedef struct _COMPLEX {
  float re, im;
} COMPLEX;

#pragma pack (1)
typedef struct _complex_buffer_desc {
  COMPLEX *data;
  int size, ovlp, want, have, done, mine;
} CXBuffer, *CXB;


#pragma pack (1)
typedef struct _LMSR
{
  COMPLEX *signal;			/* Signal Buffer */
  int signal_size;		/* Number of samples in signal buffer */
  REAL *delay_line;		/* Delay Line circular buffer for holding samples */
  REAL *adaptive_filter;	/* Filter coefficients */
  REAL adaptation_rate;		/* Adaptation rate for the LMS stochastic gradient */
  REAL leakage;			/* Exponential decay constant for filter coefficients */
  int adaptive_filter_size;	/* number taps in adaptive filter */
  int filter_type;		/* Filter type */
  int delay;			/* Total delay between current sample and filter */
  int delay_line_ptr;		/* Pointer for next sample into the delay line */
  int size;			/* Delay line size */
  int mask;			/* Mask for circular buffer */
} *LMSR, _lmsstate;




// just to make the algorithm itself a little clearer,
// get the admin stuff out of the way

#define ssiz (lms->signal_size)
#define asiz (lms->adaptive_filter_size)
#define dptrf (lms->delay_line_ptr)
#define rate (lms->adaptation_rate)
#define leak (lms->leakage)

#define ssig(n) (lms->signal[n].re)
#define ssig_i(n) (lms->signal[n].im)

#define dlay(n) (lms->delay_line[(n)])

#define afil(n) (lms->adaptive_filter[(n)])
#define wrap(n) (((n) + (lms->delay) + (lms->delay_line_ptr)) & (lms->mask))
#define bump(n) (((n) + (lms->mask)) & (lms->mask))





void new_lmsr_init( COMPLEX *signal, int signal_size,
	  int delay,
	  REAL adaptation_rate,
	  REAL leakage, int adaptive_filter_size, int filter_type);

void lmsr_adapt(/*LMSR lms*/void);

void lmsr_adapt_i (/*LMSR lms*/void);
void lmsr_adapt_n (void);

#endif
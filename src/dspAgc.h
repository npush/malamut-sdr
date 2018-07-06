#ifndef __DSPAGC_H_
#define __DSPAGC_H_

typedef enum _agcmode
{ 
  agcOFF=0, agcLONG, agcSLOW, agcMED, agcFAST, agcVFAST, agcLONG_TX 
} AGCMODE;

/*typedef struct _COMPLEX
{  
  float re, im;
} COMPLEX;
*/


typedef struct _dttspagc{
  int mode;  
  struct _gain  
  {    
    float top, now, fastnow, bottom, old, limit, raw, fix;  
  } gain, fastgain;  
  float attack;  
  float one_m_attack;  
  float decay;  
  float one_m_decay;  
  float slope;  
  float fastattack;  
  float one_m_fastattack;  
  float fastdecay;  
  float one_m_fastdecay;  
  float hangtime;  
  float hangthresh;  
  float fasthangtime;		  
  float *circ;  
  float *magvec;  
 // CXB buff;  
  int mask;  
  int indx;  
  int sndx;  
  int hangindex;  
  int fastindx;  
  int fasthang;			  
  
  float rx_gain_temp;
  float tx_gain_temp;
} dttspagc;//, *DTTSPAGC;

typedef struct _dttspagc_tmp{
  struct _gain_tmp  
  {    
    float top, now, fastnow, bottom, old, limit, raw, fix;  
  } gain, fastgain;  
}dttspagc_tmp;

void dspAgc_Init( void );
void dspAgc_set(int mode);
void dspAgc( float *buff, int agc_lev );
void dspAgc_setGain( unsigned short set_gain );

void adsAgc_init(void);
void adsAgc( float *buff);

#endif
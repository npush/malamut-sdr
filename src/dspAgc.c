#include "main.h"


int BufSize;
#ifdef F24
  float Limit=8000000.0f;	        // Target output
#else
  float Limit=2147000000.0f;	        // Target output
#endif
  
float attack=2.0f;		// Attack time constant in ms			 
float decay=250;		// Decay time constant in ms			 
float slope=1.0;		// Slope			 
float hangtime=250;		// Hangtime in ms			 
float samprate;	

float MinGain = 0;      	// Minimum gain as a multipler, linear not dB; 
float CurGain = 1.0;
float circ_agc[ FFT_FILTER_SIZE ];


dttspagc* a;
dttspagc agc_ram;


float dspGain;
//===============================================================================================
void dspAgc_Init( void ){

  BufSize = SdrStr.SdrSetup.FFT_Size/2;
  samprate = SdrStr.SdrSetup.Sample_Rate;
  
//  a = malloc(sizeof(dttspagc));
//  memset(a, 0, sizeof(dttspagc));

   memset((unsigned char*)&agc_ram, 0, sizeof(dttspagc));
   a=&agc_ram;
  
  a->mode = agcFAST;
  
  a->attack = (float) (1.0 - exp (-1000.0 / (attack * samprate)));
  a->one_m_attack = (float) exp (-1000.0 / (attack * samprate));
  
  a->decay = (float) (1.0 - exp (-1000.0 / (decay * samprate)));
  a->one_m_decay = (float) exp (-1000.0 / (decay * samprate));
  
  a->fastattack = (float) (1.0 - exp (-1000.0 / (0.2 * samprate)));
  a->one_m_fastattack = (float) exp (-1000.0 / (0.2 * samprate));
  
  a->fastdecay = (float) (1.0 - exp (-1000.0 / (3.0 * samprate)));
  a->one_m_fastdecay = (float) exp (-1000.0 / (3.0 * samprate));
  
  a->mask = 2 * BufSize;
  a->hangindex = a->indx = 0;
  a->hangtime = hangtime * 0.001f;
  a->hangthresh = 0.0;
  a->sndx = (int) (samprate * attack * 0.003f);
  a->fastindx = (int)(0.002f*samprate);
  a->slope = slope;
  a->gain.top = SdrStr.SdrSetup.RX_GAIN; //MaxGain;
  a->hangthresh = a->gain.bottom = MinGain;
  a->gain.fastnow = a->gain.old = a->gain.now = CurGain;
  a->rx_gain_temp = a->tx_gain_temp = 0;
  a->gain.limit = Limit;
  

  a->circ = &circ_agc[0];
  
  a->mask -= 1;
  a->fasthang = 0;
  a->fasthangtime = 0.1f*a->hangtime;
 
 
  dspAgc_set( agcSLOW );
  
  
}
//===============================================================================================
static int dspAgc_mode = 0;
void dspAgc_set(int mode){
  if( dspAgc_mode == mode )
    return;

  a->gain.top = 0;
  switch (mode)
  {
          case agcOFF:
                  a->mode = agcOFF;
                  break;
          case agcLONG:
                  a->mode = agcLONG;
                  a->hangtime = 0.75;
                  a->fasthangtime = (float) 0.075;
                  a->decay = (float) (1.0 - exp (-0.5 /samprate));
                  a->one_m_decay =(float) (1.0 - a->decay);
                  break;
          case agcSLOW:
                  a->mode = agcSLOW;
                  a->hangtime = (float) 0.5;
                  a->fasthangtime = (float) 0.05;
                  a->decay = (float) (1.0 - exp (-1000.0 / (500.0 * samprate)));
                  a->one_m_decay = (float) (1.0 - a->decay);
                  break;
          case agcMED:
                  a->mode = agcMED;
                  a->hangtime = (float) 0.25;
                  a->fasthangtime = (float) 0.025;
                  a->decay = (float) (1.0 - exp (-1000.0 / (250.0 * samprate)));
                  a->one_m_decay = (float) (1.0 - a->decay);
                  break;
          case agcFAST:
                  a->mode = agcFAST;
                  a->fasthangtime = (float) 0.01;
                  a->hangtime = (float) 0.02;
                  a->decay =(float) (1.0 - exp (-1000.0 / (100.0 * samprate)));
                  a->one_m_decay =(float) (1.0 - a->decay);
                  break;
          case agcVFAST:
                  a->mode = agcFAST;
                  a->fasthangtime = (float) 0.01;
                  a->hangtime = (float) 0.02;
                  a->decay =(float) (1.0 - exp (-1000.0 / (2.0 * samprate)));
                  a->one_m_decay =(float) (1.0 - a->decay);
                  break;
          
  }
  
  if( SdrStr.SdrMode.RTX_Mode ){
    dspAgc_setGain( SdrStr.SdrSetup.MIC_GAIN );
  }else{
    dspAgc_setGain( SdrStr.SdrSetup.RX_GAIN );
  }

  dspAgc_mode = mode;
}
//===============================================================================================
#define kDspAgc 0.00000005f
//#define kDspAgc 0.0000000005f

void dspAgc( float *buff, int agc_lev ){
  	int i;
	int hangtime = (int) (samprate * a->hangtime);
	int fasthangtime = (int) (samprate * a->fasthangtime);
	float hangthresh;

        float beta=0.7f;
        float alpha=(1-beta);        

        float tmp_agc = 0, tmp_smetr = 0;

        
        dspAgc_set( agc_lev );

	if (a->hangthresh > 0)
		hangthresh = a->gain.top * a->hangthresh + a->gain.bottom * (float) (1.0 - a->hangthresh);
	else
		hangthresh = 0.;

	if (a->mode == 0)
	{
		//for (i = 0; i <BufSize; i++)
                // buff[i]*=sGain;
		return;
	}

	for (i = 0; i <BufSize; i++)
	{
                a->circ[a->indx] =buff[i] ;
                tmp_agc = 1.4f*fabsf(a->circ[a->indx]);
                
                if( tmp_smetr < tmp_agc )
                  tmp_smetr  = tmp_agc;
                
               
		if (tmp_agc > kDspAgc )
		//if (tmp_agc != 0 )
			tmp_agc = a->gain.limit / tmp_agc;	// if not zero sample, calculate gain
		else
			tmp_agc = a->gain.now;	// update. If zero, then use old gain
		if (tmp_agc < hangthresh)
			a->hangindex = hangtime;
		if (tmp_agc >= a->gain.now)
		{
			a->gain.raw = a->one_m_decay * a->gain.now + a->decay * tmp_agc;
			if (a->hangindex++ > hangtime)
			{
				a->gain.now =a->one_m_decay * a->gain.now + a->decay * min (a->gain.top, tmp_agc);
			}
		}
		else
		{
			a->hangindex = 0;
			a->gain.raw = a->one_m_attack * a->gain.now + a->attack * tmp_agc;
			a->gain.now = a->one_m_attack * a->gain.now + a->attack * max (tmp_agc, a->gain.bottom);
		}

		
                tmp_agc = 1.4f*fabsf(a->circ[a->fastindx]);
		if (tmp_agc > kDspAgc )
		//if (tmp_agc != 0 )
			tmp_agc = a->gain.limit / tmp_agc;
		else
			tmp_agc = a->gain.fastnow;
		if (tmp_agc > a->gain.fastnow)
		{
			if (a->fasthang++ > fasthangtime)
			{
				a->gain.fastnow = min (a->one_m_fastdecay * a->gain.fastnow + a->fastdecay * min (a->gain.top, tmp_agc), a->gain.top);
			}
		}
		else
		{
			a->fasthang = 0;
			a->gain.fastnow = max (a->one_m_fastattack * a->gain.fastnow + a->fastattack * max (tmp_agc, a->gain.bottom), a->gain.bottom);
		}
		
                
                
                
                a->gain.fastnow = max (min (a->gain.fastnow, a->gain.top), a->gain.bottom);
		a->gain.now = max (min (a->gain.now, a->gain.top), a->gain.bottom);
		
                dspGain = min(a->gain.fastnow, min (a->slope * a->gain.now, a->gain.top));
                buff[i] = a->circ[a->sndx]*dspGain;
                

                /*if( fabsf(buff[i]) >Limit  ){
                  if(buff[i] > 0)
                    buff[i] = Limit;
                  else
                    buff[i] = -Limit;
                }*/
     


		a->indx = (a->indx + a->mask) & a->mask;
		a->sndx = (a->sndx + a->mask) & a->mask;
		a->fastindx = (a->fastindx + a->mask) & a->mask;
	}
        

        
        SdrStr.Smetr=(SdrStr.Smetr*beta)+(alpha*tmp_smetr/8.0);
        
}
//==============================================================================
void dspAgc_setGain( unsigned short set_gain ) {
  a->gain.top = (float)set_gain; //MaxGain;
  a->gain.fastnow = a->gain.old = a->gain.now = set_gain ;
}
//==============================================================================

//==============================================================================


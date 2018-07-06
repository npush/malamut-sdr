#include "main.h"

//------------------------------------------------------------------------------
#define anOrd (int)(32)// порядок должен быть кратен 4
#define anM_delay_max (int)(64)//задержка 88
#define anDelay_length (anM_delay_max+(FFT_FILTER_SIZE/2))
float anH[anOrd]@"CCM_RAM";
float anX[anDelay_length]@"CCM_RAM";

void ANF_filter(float* Xinp, int size)
{
  
    
  int i, cnt;
  float error, sc1, sc2, sos=0, sos_tmp=0,tmp, mu,Leakage;
  float y=0;
  float dk;

       mu = 0.00005; //notch
  Leakage = 0.000001;
  sc1= (1.0 - (mu * Leakage));

  for (i=0; i<anM_delay_max; i++)//88
    anX[i]=anX[(SdrStr.SdrSetup.FFT_Size/2)+i];//копирую последние сэмплы в начало

  for (i=anM_delay_max; i<anDelay_length; i++)//88..88+1024
    anX[i]=Xinp[i-anM_delay_max];//копирую новые сэмплы

  for (i=0; i<anOrd; i+=4)//32
    sos_tmp+=anX[i]*anX[i]+anX[i+1]*anX[i+1]+anX[i+2]*anX[i+2]+anX[i+3]*anX[i+3];

  for (cnt=0;cnt<size;cnt++)
  {

    dk=Xinp[cnt];
    y=0;
    sos=0;

    for (i=0; i<anOrd; i+=4)
      y+=anH[i]*anX[cnt+i]+anH[i+1]*anX[cnt+i+1]+ anH[i+2]*anX[cnt+i+2]+anH[i+3]*anX[cnt+i+3] ;


    if (cnt!=0)
    {
      sos_tmp-=anX[cnt-1]*anX[cnt-1];
      sos_tmp+=anX[cnt+anOrd-1]*anX[cnt+anOrd-1];
      sos=sos_tmp/anOrd;
    }
    else
    {
      sos=sos_tmp/anOrd;
    }

    sc2 =(mu / (sos + 0.0000001f));
    tmp=error = dk-y;
    error=error*sc2;
    for(i=0;i<anOrd;i+=4)
    {
      anH[i  ]= sc1*anH[i  ] + (error*anX[cnt+i]);
      anH[i+1]= sc1*anH[i+1] + (error*anX[cnt+i+1]);
      anH[i+2]= sc1*anH[i+2] + (error*anX[cnt+i+2]);
      anH[i+3]= sc1*anH[i+3] + (error*anX[cnt+i+3]);
    }

    Xinp[cnt]=tmp;


  }
}

//------------------------------------------------------------------------------


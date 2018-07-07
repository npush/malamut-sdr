#include "main.h"

//------------------------------------------------------------------------------
#define nrOrd (int)(128)// порядок должен быть кратен 4(48)
#define nrM_delay_max (int)(128)//задержка (96)
#define nrDelay_length (nrM_delay_max+(FFT_FILTER_SIZE/2))
float nrH[nrOrd] CCM_RAM;
float nrX[nrDelay_length] CCM_RAM;

void DNR_filter(float* Xinp, int size)
{
  int i, cnt;
  float error, sc1, sc2, sos=0, sos_tmp=0,mu,Leakage;
  float y=0;
  float dk;
  int nrDelay_length_set = SdrStr.SdrSetup.DNR_Del + (FFT_FILTER_SIZE/2);

  mu = SdrStr.SdrSetup.DNR_Rate; //dnr
  Leakage = SdrStr.SdrSetup.DNR_Leak;
  sc1= (1.0 - (mu * Leakage));

  for (i=0; i<SdrStr.SdrSetup.DNR_Del; i++)//88
    nrX[i]=nrX[(SdrStr.SdrSetup.FFT_Size/2)+i];//копирую последние сэмплы в начало

  for (i=SdrStr.SdrSetup.DNR_Del; i<nrDelay_length_set; i++)//88..88+1024
    nrX[i]=Xinp[i-SdrStr.SdrSetup.DNR_Del];//копирую новые сэмплы

  for (i=0; i< SdrStr.SdrSetup.DNR_Ord; i+=4)//32
    sos_tmp+=nrX[i]*nrX[i]+nrX[i+1]*nrX[i+1]+nrX[i+2]*nrX[i+2]+nrX[i+3]*nrX[i+3];

  for (cnt=0;cnt<size;cnt++)
  {

    dk=Xinp[cnt];
    y=0;
    sos=0;

    //for (i=0; i< SdrStr.SdrSetup.DNR_Ord; i+=4)
    //  y+=nrH[i]*nrX[cnt+i]+nrH[i+1]*nrX[cnt+i+1]+ nrH[i+2]*nrX[cnt+i+2]+nrH[i+3]*nrX[cnt+i+3] ;
    for (i=0; i< SdrStr.SdrSetup.DNR_Ord; ++i)
      y+=nrH[i]*nrX[cnt+i] ;


    if (cnt!=0)
    {
      sos_tmp-=nrX[cnt-1]*nrX[cnt-1];
      sos_tmp+=nrX[cnt+ SdrStr.SdrSetup.DNR_Ord-1]*nrX[cnt+ SdrStr.SdrSetup.DNR_Ord-1];
      sos=sos_tmp/ SdrStr.SdrSetup.DNR_Ord;
    }
    else
    {
      sos=sos_tmp/ SdrStr.SdrSetup.DNR_Ord;
    }

   // sc2 =(mu / (sos + 0.0000001f));
    sc2 =(mu / (sos + 1e-10));
    error = dk-y;
    error=error*sc2;
    /*for(i=0;i< SdrStr.SdrSetup.DNR_Ord;i+=4)
    {
      nrH[i  ]= sc1*nrH[i  ] + (error*nrX[cnt+i]);
      nrH[i+1]= sc1*nrH[i+1] + (error*nrX[cnt+i+1]);
      nrH[i+2]= sc1*nrH[i+2] + (error*nrX[cnt+i+2]);
      nrH[i+3]= sc1*nrH[i+3] + (error*nrX[cnt+i+3]);
    }*/
    for(i=0;i< SdrStr.SdrSetup.DNR_Ord;i+=4)
      nrH[i]= sc1*nrH[i] + (error*nrX[cnt+i]);

    Xinp[cnt]=y;

  }
}
//------------------------------------------------------------------------------
void DNR_filter_rst( void ){
  for(int i=0;i< nrOrd;++i)
    nrH[i] = 0;
  for(int i=0;i< nrDelay_length;++i)
    nrX[i] = 0;

}
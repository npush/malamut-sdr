#include "main.h"


//float                Pi_coef[FFT_FILTER_SIZE];
//float        Two_div_Pi_coef[FFT_FILTER_SIZE];
float            WindowCoefs[FFT_FILTER_SIZE/2+1];

static int old_FreqLoAM=0, old_FreqHiAM=0;
static int old_FreqLo=0,   old_FreqHi=0;
//-----------------------------------------------------------------------------
void Calculate_Window_coef( int window_type ){
  int iCnt;
  float dTmpFloat;
  int FRAMES_PER_BUFFER = SdrStr.SdrSetup.FFT_Size/2;
  dTmpFloat = 2.0 * M_PI / (float)(FRAMES_PER_BUFFER);
  switch (window_type)
  {
    case 1://окно Хэмминга
      for (iCnt = 0; iCnt < FRAMES_PER_BUFFER+1; iCnt++)
      {
        WindowCoefs[iCnt]= (float)(0.53836 - 0.46164 * cosf(dTmpFloat * iCnt));
      }
      break;

    case 2:
      for (iCnt = 0; iCnt < FRAMES_PER_BUFFER+1; iCnt++)
      {
        //окно Блэкмана-Хариса
        //WindowCoefs[iCnt]= 0.35875f-0.48829f*cosf(dTmpFloat*iCnt)+ 0.14128*cosf(2*dTmpFloat* iCnt)-0.01168f*cosf(3*dTmpFloat * iCnt);
        //Окно Блэкмана — Наталла
        WindowCoefs[iCnt]= 0.3638519-0.4891775*cosf(dTmpFloat*iCnt)+ 0.1365995*cosf(2*dTmpFloat* iCnt)-0.0106411*cosf(3*dTmpFloat * iCnt);
      }
      break;
  }

/*  for (iCnt = 0; iCnt < Exchange.FFT_Size; iCnt++)
  {
    Pi_coef[iCnt]= (float)(iCnt*M_PI);
    Two_div_Pi_coef[iCnt]=2.0f/Pi_coef[iCnt];
  }*/


  #ifdef SpectrWS  
   dTmpFloat = 2.0 * M_PI / (SdrStr.SdrSetup.FFT_Size-1);
  for (iCnt = 0; iCnt < SdrStr.SdrSetup.FFT_Size; iCnt++)
  {
    //WindowCoefs_for_Spectrum[iCnt]= 0.35875f-0.48829f* cosf(dTmpFloat * iCnt) +  0.14128*cosf(2*dTmpFloat * iCnt)-0.01168f*cosf(3*dTmpFloat * iCnt);
    //Окно Блэкмана — Наталла
    WindowCoefs_for_Spectrum[iCnt]= 0.3638519-0.4891775*cosf(dTmpFloat*iCnt)+ 0.1365995*cosf(2*dTmpFloat* iCnt)-0.0106411*cosf(3*dTmpFloat * iCnt);
  }
  #endif

}
//-----------------------------------------------------------------------------
void Calculate_FIR_coef( int FreqLo, int FreqHi ){
  

  float m_fCutLow,m_fCutHigh, deltaF, sumF, iCntPi;

  int cm;
  float k;
  int iHalfLen, iCnt;

  old_FreqLoAM = old_FreqHiAM = 0;
  
  if ((old_FreqLo==FreqLo)&&(old_FreqHi==FreqHi))//если ничего не изменилось
    return;
  else
  {
    old_FreqLo=FreqLo;
    old_FreqHi=FreqHi;
  }

  cm = 2;//20
  k = ((float)SdrStr.SdrSetup.Sample_Rate/(float)SdrStr.SdrSetup.FFT_Size)*(float)cm;
  FreqLo +=k;
  FreqHi +=k;
  
  iHalfLen = (SdrStr.SdrSetup.FFT_Size/2)/2;//512
  m_fCutLow  = (float)((FreqLo/ (float)SdrStr.SdrSetup.Sample_Rate) * 2.0);
  m_fCutHigh = (float)((FreqHi / (float)SdrStr.SdrSetup.Sample_Rate) * 2.0);
  deltaF=m_fCutHigh - m_fCutLow;
  sumF=m_fCutHigh + m_fCutLow;

  for (iCnt=0;iCnt<SdrStr.SdrSetup.FFT_Size;iCnt+=4)
  {
    Buf_float_tmp[iCnt]=0;
    Buf_float_tmp[iCnt+1]=0;
    Buf_float_tmp[iCnt+2]=0;
    Buf_float_tmp[iCnt+3]=0;
  }
  Buf_float_tmp[iHalfLen]= deltaF;//m_fCutHigh - m_fCutLow;
  deltaF=deltaF/2;
  sumF=sumF/2;
  for (iCnt = 1; iCnt <= iHalfLen; iCnt++)
  {
    //Buf_float_tmp[iHalfLen + iCnt] = (float)(Two_div_Pi_coef[iCnt]* sinf((deltaF) *Pi_coef[iCnt]) * cosf((sumF) * Pi_coef[iCnt]));
    iCntPi = (float)iCnt*M_PI;
    Buf_float_tmp[iHalfLen + iCnt] = ((2.0/iCntPi)*  sinf( deltaF * iCntPi) * cosf(sumF * iCntPi));

    Buf_float_tmp[iHalfLen - iCnt] = Buf_float_tmp[iHalfLen + iCnt];
  }

 
  
  for( int i=0;i< SdrStr.SdrSetup.FFT_Size; ++i){
      FIR_coef[i].re = 0;
      FIR_coef[i].im = 0;
  }
 
  /*------------------------------*/
  /* multiplying with a window    */
  /*------------------------------*/
  //for (iCnt = 0; iCnt < FRAMES_PER_BUFFER+1; iCnt+=4)
  for (iCnt = 0; iCnt < (SdrStr.SdrSetup.FFT_Size/2+1); ++iCnt)
  {
    Buf_float_tmp[iCnt] *=  WindowCoefs[iCnt];
    
    FIR_coef[iCnt].re = Buf_float_tmp[iCnt] ;
    FIR_coef[iCnt].im = Buf_float_tmp[iCnt] ;
  }

    arm_cfft_f32(CFFT_init_struct, (float*)&FIR_coef,0,1);
  
  for( int i=0;i< SdrStr.SdrSetup.FFT_Size/2 ; ++i){
    FIR_coef[i].re = 0;
    FIR_coef[i].im = 0;
  }
  for( int i=0;i < cm ; ++i){
    FIR_coef[i].re = FIR_coef[SdrStr.SdrSetup.FFT_Size-cm+i].re;
    FIR_coef[i].im = FIR_coef[SdrStr.SdrSetup.FFT_Size-cm+i].im;
  }

  for( int i=0;i< SdrStr.SdrSetup.FFT_Size/2-cm ; ++i){
      FIR_coef[SdrStr.SdrSetup.FFT_Size-1-i].re = FIR_coef[SdrStr.SdrSetup.FFT_Size-1-i-cm].re ;
      FIR_coef[SdrStr.SdrSetup.FFT_Size-1-i-cm].re = 0;

      FIR_coef[SdrStr.SdrSetup.FFT_Size-1-i].im = FIR_coef[SdrStr.SdrSetup.FFT_Size-1-i-cm].im ;
      FIR_coef[SdrStr.SdrSetup.FFT_Size-1-i-cm].im = 0;
  }    
  
  
  //test
  k = FreqHi/((float)SdrStr.SdrSetup.Sample_Rate/(float)SdrStr.SdrSetup.FFT_Size);
  k*=2;
   for( int i=SdrStr.SdrSetup.FFT_Size/2+cm;i< SdrStr.SdrSetup.FFT_Size-k ; ++i){
    FIR_coef[i].re = 0;
    FIR_coef[i].im = 0;
  }  
  
}
//-----------------------------------------------------------------------------
void Calculate_FIRam_coef( int FreqLo, int FreqHi ){
  

  float m_fCutLow,m_fCutHigh, deltaF, sumF, iCntPi;

  int iHalfLen, iCnt;

  old_FreqLo = old_FreqHi = 0;
  
  if ((old_FreqLoAM==FreqLo)&&(old_FreqHiAM==FreqHi))//если ничего не изменилось
    return;
  else
  {
    old_FreqLoAM=FreqLo;
    old_FreqHiAM=FreqHi;
  }


  
  iHalfLen = (SdrStr.SdrSetup.FFT_Size/2)/2;//512
  m_fCutLow  = (float)((FreqLo/ (float)SdrStr.SdrSetup.Sample_Rate) * 2.0);
  m_fCutHigh = (float)((FreqHi / (float)SdrStr.SdrSetup.Sample_Rate) * 2.0);
  deltaF=m_fCutHigh - m_fCutLow;
  sumF=m_fCutHigh + m_fCutLow;

  for (iCnt=0;iCnt<SdrStr.SdrSetup.FFT_Size;iCnt+=4)
  {
    Buf_float_tmp[iCnt]=0;
    Buf_float_tmp[iCnt+1]=0;
    Buf_float_tmp[iCnt+2]=0;
    Buf_float_tmp[iCnt+3]=0;
  }
  Buf_float_tmp[iHalfLen]= deltaF;//m_fCutHigh - m_fCutLow;
  deltaF=deltaF/2;
  sumF=sumF/2;
  for (iCnt = 1; iCnt <= iHalfLen; iCnt++)
  {
    //Buf_float_tmp[iHalfLen + iCnt] = (float)(Two_div_Pi_coef[iCnt]* sinf((deltaF) *Pi_coef[iCnt]) * cosf((sumF) * Pi_coef[iCnt]));
    iCntPi = (float)iCnt*M_PI;
    Buf_float_tmp[iHalfLen + iCnt] = ((2.0/iCntPi)*  sinf( deltaF * iCntPi) * cosf(sumF * iCntPi));
    Buf_float_tmp[iHalfLen - iCnt] = Buf_float_tmp[iHalfLen + iCnt];
  }

  memset( (unsigned char*)&FIR_coef, 0, sizeof( FIR_coef ) );
  
  for (iCnt = 0; iCnt < (SdrStr.SdrSetup.FFT_Size/2+1); ++iCnt)
  {
    Buf_float_tmp[iCnt] *=  WindowCoefs[iCnt];
    
    FIR_coef[iCnt].re = Buf_float_tmp[iCnt] ;
    FIR_coef[iCnt].im = Buf_float_tmp[iCnt] ;
  }

  arm_cfft_f32(CFFT_init_struct, (float*)&FIR_coef,0,1);

}
//-----------------------------------------------------------------------------
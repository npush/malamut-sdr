#include "main.h"
//#include "Windish.h"

float Mag_avg[8];


//-----------------------------------------------------------------------------
void SDR_RX( void ){
  
 static float am_mean;
 float am_mean_calc;
 float am_beta = 0.90;
 float am_alpha = 1.0-am_beta;
      
  Get_New_Pointer();
  Cpy_New_data(0);
  
 
  
  //*********************************************
  if( SdrStr.SdrMode.NB )
    noiseblanker( &cfft_buffer[ SdrStr.SdrSetup.FFT_Size/2 ], SdrStr.SdrSetup.FFT_Size/2,  SdrStr.SdrMode.NB, SdrStr.SdrMode.NB_LEVEL   );
  //*********************************************
  //IQ_imbalance_compensation (2);
   
  Cpy_Old_data();
  
  
  //*********************************************
  #ifdef SpectrWS
    Calc_Spec_Mag( cfft_buffer );
    if (SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]!=FM)
      arm_cfft_f32(CFFT_init_struct,(float*)&cfft_buffer,0,1);
  #else
    arm_cfft_f32(CFFT_init_struct,(float*)&cfft_buffer,0,1);
    Calc_Spec_Mag( cfft_buffer );//2.6ms 2048/48k
  #endif
  //*********************************************
  

  
  //*********************************************
  if (SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] < AM ){//SSB (2047..1024LSB/0..1023-USB
    Calculate_FIR_coef( SdrStr.SdrSetup.FILTER_SSB[SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]][0], SdrStr.SdrSetup.FILTER_SSB[SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]][1] );
    arm_cmplx_mult_cmplx_f32((float*)&cfft_buffer,(float*)&FIR_coef,(float*)&cfft_buffer, SdrStr.SdrSetup.FFT_Size);
    arm_cfft_f32(CFFT_init_struct,(float*)&cfft_buffer,1,1);

    int start =  SdrStr.SdrSetup.FFT_Size/2;
    for( int i=0; i<  SdrStr.SdrSetup.FFT_Size/2; ++i )
      Buf_float_tmp[i] = cfft_buffer[i+start].im;
  }
  //*********************************************
  
  //*********************************************
   /*LSB  -F=[1024...2047]=F0=[0...1023]=F+  */

  if (SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==AM){//AM
    Calculate_FIRam_coef( 0, SdrStr.SdrSetup.FILTER_AMFM[SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ]] );
    arm_cmplx_mult_cmplx_f32((float*)&cfft_buffer,(float*)&FIR_coef,(float*)&cfft_buffer, SdrStr.SdrSetup.FFT_Size);
    arm_cfft_f32(CFFT_init_struct,(float*)&cfft_buffer,1,1); 
   
    arm_cmplx_mag_f32((float*)&cfft_buffer[SdrStr.SdrSetup.FFT_Size/2],  Buf_float_tmp,  SdrStr.SdrSetup.FFT_Size/2 );
    arm_mean_f32(Buf_float_tmp, SdrStr.SdrSetup.FFT_Size/2, &am_mean_calc );
    am_mean = am_mean*am_beta + am_mean_calc*am_alpha;
    //SdrStr.SdrMode.fTmp = am_mean;
    arm_offset_f32( Buf_float_tmp,-am_mean, Buf_float_tmp,SdrStr.SdrSetup.FFT_Size/2 );
  }
  //*********************************************

  //*********************************************
  if (SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==FM){//FM
    if(SdrStr.FmPll_Init){
      //init_pll ((float)SdrStr.SdrSetup.Sample_Rate, 0.0, -24000.0f, 24000.0f, (float)(SdrStr.SdrSetup.FILTER_AMFM[SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ]]) );
      init_pll ((float)SdrStr.SdrSetup.Sample_Rate, 0.0, (float)SdrStr.SdrSetup.Sample_Rate/(-2.0),  (float)SdrStr.SdrSetup.Sample_Rate/2.0, (float)(SdrStr.SdrSetup.FILTER_AMFM[SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ]]) );
      SdrStr.FmPll_Init = 0;
    }


    Calculate_FIRam_coef( 0, SdrStr.SdrSetup.FILTER_AMFM[SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ]] );
    arm_cmplx_mult_cmplx_f32((float*)&cfft_buffer,(float*)&FIR_coef,(float*)&cfft_buffer, SdrStr.SdrSetup.FFT_Size);
    arm_cfft_f32(CFFT_init_struct,(float*)&cfft_buffer,1,1);
    
    Get_FM_PLL_demodulation( cfft_buffer, Buf_float_tmp); 
  }

  if (SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==FM2){//FM
    if(SdrStr.FmPll_Init){
      init_pll ((float)SdrStr.SdrSetup.Sample_Rate, 0.0, -4000.0f, 4000.0f, (float)(SdrStr.SdrSetup.FILTER_AMFM[SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ]]) );
      SdrStr.FmPll_Init = 0;
    }

    Calculate_FIR_coef( 0, SdrStr.SdrSetup.FILTER_AMFM[SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ]] );
    arm_cmplx_mult_cmplx_f32((float*)&cfft_buffer,(float*)&FIR_coef,(float*)&cfft_buffer, SdrStr.SdrSetup.FFT_Size);
    arm_cfft_f32(CFFT_init_struct,(float*)&cfft_buffer,1,1);
    Get_FM_PLL_demodulation( cfft_buffer, Buf_float_tmp);
  }
  //*********************************************
  
  
  



  
  //*********************************************
  if(SdrStr.SdrMode.ANF )
    ANF_filter( Buf_float_tmp, (SdrStr.SdrSetup.FFT_Size/2) );//3.6ms 2048/48k
  //*********************************************

  //*********************************************
  if( SdrStr.SdrMode.DNR )
    DNR_filter( Buf_float_tmp, (SdrStr.SdrSetup.FFT_Size/2) );//5.6ms 2048/48k
  //*********************************************
  
  
  //*********************************************
  dspAgc(Buf_float_tmp, SdrStr.SdrMode.AGC_RX[ SdrStr.SdrMode.Band ]);  
         
  //*********************************************

  //*********************************************
  //Init_Equal(Exchange.RX_SEQ);
  if(SdrStr.SdrMode.RX_EQ){
    Equal(Buf_float_tmp, Buf_float_tmp  );//4.0ms2048@216MHz
  }
  //*********************************************
  
  
  for( int i=0; i< SdrStr.SdrSetup.FFT_Size/2; ++i ){
    //(pOUT+i)->re = 0xA000000A;
    //(pOUT+i)->im = 0xe0000007;
    
    if( fabsf(Buf_float_tmp[i]) >8388607  ){
      if(Buf_float_tmp[i] > 0)
        Buf_float_tmp[i] = 8388607;
      else
        Buf_float_tmp[i] = -8388607;
    }
    (pOUT+i)->re = (signed long)(Buf_float_tmp[i] );
    (pOUT+i)->im = (pOUT+i)->re ;
  }
  Prepare_buff_i2s_format( (COMPLEX_long*)pOUT, SdrStr.SdrSetup.FFT_Size/2 , 1);
}
//-----------------------------------------------------------------------------




void  Calc_Spec_Mag( COMPLEX_float* input ){

  /*if( nMag !=0 )
    return;*/

  unsigned char p=0;
  unsigned int s, spScale,n,k,e;
  float tmp;
  float beta_up=SdrStr.SdrSetup.FFT_Spect_beta_up;
  float alpha_up=1-beta_up;
  float beta_dwn=SdrStr.SdrSetup.FFT_Spect_beta_dwn;
  float alpha_dwn=1-beta_dwn;
  
  if( SdrStr.SdrSetup.FFT_Size == 2048 )
    s = 8;
  if( SdrStr.SdrSetup.FFT_Size == 1024 )
    s = 4;
  if( SdrStr.SdrSetup.FFT_Size == 512 )
    s = 2;
  if( SdrStr.SdrSetup.FFT_Size == 256 )
    s = 1;
  //****************************  
  #ifdef SpectrWS
  arm_cmplx_mult_real_f32( (float*)input, WindowCoefs_for_Spectrum, (float*)Tmp_Pwr, SdrStr.SdrSetup.FFT_Size );
  arm_cfft_f32(CFFT_init_struct, (float*)&Tmp_Pwr,0,1);
  #endif
  //****************************  
  

  spScale = SdrStr.SdrSetup.SpecScale;
  if( spScale > s )
    spScale = s;

  /*LSB  -F=[1024...2047]=F0=[0...1023]=F+  */
  
  //n = 1024;
  //e = Exchange.FFT_Size;
  
  n = SdrStr.SdrSetup.FFT_Size - SdrStr.SdrSetup.FFT_Size/2/spScale;
  e = SdrStr.SdrSetup.FFT_Size/spScale;
  s /=spScale;
 
    
  k = SdrStr.SdrSetup.FFT_Size;
   
  for (int i=0;i<e;i++)
    {

      #ifdef SpectrWS
        Mag_avg[(i&(s-1))] = (Tmp_Pwr[n].im*Tmp_Pwr[n].im+Tmp_Pwr[n].re*Tmp_Pwr[n].re);
      #else
        Mag_avg[(i&(s-1))] = (input[n].im*input[n].im+input[n].re*input[n].re);
      #endif
      n++;
      if( n==k )
        n = 0;
     
     
      if( (i&(s-1))==(s-1) ){
        
        if(SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==USB)
          p = 255-(unsigned char)(i/s);
        else
          p = (unsigned char)(i/s);

        tmp = 0;
        for( int j=0;j<s;++j )
         tmp = MAX(tmp, Mag_avg[j] );

        tmp = sqrtf(tmp);
        
        tmp=(10.0f*log10f(tmp));
        if( SdrStr.SdrSetup.FFT_Spect_ofset == 0 ){
          #ifdef F24
            tmp-=49.0f;
          #else      
            tmp-=70.0f;
          #endif
        }else
          tmp-=SdrStr.SdrSetup.FFT_Spect_ofset;
          
        
        // 2147483647  * fft/2  F32
       // tmp=(20.74f*log10f(tmp));
       //  8388607  * fft/2  F24
       // tmp=(25.76f*log10f(tmp));
        
        
        if( tmp < 0)
          tmp = 0;
        tmp *= 5.0;
        if( tmp>255 )
          tmp = 255;      
          
        SdrStr.Spectr[p] = (unsigned char)tmp;
        if( SdrStr.Spectr[p] > SdrStr.Spectr_avg[p] )
          SdrStr.Spectr_avg[p] =  (unsigned char)((float)SdrStr.Spectr_avg[p]*beta_up+(float) SdrStr.Spectr[p]*alpha_up);
        else
          SdrStr.Spectr_avg[p] =  (unsigned char)((float)SdrStr.Spectr_avg[p]*beta_dwn+(float) SdrStr.Spectr[p]*alpha_dwn);
        //p++;
      }
    }
  
 /*}else{
    
    for (int i=0;i<128;i++)
    {
      tmp = sqrtf(input[i].im*input[i].im+input[i].re*input[i].re);
      tmp=(10.0f*log10f(tmp));
      tmp-=74.0f;
      if( tmp < 0)
        tmp = 0;
      tmp *= 5.0;
      Mag_avg[0] = tmp;

      if(TRX_mode.Mode[ TRX_mode.Band ]==USB)
        p = 255-(unsigned char)(128+i);
      else
        p = (unsigned char)(128+i);
      SMag[p] = 0;
      SMag[p] = MAX( SMag[p], (unsigned char)Mag_avg[0] );
      SMag_avg[p] =  (unsigned char)((float)SMag_avg[p]*beta+(float)SMag[p]*alpha);
    }
    
    for (int i=128;i<256;i++)
    {
      s=2048-128+(i-128);
      tmp = sqrtf(input[s].im*input[s].im+input[s].re*input[s].re);
      tmp=(10.0f*log10f(tmp));
      tmp-=74.0f;
      if( tmp < 0)
        tmp = 0;
      tmp *= 5.0;
      Mag_avg[0] = tmp;

      if(TRX_mode.Mode[ TRX_mode.Band ]==USB)
        p = 255-(unsigned char)(128+i);
      else
        p = (unsigned char)(128+i);
      SMag[p] = 0;
      SMag[p] = MAX( SMag[p], (unsigned char)Mag_avg[0] );
      SMag_avg[p] =  (unsigned char)((float)SMag_avg[p]*beta+(float)SMag[p]*alpha);
     } 
    
  }*/
  

  
   //nMag = 1;
}

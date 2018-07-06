#include "main.h"


float *echo_in;
float *echo_out;
  
int del; 



//-----------------------------------------------------------------------------  
void echo_init( void ){
  
  del = SdrStr.SdrMode.Echo_del;
  echo_in = &Buf_float_tmp2[SdrStr.SdrMode.Echo_del];
  echo_out = &Buf_float_tmp2[0];

  
  //echo_in = &Buf_float_tmp2[0];
  //echo_out = &Buf_float_tmp2[1];
    
  memset( (unsigned char*)&Buf_float_tmp2, 0, sizeof( Buf_float_tmp2 ) );
}
//-----------------------------------------------------------------------------  
void echo_proc( void ){
  int k;
  float input_temp =0;

  k = SdrStr.SdrMode.Echo_del;
  if( del !=  k){
    /*echo_in = &Buf_float_tmp2[SdrStr.SdrMode.Echo_del];
    echo_out = &Buf_float_tmp2[0];
    memset( (unsigned char*)&Buf_float_tmp2, 0, sizeof( Buf_float_tmp2 ) );*/

    echo_in+=(k-del);
    if( echo_in > &Buf_float_tmp2[Buf_float_tmp2_size-1]  )
      echo_in -= (int)(&Buf_float_tmp2[0]);
    if( echo_in < &Buf_float_tmp2[0]  )
      echo_in += (int)(&Buf_float_tmp2[0]);
    
    del = k;
  }
  

  k = SdrStr.SdrSetup.FFT_Size/2;
  
  for( int i=0; i< k; ++i ){


    //----------------
    input_temp = Buf_float_tmp[i];

    Buf_float_tmp[i] = input_temp*(1.0-SdrStr.SdrMode.Echo_vol) + (*echo_out)*SdrStr.SdrMode.Echo_vol ;//mic input
    *echo_in++ =  (input_temp+(*echo_out++)*SdrStr.SdrMode.Echo_rpt) ;

   //----------------
    if( echo_out > &Buf_float_tmp2[Buf_float_tmp2_size-1]  )
      echo_out = &Buf_float_tmp2[0];
    if( echo_in > &Buf_float_tmp2[Buf_float_tmp2_size-1]  )
      echo_in = &Buf_float_tmp2[0];
    
    /*
    *echo_out++ = Buf_float_tmp[i];
      
    if( echo_out > &Buf_float_tmp2[SdrStr.SdrMode.Echo_del-1]  )
      echo_out = &Buf_float_tmp2[0];
    if( echo_in > &Buf_float_tmp2[SdrStr.SdrMode.Echo_del-1]  )
      echo_in = &Buf_float_tmp2[0];

    Buf_float_tmp[i] =  *echo_in;
    half++;
    if( half == 2 ){
    echo_in++;
    half = 0;
    }*/
    
  }
}    
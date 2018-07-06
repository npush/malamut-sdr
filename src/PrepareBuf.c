#include "main.h"

//-----------------------------------------------------------------------------
void Get_New_Pointer( void ){
  if(  need_next == 2 ){
    pIN = &ADC_buff[ 0 ];
    pOUT = &DAC_buff[ 0 ];
  }
  else{
    pIN = &ADC_buff[ SdrStr.SdrSetup.FFT_Size/2 ];
    pOUT = &DAC_buff[ SdrStr.SdrSetup.FFT_Size/2 ];
  }
}
//-----------------------------------------------------------------------------
void Cpy_New_data(int txm){
   
  float magn = SdrStr.SdrSetup.rx_mag[ SdrStr.SdrMode.Band ];
  float ph = SdrStr.SdrSetup.rx_ph[ SdrStr.SdrMode.Band ];
  int  point = SdrStr.SdrSetup.FFT_Size/2;
  signed int k;
  
  if( SdrStr.SdrMode.RTX_Mode==1 ){
    magn = 1.0;
    ph=0.0;
  }
  
  Prepare_buff_i2s_format( (COMPLEX_long*)pIN, point, 0 );

  k = 1;
  if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==USB )
    k=-1;
  for( int i=0; i< point; ++i ){
    cfft_buffer[i+point].re = (float)((pIN+i)->re)*magn + (float)((pIN+i)->im)*ph;
    cfft_buffer[i+point].im = (float)( ((pIN+i)->im)*k );
  }
  
 /* if( txm ){
    for( int i=0; i< point; ++i ){
        cfft_buffer[i+point].im = 0;
    }
  }*/
  
  /*for( int i=0; i< point; ++i ){
    cfft_buffer[i+point].re = (float)((pIN+i)->re)*magn + (float)((pIN+i)->im)*ph;
    if( txm ){
        cfft_buffer[i+point].im = 0;
    }else{
      if( Exchange.Mode==USB )
        cfft_buffer[i+point].im = (float)((pIN+i)->im)*(-1.0);
      else
        cfft_buffer[i+point].im = (float)((pIN+i)->im);
    }
  }*/
}
//-----------------------------------------------------------------------------
void Cpy_Old_data( void ){
  int point = SdrStr.SdrSetup.FFT_Size/2;
  for( int i=0; i< point; ++i ){
    cfft_buffer[i].re = cfft_buffer_old[i].re;
    cfft_buffer[i].im = cfft_buffer_old[i].im;
    cfft_buffer_old[i].re = cfft_buffer[i+point].re ;
    cfft_buffer_old[i].im = cfft_buffer[i+point].im ;
  }
}
//-----------------------------------------------------------------------------
void Prepare_buff_i2s_format( COMPLEX_long *buf, int size , int type){
  
 // return;
 // 0  - CS4272 to int 
 // 1  - int to CS4272
 unsigned long tmp;
  for( int i = 0; i< size; ++i ){

    if( type == 0 ){//in
#ifdef F24
      //tmp=((buf[i].re<<8)&0x00FFFF00)|((buf[i].re>>24)&0x000000FF);
      tmp=((buf[i].re& 0xFFFF)<<8)|((buf[i].re & 0xFFFF0000)>>24);
      if ((tmp&0x800000)==0x800000)
        buf[i].re=(tmp|0xFF000000);
      else 
        buf[i].re=tmp;

      //tmp=((buf[i].im<<8)&0x00FFFF00)|((buf[i].im>>24)&0x000000FF);
      tmp=((buf[i].im& 0xFFFF)<<8)|((buf[i].im & 0xFFFF0000)>>24);
      if ((tmp&0x800000)==0x800000)
        buf[i].im=(tmp|0xFF000000);
      else 
        buf[i].im=tmp;
      
      /*if(SdrStr.SdrMode.Moni){//test 16bit
        buf[i].re/=8;
        buf[i].im/=8;
      }*/
#else
      tmp=((buf[i].re& 0xFFFF)<<16)|((buf[i].re & 0xFFFF0000)>>16);
      buf[i].re=tmp;
      tmp=((buf[i].im& 0xFFFF)<<16)|((buf[i].im & 0xFFFF0000)>>16);
      buf[i].im=tmp;
#endif      
    }
    else{//out

#ifdef F24
      tmp = buf[i].re;
      buf[i].re = ((tmp<<24)&0xFF000000)|((tmp>>8)&0x0000FFFF) ;
      tmp = buf[i].im;
      buf[i].im = ((tmp<<24)&0xFF000000)|((tmp>>8)&0x0000FFFF) ;
#else
      tmp=((buf[i].re& 0xFFFF)<<16)|((buf[i].re & 0xFFFF0000)>>16);
      buf[i].re=tmp;
      tmp=((buf[i].im& 0xFFFF)<<16)|((buf[i].im & 0xFFFF0000)>>16);
      buf[i].im=tmp;
#endif      


    }
  }
}
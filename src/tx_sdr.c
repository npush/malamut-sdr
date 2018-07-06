#include "main.h"



  static unsigned long fm_mod_accum = 0;
  static unsigned short fm_mod_idx = 0;
    
#define FM_DDS_TBL_SIZE		1024

const short FM_DDS_TABLE[FM_DDS_TBL_SIZE] = {

		// 1024 points
		0,
		201, 402, 603, 804, 1005, 1206, 1406, 1607, 1808, 2009, 2209, 2410, 2610, 2811, 3011,
		3211, 3411, 3611, 3811, 4011, 4210, 4409, 4608, 4807, 5006, 5205, 5403, 5601, 5799, 5997, 6195,
		6392, 6589, 6786, 6982, 7179, 7375, 7571, 7766, 7961, 8156, 8351, 8545, 8739, 8932, 9126, 9319,
		9511, 9703, 9895, 10087, 10278, 10469, 10659, 10849, 11038, 11227, 11416, 11604, 11792, 11980, 12166, 12353,
		12539, 12724, 12909, 13094, 13278, 13462, 13645, 13827, 14009, 14191, 14372, 14552, 14732, 14911, 15090, 15268,
		15446, 15623, 15799, 15975, 16150, 16325, 16499, 16672, 16845, 17017, 17189, 17360, 17530, 17699, 17868, 18036,
		18204, 18371, 18537, 18702, 18867, 19031, 19194, 19357, 19519, 19680, 19840, 20000, 20159, 20317, 20474, 20631,
		20787, 20942, 21096, 21249, 21402, 21554, 21705, 21855, 22004, 22153, 22301, 22448, 22594, 22739, 22883, 23027,
		23169, 23311, 23452, 23592, 23731, 23869, 24006, 24143, 24278, 24413, 24546, 24679, 24811, 24942, 25072, 25201,
		25329, 25456, 25582, 25707, 25831, 25954, 26077, 26198, 26318, 26437, 26556, 26673, 26789, 26905, 27019, 27132,
		27244, 27355, 27466, 27575, 27683, 27790, 27896, 28001, 28105, 28208, 28309, 28410, 28510, 28608, 28706, 28802,
		28897, 28992, 29085, 29177, 29268, 29358, 29446, 29534, 29621, 29706, 29790, 29873, 29955, 30036, 30116, 30195,
		30272, 30349, 30424, 30498, 30571, 30643, 30713, 30783, 30851, 30918, 30984, 31049, 31113, 31175, 31236, 31297,
		31356, 31413, 31470, 31525, 31580, 31633, 31684, 31735, 31785, 31833, 31880, 31926, 31970, 32014, 32056, 32097,
		32137, 32176, 32213, 32249, 32284, 32318, 32350, 32382, 32412, 32441, 32468, 32495, 32520, 32544, 32567, 32588,
		32609, 32628, 32646, 32662, 32678, 32692, 32705, 32717, 32727, 32736, 32744, 32751, 32757, 32761, 32764, 32766,
		32766, 32766, 32764, 32761, 32757, 32751, 32744, 32736, 32727, 32717, 32705, 32692, 32678, 32662, 32646, 32628,
		32609, 32588, 32567, 32544, 32520, 32495, 32468, 32441, 32412, 32382, 32350, 32318, 32284, 32249, 32213, 32176,
		32137, 32097, 32056, 32014, 31970, 31926, 31880, 31833, 31785, 31735, 31684, 31633, 31580, 31525, 31470, 31413,
		31356, 31297, 31236, 31175, 31113, 31049, 30984, 30918, 30851, 30783, 30713, 30643, 30571, 30498, 30424, 30349,
		30272, 30195, 30116, 30036, 29955, 29873, 29790, 29706, 29621, 29534, 29446, 29358, 29268, 29177, 29085, 28992,
		28897, 28802, 28706, 28608, 28510, 28410, 28309, 28208, 28105, 28001, 27896, 27790, 27683, 27575, 27466, 27355,
		27244, 27132, 27019, 26905, 26789, 26673, 26556, 26437, 26318, 26198, 26077, 25954, 25831, 25707, 25582, 25456,
		25329, 25201, 25072, 24942, 24811, 24679, 24546, 24413, 24278, 24143, 24006, 23869, 23731, 23592, 23452, 23311,
		23169, 23027, 22883, 22739, 22594, 22448, 22301, 22153, 22004, 21855, 21705, 21554, 21402, 21249, 21096, 20942,
		20787, 20631, 20474, 20317, 20159, 20000, 19840, 19680, 19519, 19357, 19194, 19031, 18867, 18702, 18537, 18371,
		18204, 18036, 17868, 17699, 17530, 17360, 17189, 17017, 16845, 16672, 16499, 16325, 16150, 15975, 15799, 15623,
		15446, 15268, 15090, 14911, 14732, 14552, 14372, 14191, 14009, 13827, 13645, 13462, 13278, 13094, 12909, 12724,
		12539, 12353, 12166, 11980, 11792, 11604, 11416, 11227, 11038, 10849, 10659, 10469, 10278, 10087, 9895, 9703,
		9511, 9319, 9126, 8932, 8739, 8545, 8351, 8156, 7961, 7766, 7571, 7375, 7179, 6982, 6786, 6589,
		6392, 6195, 5997, 5799, 5601, 5403, 5205, 5006, 4807, 4608, 4409, 4210, 4011, 3811, 3611, 3411,
		3211, 3011, 2811, 2610, 2410, 2209, 2009, 1808, 1607, 1406, 1206, 1005, 804, 603, 402, 201,
		0,
		-201, -402, -603, -804, -1005, -1206, -1406, -1607, -1808, -2009, -2209, -2410, -2610, -2811, -3011,
		-3211, -3411, -3611, -3811, -4011, -4210, -4409, -4608, -4807, -5006, -5205, -5403, -5601, -5799, -5997, -6195,
		-6392, -6589, -6786, -6982, -7179, -7375, -7571, -7766, -7961, -8156, -8351, -8545, -8739, -8932, -9126, -9319,
		-9511, -9703, -9895, -10087, -10278, -10469, -10659, -10849, -11038, -11227, -11416, -11604, -11792, -11980, -12166, -12353,
		-12539, -12724, -12909, -13094, -13278, -13462, -13645, -13827, -14009, -14191, -14372, -14552, -14732, -14911, -15090, -15268,
		-15446, -15623, -15799, -15975, -16150, -16325, -16499, -16672, -16845, -17017, -17189, -17360, -17530, -17699, -17868, -18036,
		-18204, -18371, -18537, -18702, -18867, -19031, -19194, -19357, -19519, -19680, -19840, -20000, -20159, -20317, -20474, -20631,
		-20787, -20942, -21096, -21249, -21402, -21554, -21705, -21855, -22004, -22153, -22301, -22448, -22594, -22739, -22883, -23027,
		-23169, -23311, -23452, -23592, -23731, -23869, -24006, -24143, -24278, -24413, -24546, -24679, -24811, -24942, -25072, -25201,
		-25329, -25456, -25582, -25707, -25831, -25954, -26077, -26198, -26318, -26437, -26556, -26673, -26789, -26905, -27019, -27132,
		-27244, -27355, -27466, -27575, -27683, -27790, -27896, -28001, -28105, -28208, -28309, -28410, -28510, -28608, -28706, -28802,
		-28897, -28992, -29085, -29177, -29268, -29358, -29446, -29534, -29621, -29706, -29790, -29873, -29955, -30036, -30116, -30195,
		-30272, -30349, -30424, -30498, -30571, -30643, -30713, -30783, -30851, -30918, -30984, -31049, -31113, -31175, -31236, -31297,
		-31356, -31413, -31470, -31525, -31580, -31633, -31684, -31735, -31785, -31833, -31880, -31926, -31970, -32014, -32056, -32097,
		-32137, -32176, -32213, -32249, -32284, -32318, -32350, -32382, -32412, -32441, -32468, -32495, -32520, -32544, -32567, -32588,
		-32609, -32628, -32646, -32662, -32678, -32692, -32705, -32717, -32727, -32736, -32744, -32751, -32757, -32761, -32764, -32766,
		-32766, -32766, -32764, -32761, -32757, -32751, -32744, -32736, -32727, -32717, -32705, -32692, -32678, -32662, -32646, -32628,
		-32609, -32588, -32567, -32544, -32520, -32495, -32468, -32441, -32412, -32382, -32350, -32318, -32284, -32249, -32213, -32176,
		-32137, -32097, -32056, -32014, -31970, -31926, -31880, -31833, -31785, -31735, -31684, -31633, -31580, -31525, -31470, -31413,
		-31356, -31297, -31236, -31175, -31113, -31049, -30984, -30918, -30851, -30783, -30713, -30643, -30571, -30498, -30424, -30349,
		-30272, -30195, -30116, -30036, -29955, -29873, -29790, -29706, -29621, -29534, -29446, -29358, -29268, -29177, -29085, -28992,
		-28897, -28802, -28706, -28608, -28510, -28410, -28309, -28208, -28105, -28001, -27896, -27790, -27683, -27575, -27466, -27355,
		-27244, -27132, -27019, -26905, -26789, -26673, -26556, -26437, -26318, -26198, -26077, -25954, -25831, -25707, -25582, -25456,
		-25329, -25201, -25072, -24942, -24811, -24679, -24546, -24413, -24278, -24143, -24006, -23869, -23731, -23592, -23452, -23311,
		-23169, -23027, -22883, -22739, -22594, -22448, -22301, -22153, -22004, -21855, -21705, -21554, -21402, -21249, -21096, -20942,
		-20787, -20631, -20474, -20317, -20159, -20000, -19840, -19680, -19519, -19357, -19194, -19031, -18867, -18702, -18537, -18371,
		-18204, -18036, -17868, -17699, -17530, -17360, -17189, -17017, -16845, -16672, -16499, -16325, -16150, -15975, -15799, -15623,
		-15446, -15268, -15090, -14911, -14732, -14552, -14372, -14191, -14009, -13827, -13645, -13462, -13278, -13094, -12909, -12724,
		-12539, -12353, -12166, -11980, -11792, -11604, -11416, -11227, -11038, -10849, -10659, -10469, -10278, -10087, -9895, -9703,
		-9511, -9319, -9126, -8932, -8739, -8545, -8351, -8156, -7961, -7766, -7571, -7375, -7179, -6982, -6786, -6589,
		-6392, -6195, -5997, -5799, -5601, -5403, -5205, -5006, -4807, -4608, -4409, -4210, -4011, -3811, -3611, -3411,
		-3211, -3011, -2811, -2610, -2410, -2209, -2009, -1808, -1607, -1406, -1206, -1005, -804, -603, -402, -201
};


/*
ADC (5.6 Dif) 6.0 Vp-p ??? (2*2.8)
DAC (5.0 Dif)2.5 Vp-p
*/

// max24/2 +-8388607*FFTsize  = max 17179867136
// c^2 =a^2+b^2 12148000551

#ifdef F24
  #define tone_car_magn 1.2148000551e+10//  c^2 =a^2+b^2 856 mVrms(max)
#else
  #define tone_car_magn 3.0E+12
#endif
  
//float am_car_magn = -1.8E+12;//6-2.5V(max dac) 1.8-1.25V F32

float am_car_magn = 6.0E+9;// 410 mVrms(max) F24 if( re & im = 0)




/*

void SDR_TX_DTONE( void ){
  static unsigned char chet = 0;
  signed long mul = 1;
  float magn = SdrStr.SdrSetup.tx_mag[ SdrStr.SdrMode.Band ];
  float ph = SdrStr.SdrSetup.tx_ph[ SdrStr.SdrMode.Band ];

  Get_New_Pointer();
  
  if (SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] < AM ){//SSB
    if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==USB  )
     mul = 1;
    else
     mul = -1;
  }

  memset((float*)&cfft_buffer,0,sizeof( cfft_buffer ));
  int f = (SdrStr.SdrSetup.FILTER_SSB[SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]][0]+SdrStr.SdrSetup.FILTER_SSB[SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]][1])/2;
  int k = f /(  SdrStr.SdrSetup.Sample_Rate/ SdrStr.SdrSetup.FFT_Size);
  //if( (chet++)&1 )
  if( k&0x01 )
    k+=1;
  
    cfft_buffer[ SdrStr.SdrSetup.FFT_Size-k].re = tone_car_magn/2;
    cfft_buffer[ SdrStr.SdrSetup.FFT_Size-k].im = -tone_car_magn/2;

   k+=2; 
    cfft_buffer[ SdrStr.SdrSetup.FFT_Size-k].re = tone_car_magn/2;
    cfft_buffer[ SdrStr.SdrSetup.FFT_Size-k].im = -tone_car_magn/2;

  #ifdef SpectrWS
    arm_cfft_f32(CFFT_init_struct,(float*)&cfft_buffer,1,1);
    Calc_Spec_Mag( cfft_buffer );
  #else    
    Calc_Spec_Mag( cfft_buffer );
    arm_cfft_f32(CFFT_init_struct,(float*)&cfft_buffer,1,1);
  #endif

  int start =  SdrStr.SdrSetup.FFT_Size/2;
  for( int i=0; i< start; ++i ){
      cfft_buffer[i+start].re = cfft_buffer[i+start].re*magn + cfft_buffer[i+start].im*ph;
      (pOUT+i)->re =  (signed long)(cfft_buffer[i+start].re)*mul ;//LSB/USB
      (pOUT+i)->im =  (signed long)(cfft_buffer[i+start].im) ;
  }
  Prepare_buff_i2s_format( (COMPLEX_long*)pOUT,  SdrStr.SdrSetup.FFT_Size/2 , 1);
}
*/

//-----------------------------------------------------------------------------
void SDR_TX( void ){
 

  //tone_car_magn = SdrStr.SdrMode.fTmp;
  signed long mul = 1;
  float tx_mix = (float)SdrStr.SdrSetup.tx_mix/100.0;
  float magn = SdrStr.SdrSetup.tx_mag[ SdrStr.SdrMode.Band ];
  float ph = SdrStr.SdrSetup.tx_ph[ SdrStr.SdrMode.Band ];
  
  
  //**************************************************************************
  Get_New_Pointer();
  Cpy_New_data(0);
  //**************************************************************************
  
  //**************************************************************************
  int start = SdrStr.SdrSetup.FFT_Size/2;
  for( int i=0; i< start; ++i )
      Buf_float_tmp[i] = cfft_buffer[i+start].re;//mic input 
  
  /*if(SdrStr.TX_Tone==2){
    for( int i=0; i< start; ++i )
      Buf_float_tmp[i] = cfft_buffer[i+start].im;//line input 
  }else{
    for( int i=0; i< start; ++i )
      Buf_float_tmp[i] = cfft_buffer[i+start].re;//mic input 
  }*/
  //**************************************************************************

  //**************************************************************************
  //Init_Equal(Exchange.RX_SEQ);
  if(SdrStr.SdrMode.TX_EQ){
    Equal(Buf_float_tmp, Buf_float_tmp  );
  }
  //**************************************************************************

  
  //**************************************************************************
  dspAgc(Buf_float_tmp, SdrStr.SdrMode.AGC_TX);
  //**************************************************************************

  //**************************************************************************
  if ( SdrStr.SdrMode.TX_ECHO )
    echo_proc();
  //**************************************************************************

  //**************************************************************************
  //dspAgc(Buf_float_tmp, SdrStr.SdrMode.AGC_TX);
  //**************************************************************************
     
  
  //**************************************************************************
  for( int i=0; i< start; ++i ){
//    cfft_buffer[i+start].re = Buf_float_tmp[i]*1.5;
    cfft_buffer[i+start].re = Buf_float_tmp[i];
    cfft_buffer[i+start].im = cfft_buffer[i+start].re;
  }
  Cpy_Old_data();
  //**************************************************************************

  
  //**************************************************************************
  if( SdrStr.TX_Tone==1){
    memset((float*)&cfft_buffer,0,sizeof( cfft_buffer ));
    int f = 700;//(SdrStr.SdrSetup.FILTER_SSB[SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]][0]+SdrStr.SdrSetup.FILTER_SSB[SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]][1])/2;
    int k = f /(  SdrStr.SdrSetup.Sample_Rate/ SdrStr.SdrSetup.FFT_Size);
    //if( (chet++)&1 )
    if( k&0x01 )
      k+=1;
    for( int i=0; i<SdrStr.SdrSetup.FFT_Size; ++i ){
      cfft_buffer[i].re = (sinf( TWOPI*(float)i*(float)k/2048.0)*8000000.0);//test tone 24-564hz 48k/2k
      cfft_buffer[i].im = cfft_buffer[i].re;
    }
  }
  if( SdrStr.TX_Tone==2){
    memset((float*)&cfft_buffer,0,sizeof( cfft_buffer ));
    int f1 = 700;
    int k1 = f1 /(  SdrStr.SdrSetup.Sample_Rate/ SdrStr.SdrSetup.FFT_Size);
    if( k1&0x01 )
      k1+=1;
    int f2 = 500;
    int k2 = f2 /(  SdrStr.SdrSetup.Sample_Rate/ SdrStr.SdrSetup.FFT_Size);
    if( k2&0x01 )
      k2+=1;
    
    
    for( int i=0; i<SdrStr.SdrSetup.FFT_Size; ++i ){
      cfft_buffer[i].re = (sinf( TWOPI*(float)i*(float)k1/2048.0)*4000000.0)+(sinf( TWOPI*(float)i*(float)k2/2048.0)*4000000.0);//test tone 564 hz 48k/2k (18+421.875Hz)34-796/8-187.5
      cfft_buffer[i].im = cfft_buffer[i].re;
    }
  }  
  //**************************************************************************
  
  
  //**************************************************************************
   if (SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] < AM ){//SSB
     if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==USB  )
       mul = 1;
     else
       mul = -1;

    arm_cfft_f32(CFFT_init_struct,(float*)&cfft_buffer,0,1);
    Calculate_FIR_coef( SdrStr.SdrSetup.FILTER_SSB[SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]][0], SdrStr.SdrSetup.FILTER_SSB[SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]][1] );
    arm_cmplx_mult_cmplx_f32((float*)&cfft_buffer,(float*)&FIR_coef,(float*)&cfft_buffer, SdrStr.SdrSetup.FFT_Size);

    //Calc_Spec_Mag( cfft_buffer );
    arm_cfft_f32(CFFT_init_struct,(float*)&cfft_buffer,1,1);
    Calc_Spec_Mag( cfft_buffer );

    int start =  SdrStr.SdrSetup.FFT_Size/2;
    for( int i=0; i< start; ++i ){
      
      if( fabsf(cfft_buffer[i+start].re) >8000000.0  ){
        if(cfft_buffer[i+start].re > 0)
          cfft_buffer[i+start].re = 8000000.0;
        else
          cfft_buffer[i+start].re = -8000000.0;
      }
      if( fabsf(cfft_buffer[i+start].im) >8000000.0  ){
        if(cfft_buffer[i+start].im > 0)
          cfft_buffer[i+start].im = 8000000.0;
        else
          cfft_buffer[i+start].im = -8000000.0;
      }

      cfft_buffer[i+start].re = cfft_buffer[i+start].re*magn + cfft_buffer[i+start].im*ph;

      (pOUT+i)->re =  (signed long)(cfft_buffer[i+start].re*tx_mix)*mul ;//LSB/USB
      (pOUT+i)->im =  (signed long)(cfft_buffer[i+start].im*tx_mix) ;
    }
    Prepare_buff_i2s_format( (COMPLEX_long*)pOUT, SdrStr.SdrSetup.FFT_Size/2 , 1);
  }
  //**************************************************************************
  
  
  
  //**************************************************************************
  #define AM_CAR 2   
  #define AM_SHIFT 2     
 
  if (SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==AM){//FM
    
    float am_modfac = (float)SdrStr.SdrSetup.tx_am_mod / 100.0 /2.0;
    float am_sample;
    int start =  SdrStr.SdrSetup.FFT_Size/2;
 
    arm_cfft_f32(CFFT_init_struct,(float*)&cfft_buffer,0,1);
    Calculate_FIRam_coef( 50, SdrStr.SdrSetup.FILTER_AMFM[SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ]] );
    arm_cmplx_mult_cmplx_f32((float*)&cfft_buffer,(float*)&FIR_coef,(float*)&cfft_buffer, SdrStr.SdrSetup.FFT_Size);
    arm_cfft_f32(CFFT_init_struct,(float*)&cfft_buffer,1,1);
    Calc_Spec_Mag( cfft_buffer );

    

    for(int i = 0; i < start; i++){
      am_sample = cfft_buffer[i+start].im/8388607.0;
      if( am_sample > 1.0 )
        am_sample = 1.0;
      if( am_sample < -1.0 )
        am_sample = -1.0;
      
      fm_mod_accum += AM_CAR;	// change frequency using scaled audio
      fm_mod_accum &= 0xffff;				                // limit to 64k range
      fm_mod_idx   = fm_mod_accum;
      fm_mod_idx   &= (FM_DDS_TBL_SIZE - 1);		                // limit lookup to range of sine table

      cfft_buffer[i+start].im = (float32_t)(FM_DDS_TABLE[fm_mod_idx])*(128 + 128*am_sample *am_modfac)-1;    // Load I value
      fm_mod_idx += (FM_DDS_TBL_SIZE/4);	                                // do 90 degree shift by indexing 1/4 into sine table
      fm_mod_idx &= (FM_DDS_TBL_SIZE - 1);		                        // limit lookup to range of sine table
      cfft_buffer[i+start].re = (float32_t)(FM_DDS_TABLE[fm_mod_idx])*(128 + 128*am_sample *am_modfac)-1;	// Load Q value
    } 
    
    for( int i=0; i< start; ++i ){
      cfft_buffer[i+start].re = cfft_buffer[i+start].re*magn + cfft_buffer[i+start].im*ph;
      (pOUT+i)->re =  (signed long)(cfft_buffer[i+start].re*tx_mix) ;//LSB/USB
      (pOUT+i)->im =  (signed long)(cfft_buffer[i+start].im*tx_mix) ;
    }
    Prepare_buff_i2s_format( (COMPLEX_long*)pOUT, SdrStr.SdrSetup.FFT_Size/2 , 1);    
  }
  //**************************************************************************

  
  
  

  //**************************************************************************
// 256 points
//#define DDS_ACC_SHIFT		8
// 512 points
//#define DDS_ACC_SHIFT		7
// 1024 points
#define DDS_ACC_SHIFT		6
  
  if (SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==FM){//FM
    int start =  SdrStr.SdrSetup.FFT_Size/2;
    float fm_sample;

    arm_cfft_f32(CFFT_init_struct,(float*)&cfft_buffer,0,1);
    Calculate_FIRam_coef( 200, SdrStr.SdrSetup.FILTER_AMFM[SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ]] );
    arm_cmplx_mult_cmplx_f32((float*)&cfft_buffer,(float*)&FIR_coef,(float*)&cfft_buffer, SdrStr.SdrSetup.FFT_Size);
    arm_cfft_f32(CFFT_init_struct,(float*)&cfft_buffer,1,1);
    Calc_Spec_Mag( cfft_buffer );


    for(int i = 0; i < start; i++){
      //cfft_buffer[i+start].im = (sinf( TWOPI*(float)i*24.0/2048.0)*8388607.0);//test tone 564 hz 48k/2k
      
      fm_sample = cfft_buffer[i+start].im/8388607.0;
      if( fm_sample > 1.0 )
        fm_sample = 1.0;
      if( fm_sample < -1.0 )
        fm_sample = -1.0;
      
      fm_mod_accum += (unsigned short)(3.0 + (fm_sample *(float)SdrStr.SdrSetup.tx_fm_dev) );	// 141Hz+change frequency using scaled audio
      //fm_mod_accum +=106;// freq = SmRate/2048*2 * fm_mod_accum ( exp 48180/2048*2 * 100 =  4705 Hz )(53=2500 Hz)
      fm_mod_accum &= 0xffff;				                // limit to 64k range
      fm_mod_idx   = fm_mod_accum;
      fm_mod_idx   &= (FM_DDS_TBL_SIZE - 1);		                // limit lookup to range of sine table
      

      
     /* fm_mod_accum += (unsigned short)(200.0 + fm_sample * 6800.0);	// change frequency using scaled audio
      //fm_mod_accum +=8192;//  tone IQ 1000-735HZ/ 8192-6022hz ( 48k/2048fft ) at DDS_ACC_SHIFT 6 //div 64
      fm_mod_accum &= 0xffff;				                // limit to 64k range
      fm_mod_idx    = fm_mod_accum >> DDS_ACC_SHIFT;
      fm_mod_idx &= (FM_DDS_TBL_SIZE - 1);		                // limit lookup to range of sine table*/
     

      cfft_buffer[i+start].im = (float32_t)(FM_DDS_TABLE[fm_mod_idx])*255.0;    // Load I value
      fm_mod_idx += (FM_DDS_TBL_SIZE/4);	                                // do 90 degree shift by indexing 1/4 into sine table
      fm_mod_idx &= (FM_DDS_TBL_SIZE - 1);		                        // limit lookup to range of sine table
      cfft_buffer[i+start].re = (float32_t)(FM_DDS_TABLE[fm_mod_idx])*255.0;	// Load Q value
    } 
    
    for( int i=0; i< start; ++i ){
      cfft_buffer[i+start].re = cfft_buffer[i+start].re*magn + cfft_buffer[i+start].im*ph;
      (pOUT+i)->re =  (signed long)(cfft_buffer[i+start].re*tx_mix) ;//LSB/USB
      (pOUT+i)->im =  (signed long)(cfft_buffer[i+start].im*tx_mix) ;
    }
    Prepare_buff_i2s_format( (COMPLEX_long*)pOUT, SdrStr.SdrSetup.FFT_Size/2 , 1);    
  }
  //**************************************************************************
  
}
//-----------------------------------------------------------------------------

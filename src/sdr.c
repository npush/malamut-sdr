#include "main.h"

#include "lmadf.h"


//#include "Windish.h"

/* Create a section for variables. */
#pragma section = "CCM_RAM"



arm_cfft_radix4_instance_f32  cfft_instance;
arm_cfft_instance_f32*        CFFT_init_struct;

__IO unsigned char rx_start, tx_start;

__IO COMPLEX_long *pADC;
__IO COMPLEX_long *pDAC;
__IO COMPLEX_long *pIN;
__IO COMPLEX_long *pOUT;

COMPLEX_long ADC_buff[FFT_FILTER_SIZE];
COMPLEX_long DAC_buff[FFT_FILTER_SIZE];

COMPLEX_float cfft_buffer[FFT_FILTER_SIZE] CCM_RAM;       // внутренни буффер для БПФ
COMPLEX_float cfft_buffer_old[FFT_FILTER_SIZE/2] CCM_RAM; // внутренни буффер для БПФ старые данные
COMPLEX_float buffer_old[40] CCM_RAM;                     // для смещения спектра SHIFT (48k/2048 = 23.4375Hz*40 = 937.5Hz)
COMPLEX_float FIR_coef[FFT_FILTER_SIZE] CCM_RAM;          // Filter

float Buf_float_tmp[FFT_FILTER_SIZE/2];                       // Аудио данные для AGC
float Buf_float_tmp2[Buf_float_tmp2_size];                  // RX - spectr_waterfall / TX - echo

#ifdef SpectrWS
  COMPLEX_float Tmp_Pwr[2048];
  float WindowCoefs_for_Spectrum[2048];
#endif

//void (*CALL_SDR)();

static unsigned char RTX_Mode;

//-----------------------------------------------------------------------------
void SDR_init( void ){
  
  CS_TX_DeInit();
  CS_RX_DeInit();

    
  GPIO_InitTypeDef      GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(LedTx_CLK|LedRx_CLK, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin = LedTx_PIN|LedRx_PIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
 
  GPIO_Init(LedTx_PORT, &GPIO_InitStructure);
  GPIO_WriteBit(LedTx_PORT, LedTx_PIN, Bit_SET  );

  GPIO_Init(LedRx_PORT, &GPIO_InitStructure);
  GPIO_WriteBit(LedRx_PORT, LedRx_PIN, Bit_SET  );
    
  
  

  
  pADC=&ADC_buff[ 0 ];
  pDAC=&DAC_buff[ 0 ];

  

  
  if( SdrStr.SdrSetup.FFT_Size == 2048 ){
    CFFT_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len2048);
  }
  if( SdrStr.SdrSetup.FFT_Size == 1024 ){
    CFFT_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len1024);
  }
  if( SdrStr.SdrSetup.FFT_Size == 512 ){
    CFFT_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len512);
  }
  if( SdrStr.SdrSetup.FFT_Size == 256 ){
    CFFT_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len256);
  }
  if( SdrStr.SdrSetup.FFT_Size == 128 ){
    CFFT_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len128);
  }
  
  
   /* 
  //for test
  for( int i=0; i<2048; ++i ){
    cfft_buffer[i].re = (sinf( TWOPI*(float)i*25.0/2048.0)*8388607.0)*0.5 + (sinf( TWOPI*(float)i*25.0/2048.0)*8388607.0)*0.5;
    cfft_buffer[i].im = (cosf( TWOPI*(float)i*23.0/2048.0)*8388607.0)*0.5 + (cosf( TWOPI*(float)i*23.0/2048.0)*8388607.0)*0.5;
  }
  arm_cfft_f32(CFFT_init_struct,(float*)&cfft_buffer,0,1);*/
  
  
 //mismatch_init ();//Инициализирую параметры для компенсации дисбаланса

  
  
  dspAgc_Init();

  
  Init_Equal(SdrStr.SdrMode.RX_SEQ);
  Calculate_Window_coef( 2 );
  //Calculate_FIR_coef( Exchange.Filt_Lo, Exchange.Filt_Hi );

  Calculate_FIR_coef( 1, 5 );
  //Calculate_FIR_coef( Exchange.Filt_Lo, Exchange.Filt_Hi );
  
  
  //void init_pll (float samprate, float freq, float lofreq, float hifreq, float bandwidth)
  //init_pll ((float)SdrStr.SdrSetup.Sample_Rate, 0.0, -24000.0f, 24000.0f, 9000.0f);
  //init_pll ((float)SdrStr.SdrSetup.Sample_Rate, 0.0, -24000.0f, 24000.0f, 2700.0f);
  init_pll ((float)SdrStr.SdrSetup.Sample_Rate, 0.0, -24000.0f, 24000.0f, (float)(SdrStr.SdrSetup.FILTER_AMFM[SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ]]) );
  
  CS4272_I2S_init();
  
  InitTimeInt1ms();
  
  sdr_rtx_mode( SdrStr.SdrMode.RTX_Mode );//DMA init
  RTX_Mode = SdrStr.SdrMode.RTX_Mode;

  
  
  /*new_lmsr_init((COMPLEX*)&cfft_buffer[Exchange.FFT_Size/2], Exchange.FFT_Size/2,// CXB signal,
			    64,	        // int delay,
			    0.01f,	// REAL adaptation_rate,
			    0.00001f,	// REAL leakage,
			    45,	        // int adaptive_filter_size,
			    LMADF_INTERFERENCE);
  */
  

}
//-----------------------------------------------------------------------------

void sdr_rtx_mode( int st_tx ){
  

  if( st_tx ){
    MIX_RX_OFF();
    dspAgc_setGain( SdrStr.SdrSetup.MIC_GAIN );
      
    CS_RX_DeInit();
    
   
    memset( (unsigned char*)&ADC_buff, 0, sizeof( ADC_buff ) );
    memset( (unsigned char*)&DAC_buff, 0, sizeof( DAC_buff ) );
    memset( (unsigned char*)&cfft_buffer, 0, sizeof( cfft_buffer ) );
    memset( (unsigned char*)&cfft_buffer_old, 0, sizeof( cfft_buffer_old ) );
    memset( (unsigned char*)&Buf_float_tmp, 0, sizeof( Buf_float_tmp ) );
    memset( (unsigned char*)&Buf_float_tmp2, 0, sizeof( Buf_float_tmp2 ) );
    #ifdef SpectrWS
    memset( (unsigned char*)&Tmp_Pwr, 0, sizeof( Tmp_Pwr ) );
    #endif


    memset( (unsigned char*)&SdrStr.Spectr, 0, 256 );
    memset( (unsigned char*)&SdrStr.Spectr_avg, 0, 256 );

    
    need_next = 0;
    echo_init();
    if(SdrStr.SdrMode.TX_EQ)
      Init_Equal(SdrStr.SdrMode.TX_SEQ);
    
    //4*Exchange.FFT_Size - 4 = 2(chanel)*2(long/short)
    CS_TX_Init( (unsigned char*)pADC,(unsigned char*)pDAC,  4*SdrStr.SdrSetup.FFT_Size );
    CS_RX_Init( NULL,(unsigned char*)pDAC,  4*SdrStr.SdrSetup.FFT_Size );// no mute
    
    TX_ON();
    MIX_TX_ON();

  }else{

    TX_OFF();
    MIX_TX_OFF();
    dspAgc_setGain( SdrStr.SdrSetup.RX_GAIN );

    CS_TX_DeInit();
    CS_RX_DeInit();
    
    memset( (unsigned char*)&ADC_buff, 0, sizeof( ADC_buff ) );
    memset( (unsigned char*)&DAC_buff, 0, sizeof( DAC_buff ) );
    memset( (unsigned char*)&cfft_buffer, 0, sizeof( cfft_buffer ) );
    memset( (unsigned char*)&cfft_buffer_old, 0, sizeof( cfft_buffer_old ) );
    memset( (unsigned char*)&Buf_float_tmp, 0, sizeof( Buf_float_tmp ) );
    memset( (unsigned char*)&Buf_float_tmp2, 0, sizeof( Buf_float_tmp2 ) );
    
    memset( (unsigned char*)&SdrStr.Spectr, 0, 256 );
    memset( (unsigned char*)&SdrStr.Spectr_avg, 0, 256 );  
   
    if(SdrStr.SdrMode.RX_EQ)
      Init_Equal(SdrStr.SdrMode.RX_SEQ);
    
    need_next = 0;
    CS_RX_Init( (unsigned char*)pADC,(unsigned char*)pDAC,  4*SdrStr.SdrSetup.FFT_Size );

    MIX_RX_ON();
  }
}
//-----------------------------------------------------------------------------
void InitTimeInt1ms( void )
{
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

  /* Enable the TIM2 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Timer1ms_SDR_Pre;// SDR
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = Timer1ms_SDR_Sub;// SDR
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
   
  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 1 MHz down to 1 KHz (1 ms)
  TIM_TimeBaseStructure.TIM_Prescaler = 108 - 1; // 24 MHz Clock down to 1 MHz (adjust per your clock)84-168 108-216
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  /* TIM IT enable */
  
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}
//-----------------------------------------------------------------------------
void TIM2_IRQHandler(void){
  
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    
    if( RTX_Mode != SdrStr.SdrMode.RTX_Mode ){
      sdr_rtx_mode( SdrStr.SdrMode.RTX_Mode );//DMA init
      RTX_Mode = SdrStr.SdrMode.RTX_Mode;
    }
    
    if( need_next ){
      if( SdrStr.SdrMode.RTX_Mode ){
        GPIO_WriteBit(LedTx_PORT,LedTx_PIN,Bit_RESET);
        SDR_TX();
        GPIO_WriteBit(LedTx_PORT,LedTx_PIN,Bit_SET);
      }
      else{
        GPIO_WriteBit(LedRx_PORT,LedRx_PIN,Bit_RESET);
        SDR_RX();
        GPIO_WriteBit(LedRx_PORT,LedRx_PIN,Bit_SET);
      }
      //CALL_SDR();
      need_next = 0;
    }  
    
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}
//-----------------------------------------------------------------------------

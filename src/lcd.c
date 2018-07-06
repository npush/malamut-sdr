
#include "main.h"



//***********************************
unsigned short Waterfall_gamma[256];
//***********************************

#define lcd_txt_left 262

//-----------------------------------------------------------------------------
void Lcd_init( void ){


  TM_ILI9341_Init();
  TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_1);
  
  //TM_ILI9341_Fill(BLACK);
  // TM_ILI9341_Fill(ILI9341_COLOR_GREEN);
  /* 
  TM_ILI9341_DrawCircle(60, 60, 40, ILI9341_COLOR_GREEN);
  TM_ILI9341_DrawFilledCircle(60, 60, 35, ILI9341_COLOR_RED);
  TM_ILI9341_DrawRectangle(120, 20, 220, 100, ILI9341_COLOR_BLUE);
  TM_ILI9341_DrawFilledRectangle(130, 30, 210, 90, ILI9341_COLOR_BLACK);
  TM_ILI9341_DrawLine(10, 120, 310, 120, 0x0005);
  TM_ILI9341_Puts(65, 130, "STM32F4 Discovery", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
  TM_ILI9341_Puts(60, 150, "ILI9341 LCD Module", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
  TM_ILI9341_Puts(245, 225, "majerle.eu", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);*/
  
  // TM_ILI9341_DrawLine(10, 100, 310, 100, RED);
}
//-----------------------------------------------------------------------------

void Lcd_Spectr_init( int type ){
  
  int a = 0;
  memset((unsigned char*)&Waterfall_gamma, RGB16(255,0,255), sizeof(Waterfall_gamma));
  
  if( type ){
    for( int i=0;i<42; ++i ){
      Waterfall_gamma[a+i] = RGB16(0,0,(int)(powf(0.095*(float)i,4)));
    }a+=42;
    for( int i=0;i<42; ++i ){
      Waterfall_gamma[a+i] = RGB16(0,i*6,255);
    }a+=42;
    for( int i=0;i<42; ++i ){
      Waterfall_gamma[a+i] = RGB16(0,255, (int)( (0.39*(float)(41-i))*(0.39*(float)(41-i)) ) );
    }a+=42;
    for( int i=0;i<42; ++i ){
      Waterfall_gamma[a+i] = RGB16(i*6,255,0);
    }a+=42;
    for( int i=0;i<42; ++i ){
      Waterfall_gamma[a+i] = RGB16(255,(41-i)*6,0);
    }a+=42;
    for( int i=0;i<42; ++i ){
      Waterfall_gamma[a+i] = RGB16(255,0,i*6);
    }a+=42;
  }else{
  

    for( int i=0;i<64; ++i )
        Waterfall_gamma[a+i] = RGB16(0,0,(int)(powf(0.0625*(float)i,4)));

    a+=64;

    for( int i=0;i<32; ++i )
      Waterfall_gamma[a+i] = RGB16(0,i*8,255);
    a+=32;
    for( int i=0;i<32; ++i )
      Waterfall_gamma[a+i] = RGB16(0,255,255-i*8);
    a+=32;
    for( int i=0;i<32; ++i )
      Waterfall_gamma[a+i] = RGB16(i*8,255,0);
    a+=32;

    for( int i=0;i<64; ++i )
      Waterfall_gamma[a+i] = RGB16(255,255-i*4,0);
    a+=64;
    
    for( int i=0;i<32; ++i )
      Waterfall_gamma[a+i] = RGB16(255,0,i*8);
    a+=32;

  }

 /* for( int i=0;i<240; ++i ){
    TM_ILI9341_DrawLine( 310, i ,319,i,Waterfall_gamma[i+15]);
  }*/
  /*for( int i=0;i<256; ++i )
  Waterfall_gamma[i] = 0x01FF;*/
  

}
//------------------------------------------------------------------------------
void Lcd_Freq( int freq ){
  /*
  int col = YELLOW;
  if( SdrStr.SdrMode.Band <10 )
    col = GREEN;
  if( (freq< BandTable[SdrStr.SdrMode.Band][0])||(freq >BandTable[SdrStr.SdrMode.Band][1]) )
    col = BLUE;
  */
  int col = BLUE;
  
  for( int i=0; i<10;++i ){//HM
    if( (BandTable[i][0] <= freq)&&(freq <= BandTable[i][1]) ){
      col = GREEN;
      break;
    }
  }
  for( int i=10; i<(TotalBend-1);++i ){//AM
    if( (BandTable[i][0] <= freq)&&(freq <= BandTable[i][1]) ){
      col = YELLOW;
      break;
    }
  }
  if( SdrStr.SdrMode.Band == (TotalBend-1) ){
    col = MAUVE;
  }
  /*if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] >= AM )
    freq-=SdrStr.SdrSetup.AMFM_Centr;*/

  Freq_Lcd( 25,170,freq,col,BLACK );
    


  /*Freq_Lcd( 30,150,freq,GREEN,BLACK );*/
}
//------------------------------------------------------------------------------

  #define SPIx_DMA                       DMA2
  #define SPIx_DMA_CLK                   RCC_AHB1Periph_DMA2
  #define SPIx_TX_DMA_CHANNEL            DMA_Channel_3
  #define SPIx_TX_DMA_STREAM             DMA2_Stream3
  #define SPIx_TX_DMA_FLAG_TCIF          DMA_FLAG_TCIF3

void LcdSpiDmaSend( void ){
  
  SPI_Cmd(SPIx, DISABLE);
  SPI_I2S_DeInit(SPIx);
  SPI_InitTypeDef  SPI_InitStructure;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPIx, &SPI_InitStructure);
  SPI_Cmd(SPIx, ENABLE);
  
  
  
  DMA_InitTypeDef DMA_InitStructure; 
  RCC_AHB1PeriphClockCmd(SPIx_DMA_CLK, ENABLE);
  
 /* unsigned short hg[2];
  hg[0] = 0x0103;
  hg[1] = 0x0F70;*/
  
  /* Deinitialize DMA Streams */
  DMA_DeInit(SPIx_TX_DMA_STREAM);
  
  /* Configure DMA Initialization Structure */
  DMA_InitStructure.DMA_BufferSize = 30720;//15360;//30720 ;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
  
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  //DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(SPI1->DR)) ;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  //DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_HalfWord;
  
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
 
  /* Configure TX DMA */
  DMA_InitStructure.DMA_Channel = SPIx_TX_DMA_CHANNEL ;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
  DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)&Buf_float_tmp2[0] ;
  DMA_Init(SPIx_TX_DMA_STREAM, &DMA_InitStructure);

  /* Enable DMA SPI TX Stream */
  DMA_Cmd(SPIx_TX_DMA_STREAM,ENABLE);
  /* Enable SPI DMA TX Requsts */
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
  

  
    /* Waiting the end of Data transfer */
  while (DMA_GetFlagStatus(SPIx_TX_DMA_STREAM,SPIx_TX_DMA_FLAG_TCIF)==RESET);
  
  /* Clear DMA Transfer Complete Flags */
  DMA_ClearFlag(SPIx_TX_DMA_STREAM,SPIx_TX_DMA_FLAG_TCIF);
  
  /* Disable DMA SPI TX Stream */
  DMA_Cmd(SPIx_TX_DMA_STREAM,DISABLE);
  
  /* Disable SPI DMA TX Requsts */
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);

  
  
    SPI_Cmd(SPIx, DISABLE);
  SPI_I2S_DeInit(SPIx);

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPIx, &SPI_InitStructure);
  SPI_Cmd(SPIx, ENABLE);
}

void DMA2_Stream3_IRQHandler( void ){

  if (DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3) == SET)
  {


  }


}
//------------------------------------------------------------------------------

volatile int freq_spec_tmp;
unsigned short* Spec_buf_h = (unsigned short*)&Buf_float_tmp2[0];
//unsigned short* Spec_buf_t = (unsigned short*)&Buf_float_tmp2[0];
//unsigned short* Spec_buf_m = (unsigned short*)&Buf_float_tmp2[0];

unsigned long TuneGreedFreq=0;
float kGreedLast;

unsigned long TuneWfFreq=0;
signed int kWfLast;


unsigned char * SMagline;
unsigned char SMag_avg_old[256]={0};
unsigned char SMag_back_old[256]={0};
float Slev = 0;
float Slev_old = 0;

//60=30720 - 256*60*2
//static unsigned char delspec=0;
void Lcd_Spectr(int wf_x, int wf_y, int wf_l, int tx_rx){
   /* if( !nMag )
    return;*/
  
  
  //delspec++;
    
  unsigned short  Spec_buf_max = 256*wf_l;
   
  float beta=0.7;
  float alpha=1-beta;
  float k;
  
  float kGreed;
  signed int xGreed;
  #define pxHz  188.2
  #define px5k  26.567
  
  signed int a;
  unsigned short rgb;
  unsigned short p;


  TuneGreedFreq = SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ];    

    
 
  //------------------------  DRAW WATER OLD LINE-------------------------

  if( tx_rx ){
    TM_ILI9341_SetCursorPosition(wf_x, wf_y, wf_x+255, wf_y+wf_l);
    ILI9341_WRX_RESET;
    ILI9341_CS_RESET;
    TM_SPI_Send(ILI9341_SPI, ILI9341_GRAM);
    ILI9341_WRX_SET;


    //GPIO_WriteBit(LedCpu_PORT, LedCpu_PIN, Bit_SET  );

    LcdSpiDmaSend();
    
    for( int j=256; j< Spec_buf_max ;++j ){//0.4ms@216MHz
      Spec_buf_h[j-256] = Spec_buf_h[j];
    }
    
    
    kWfLast = (signed int)((signed int)(TuneGreedFreq-TuneWfFreq)/pxHz);
    if( kWfLast != 0 ){
      //TuneWfFreq = TuneGreedFreq;
      TuneWfFreq+=(signed int)(kWfLast*pxHz);
    }
    if( (kWfLast > 255)||(kWfLast < -255 ) )
      kWfLast=0;
    
    if( kWfLast > 0 ){//left
      for( int i=0; i<wf_l; ++i ){//строки
        for( int j=0; j<256; ++j ){//pix
          a=j+kWfLast;
          if( a>255 )
            Spec_buf_h[i*256+j]  = 0;
          else
            Spec_buf_h[i*256+j] = Spec_buf_h[i*256+a];
        }
      }
    }
    if( kWfLast < 0 ){//right
      for( int i=0; i<wf_l; ++i ){//строки
        for( int j=255; j>=0; --j ){//pix
          a=j+kWfLast;
          if( a<0 )
            Spec_buf_h[i*256+j]  = 0;
          else
            Spec_buf_h[i*256+j] = Spec_buf_h[i*256+a];
        }
      }
    }
  
      
    //GPIO_WriteBit(LedCpu_PORT, LedCpu_PIN, Bit_RESET  );
    
    ILI9341_CS_SET;
   }
  //---------------------------------------------------------------------
  
  //-------------------------- Calc Slev --------------------------------
  a = 255;
  for( int i=0; i<256; ++i ){
   a = MIN( a, SdrStr.Spectr_avg[i] );
  }
  Slev= Slev*beta+(float)a*alpha;
  k=255.0/(255.0-Slev);
  //---------------------------------------------------------------------

  //------------------------ DRAW NEW LINE ------------------------------
  if( tx_rx ){

    for( int i=0; i<256; ++i ){
      a =SdrStr.Spectr[i]-(int)Slev;
      //a =SdrStr.Spectr_avg[i]-(int)Slev;
      a*=k*1.7;// collor contract
      if( a<0 )a = 0;
      if( a>255 )a=255;
      
      rgb = Waterfall_gamma[a];
      *(((unsigned char*)&Spec_buf_h[Spec_buf_max-256+i])+0) = *(((unsigned char*)&rgb)+1);
      *(((unsigned char*)&Spec_buf_h[Spec_buf_max-256+i])+1) = *(((unsigned char*)&rgb)+0);
    }
 
  }
  //---------------------------------------------------------------------
  
  
  
  
  
  
  //------------------------  GREED CLEAR-------------------------
  kGreed = TuneGreedFreq%5000;//5kHz
  kGreed = (int)(kGreed/pxHz);
  if( kGreedLast != kGreed ){
    for( int i=0; i<16; ++i )
      TM_ILI9341_DrawPixel(wf_x+127-(int)kGreedLast, wf_y+wf_l+1+i*4,BLACK); //centr    
    for( int j=1; j<5; ++j ){//vertical lines left
      for( int i=0; i<16; ++i ){
        xGreed = wf_x+127-(int)((float)j*px5k+kGreedLast);
        if( xGreed >= 0 )
          TM_ILI9341_DrawPixel( xGreed, wf_y+wf_l+1+i*4,BLACK);         
      }
    }  
    for( int j=1; j<6; ++j ){//vertical lines right
      for( int i=0; i<16; ++i ){
        xGreed =wf_x+127+(int)((float)j*px5k-kGreedLast);
        if( xGreed <= (wf_x+256) )
          TM_ILI9341_DrawPixel( xGreed, wf_y+wf_l+1+i*4,BLACK);
      }
    }  
    for( int j=0; j<5; ++j ){//horizontal line 5*16=80  
      for( int i=0; i<64; ++i ){
        xGreed = wf_x+i*4+3-(int)kGreedLast%4;
        TM_ILI9341_DrawPixel(xGreed, wf_y+wf_l+j*16,BLACK);         
      }
    } 
  }

  //------------------------  SPEKTR -------------------------
  SMagline = &SdrStr.Spectr_avg[0];
  for( int i=0; i<256; ++i ){
    if( SdrStr.SdrSetup.FFT_Spect_ofset == 0 ){//auto ofset
      a =SMagline[i]-(int)Slev;
      a*=k*1.7;// collor contract
      if( a<0 )a = 0;
      if( a>255 )a=255;
      a/=4;
    }else{
      a = SMagline[i]/4;
    }

   if( kGreedLast != kGreed ) 
    LineUP_Spectr2( wf_x+i,wf_y+wf_l+1, wf_y+wf_l+1+ a, &Waterfall_gamma[0]  );
   
   if( a > SMag_avg_old[ i ] ){
     if( kGreedLast == kGreed ) 
      LineUP_Spectr2( wf_x+i,wf_y+wf_l+1 , wf_y+wf_l+1+ a, &Waterfall_gamma[0]  );
    }
   else{
      LineUP_EndWhite( wf_x+i,wf_y+wf_l+1+a, wf_y+wf_l+1+SMag_avg_old[ i ], BLACK  );
   }
   SMag_avg_old[ i ] = a;
  }
  //------------------------  GREED DRAW-------------------------
  for( int i=0; i<16; ++i )
    TM_ILI9341_DrawPixel(wf_x+127-(int)kGreed, wf_y+wf_l+1+i*4,WHITE); //centr        
  for( int j=1; j<5; ++j ){//vertical lines left
    for( int i=0; i<16; ++i ){
      xGreed = wf_x+127-(int)((float)j*px5k+kGreed);
      if( xGreed >= 0 )
        TM_ILI9341_DrawPixel( xGreed, wf_y+wf_l+1+i*4,WHITE);         
    }
  }  
  for( int j=1; j<6; ++j ){//vertical lines right
    for( int i=0; i<16; ++i ){
      xGreed =wf_x+127+(int)((float)j*px5k-kGreed);
      if( xGreed <= (wf_x+256) )
        TM_ILI9341_DrawPixel( xGreed, wf_y+wf_l+1+i*4,WHITE);
    }
  }
  for( int j=0; j<5; ++j ){//horizontal line 5*16=80  
    for( int i=0; i<64; ++i ){
      xGreed =wf_x+i*4+3-(int)kGreed%4;
      TM_ILI9341_DrawPixel(xGreed, wf_y+wf_l+j*16,WHITE);         
    }
  } 
  kGreedLast = kGreed;
  //LineLF( wf_x, wf_x+256, wf_y+wf_l+64, WHITE);
  
  
  //-----------------------FILTR LINE -------------------------
  unsigned long Fil_a, Fil_b,f_k = 0;
  static unsigned char Fil_ac, Fil_bc;

  if( SdrStr.SdrSetup.FFT_Size == 2048 )
    a = 8;
  if( SdrStr.SdrSetup.FFT_Size == 1024 )
    a = 4;
  if( SdrStr.SdrSetup.FFT_Size == 512 )
    a = 2;
  if( SdrStr.SdrSetup.FFT_Size == 256 )
    a = 1;
  
  f_k =  SdrStr.SdrSetup.Sample_Rate * 100 / SdrStr.SdrSetup.FFT_Size;
  
  if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] < AM ){
    Fil_a = SdrStr.SdrSetup.FILTER_SSB[SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]][0]*100/f_k;
    Fil_b = SdrStr.SdrSetup.FILTER_SSB[SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]][1]*100/f_k;
  }
  if (SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==AM){
    Fil_a = SdrStr.SdrSetup.FILTER_AMFM[SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ]]*100/f_k;
    Fil_b = 0;
  }  
  if (SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]>=FM){
    Fil_a = SdrStr.SdrSetup.FILTER_AMFM[SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ]]*100/f_k;
    Fil_b = 0;
  }  
  
  Fil_a = Fil_a/a*SdrStr.SdrSetup.SpecScale;
  Fil_b = Fil_b/a*SdrStr.SdrSetup.SpecScale;
  
  if( Fil_a > 127 )
    Fil_a = 127;
  if( Fil_b > 127 )
    Fil_b = 127;
  
  if((SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==LSB)||(SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==CW)){
    Fil_a = 127-Fil_a;
    Fil_b = 127-Fil_b;
  }
  if((SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==USB)||(SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==CWR)){
    p = Fil_a;
    Fil_a =127+Fil_b ;
    Fil_b =127+p;
  }
  if((SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==AM)||(SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==FM) ){
    p = Fil_a;
    Fil_a =127-p ;
    Fil_b =127+p;
  }
  if(SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]==FM2){
    Fil_a = 127-Fil_a;
    Fil_b = 127;
  }

  
  
  if( (Fil_ac!=Fil_a)||(Fil_bc!=Fil_b) ){
    LineUP( wf_x+Fil_ac,wf_y+wf_l, wf_y+wf_l+64, BLACK  );
    LineUP( wf_x+Fil_bc,wf_y+wf_l, wf_y+wf_l+64, BLACK  );
    Fil_ac=Fil_a;
    Fil_bc=Fil_b;
  }
    LineUP( wf_x+Fil_a,wf_y+wf_l, wf_y+wf_l+64, WHITE  );
    LineUP( wf_x+Fil_b,wf_y+wf_l, wf_y+wf_l+64, WHITE  );

  //--------------------------------------------------------
  
 // nMag = 0;
}
//------------------------------------------------------------------------------
void LcdMode( void ){
  if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] == LSB)
    sprintf( text, "LSB" );
  if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] == USB)
    sprintf( text, "USB" );
  if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] == CW)
    sprintf( text, "CW " );
  if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] == CWR)
    sprintf( text, "CWR" );
  if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] == AM)
    sprintf( text, "AM " );  
  if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] == FM)
    sprintf( text, "FM " );  
  if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] == FM2)
    sprintf( text, "FM2 " );  

  SdrStr.FmPll_Init = 1;
  
  TM_ILI9341_Puts(220, 0, text, &TM_Font_11x18, GREEN, BLACK);
}
//------------------------------------------------------------------------------
void LcdFilters( void ){


  if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] < AM )
    sprintf( text, "%1.1fkHz ", ( SdrStr.SdrSetup.FILTER_SSB[SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]][1]- SdrStr.SdrSetup.FILTER_SSB[SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]][0])/1000.0);
  else
    //sprintf( text, "%1.1fkHz ", ( SdrStr.SdrSetup.AMFM_Centr - SdrStr.SdrSetup.FILTER_AMFM[SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ]][0])/1000.0);
    sprintf( text, "%1.1fkHz ", ( SdrStr.SdrSetup.FILTER_AMFM[SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ]])/1000.0);
    
  SdrStr.FmPll_Init = 1;
  
  TM_ILI9341_Puts(220, 40, text, &TM_Font_11x18, GREEN, BLACK);
  
}
//------------------------------------------------------------------------------
void LcdFiltersSet( void ){

  if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] < AM ){
    sprintf( text, "L%1.2f  H%1.2f",   SdrStr.SdrSetup.FILTER_SSB[SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]][0]/1000.0,
                                    SdrStr.SdrSetup.FILTER_SSB[SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]][1]/1000.0  );
  }
  else{
    sprintf( text, "L%1.2f  H%1.2f",   0/1000.0,
                                    SdrStr.SdrSetup.FILTER_AMFM[SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ]]/1000.0  );
  }
    
  TM_ILI9341_Puts(0, 187, text, &TM_Font_11x18, YELLOW, BLACK);
}
//------------------------------------------------------------------------------
void Lcd_Clock( unsigned char h, unsigned char m, unsigned char c ){
  sprintf( text, "UTC %02d%c%02d", h, c, m );
  TM_ILI9341_Puts(220, 140, text, &TM_Font_11x18, BLUE, BLACK);
}
void Lcd_Clock_wDate( unsigned char d,unsigned char mth,unsigned char h, unsigned char m, unsigned char c ){
  sprintf( text, "%02d/%02d %02d%c%02d", d, mth, h, c, m );
  TM_ILI9341_Puts(220, /*140*/132, text, &TM_Font_11x18, BLUE, BLACK);
}
//------------------------------------------------------------------------------
void Lcd_Dbg( void ){
  if( SdrStr.Tmp == 0 ){
    Lcd_Pre();
    Lcd_Att();
  }
  if( SdrStr.Tmp == 1 ){
    TM_ILI9341_Puts(200, lcd_txt_left, "RF OF", &TM_Font_11x18, RED, BLACK);
    TM_ILI9341_Puts(180, lcd_txt_left, "DS ON", &TM_Font_11x18, RED, BLACK);
  }
  if( SdrStr.Tmp == 2 ){
    TM_ILI9341_Puts(200, lcd_txt_left, "RF ON", &TM_Font_11x18, RED, BLACK);
    TM_ILI9341_Puts(180, lcd_txt_left, "DS OF", &TM_Font_11x18, RED, BLACK);
  }
  if( SdrStr.Tmp == 3 ){
    TM_ILI9341_Puts(200, lcd_txt_left, "RF OF", &TM_Font_11x18, RED, BLACK);
    TM_ILI9341_Puts(180, lcd_txt_left, "DS OF", &TM_Font_11x18, RED, BLACK);
  }
}
//------------------------------------------------------------------------------
void Lcd_DCVolt( void ){
  static unsigned long time_dcvolt;
  if( Interval_RTC_Get( time_dcvolt ) > 1000 ) {
    sprintf(text,  "%4.1fV", adc_delta * (float)adc_VLT );
    TM_ILI9341_Puts(220, 260, text, &TM_Font_11x18, GREEN, BLACK);
    time_dcvolt = RTC_Get(); 
  }
}
//------------------------------------------------------------------------------
void Lcd_Pre( void ){
  int color = GRAY4;
  if( SdrStr.SdrMode.PRE[ SdrStr.SdrMode.Band ]  )
    color = GREEN;
  TM_ILI9341_Puts(200, lcd_txt_left, "PRE  ", &TM_Font_11x18, color, BLACK);
}
//------------------------------------------------------------------------------
void Lcd_Att( void ){
  if( SdrStr.SdrMode.ATT[ SdrStr.SdrMode.Band ] == 0  )
    TM_ILI9341_Puts(180, lcd_txt_left, "ATT  ", &TM_Font_11x18, GRAY4, BLACK);
  if( SdrStr.SdrMode.ATT[ SdrStr.SdrMode.Band ] == 1  )
    TM_ILI9341_Puts(180, lcd_txt_left, "ATT 8", &TM_Font_11x18, GREEN, BLACK);
  if( SdrStr.SdrMode.ATT[ SdrStr.SdrMode.Band ] == 2  )
    TM_ILI9341_Puts(180, lcd_txt_left, "ATT16", &TM_Font_11x18, GREEN, BLACK);
  if( SdrStr.SdrMode.ATT[ SdrStr.SdrMode.Band ] == 3  )
    TM_ILI9341_Puts(180, lcd_txt_left, "ATT24", &TM_Font_11x18, GREEN, BLACK);
}
//------------------------------------------------------------------------------
void Lcd_Anf( void ){
  int color = GRAY4;
  if( SdrStr.SdrMode.ANF  )
    color = GREEN;
  TM_ILI9341_Puts(160, lcd_txt_left, "ANF", &TM_Font_11x18, color, BLACK);
}
//------------------------------------------------------------------------------
void Lcd_Dnr( void ){
  int color = GRAY4;
  if( SdrStr.SdrMode.DNR  )
    color = GREEN;
  TM_ILI9341_Puts(140, lcd_txt_left, "DNR", &TM_Font_11x18, color, BLACK);
}
//------------------------------------------------------------------------------
void Lcd_Nb( void ){
  if( SdrStr.SdrMode.NB == 0  )
    TM_ILI9341_Puts(120, lcd_txt_left, "NB00", &TM_Font_11x18, GRAY4, BLACK);
  if( SdrStr.SdrMode.NB == 1  )
    TM_ILI9341_Puts(120, lcd_txt_left, "NB01", &TM_Font_11x18, GREEN, BLACK);
  if( SdrStr.SdrMode.NB == 2  )
    TM_ILI9341_Puts(120, lcd_txt_left, "NB02", &TM_Font_11x18, GREEN, BLACK);
  if( SdrStr.SdrMode.NB == 3  )
    TM_ILI9341_Puts(120, lcd_txt_left, "NB12", &TM_Font_11x18, GREEN, BLACK);
}
//------------------------------------------------------------------------------
void Lcd_Agc( void ){
  if( SdrStr.SdrMode.RTX_Mode )
    sprintf(text,"AGC%02d", SdrStr.SdrMode.AGC_TX  );
  else
    sprintf(text,"AGC%02d", SdrStr.SdrMode.AGC_RX[ SdrStr.SdrMode.Band ]  );
  TM_ILI9341_Puts(100, lcd_txt_left, text, &TM_Font_11x18, GREEN, BLACK); 
}
//------------------------------------------------------------------------------
void Lcd_Moni( void){
    int color = GRAY4;
  if( SdrStr.SdrMode.Moni  )
    color = GREEN;
  TM_ILI9341_Puts(80, lcd_txt_left, "MONI", &TM_Font_11x18, color, BLACK);
}
//------------------------------------------------------------------------------
void Lcd_Echo( void ){
  int color = GRAY4;
  if( SdrStr.SdrMode.TX_ECHO  )
    color = GREEN;
  TM_ILI9341_Puts(60, lcd_txt_left, "ECHO", &TM_Font_11x18, color, BLACK);
}
//------------------------------------------------------------------------------
void Lcd_Eq( void ){
  if( (SdrStr.SdrMode.TX_EQ==0)&&(SdrStr.SdrMode.RX_EQ==1)  ){
    TM_ILI9341_Puts(40, lcd_txt_left, "EQR", &TM_Font_11x18, GREEN, BLACK);
    TM_ILI9341_Puts(40, lcd_txt_left+33, "T", &TM_Font_11x18, GRAY4, BLACK);
    return;
  }
  if( (SdrStr.SdrMode.TX_EQ==1)&&(SdrStr.SdrMode.RX_EQ==0)  ){
      TM_ILI9341_Puts(40, lcd_txt_left, "EQ", &TM_Font_11x18, GREEN, BLACK);
    TM_ILI9341_Puts(40, lcd_txt_left+22, "R", &TM_Font_11x18, GRAY4, BLACK);
    TM_ILI9341_Puts(40, lcd_txt_left+33, "T", &TM_Font_11x18, GREEN, BLACK);
    return;
  }
  
  if( (SdrStr.SdrMode.TX_EQ==0)&&(SdrStr.SdrMode.RX_EQ==0)  )
    TM_ILI9341_Puts(40, lcd_txt_left, "EQRT", &TM_Font_11x18, GRAY4, BLACK);
  if( (SdrStr.SdrMode.TX_EQ==1)&&(SdrStr.SdrMode.RX_EQ==1)  )
    TM_ILI9341_Puts(40, lcd_txt_left, "EQRT", &TM_Font_11x18, GREEN, BLACK);
  
}
//------------------------------------------------------------------------------
void Lcd_RfPwr( void ){

   if(SdrStr.SdrMode.TX_PWR == 0)
    sprintf( text, "P 10%%");
  if(SdrStr.SdrMode.TX_PWR == 1)
    sprintf( text, "P 25%%");
  if(SdrStr.SdrMode.TX_PWR == 2)
    sprintf( text, "P 50%%");
  if(SdrStr.SdrMode.TX_PWR == 3)
    sprintf( text, "P 80%%");
  if(SdrStr.SdrMode.TX_PWR == 4)
    sprintf( text, "P100%%");
  TM_ILI9341_Puts(20, lcd_txt_left, text, &TM_Font_11x18, GREEN, BLACK); 

}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void LcdPwrSet( void ){
  if(SdrStr.SdrMode.TX_PWR == 0)
    sprintf( text, "10%% ");
  if(SdrStr.SdrMode.TX_PWR == 1)
    sprintf( text, "25%% ");
  if(SdrStr.SdrMode.TX_PWR == 2)
    sprintf( text, "50%% ");
  if(SdrStr.SdrMode.TX_PWR == 3)
    sprintf( text, "80%% ");
  if(SdrStr.SdrMode.TX_PWR == 4)
    sprintf( text, "100%% ");
  TM_ILI9341_Puts(0, 36, text, &TM_Font_11x18, YELLOW, BLACK);
  
  Lcd_RfPwr();
}
//------------------------------------------------------------------------------

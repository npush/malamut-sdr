#include "main.h"


//sizeof(SDR_MODE) 289
//sizeof(SDR_SETUP)476

#define SAVE_NUMCRC             14

#define EEP_START               0
#define ADR1_SDR_MODE           12//289
#define ADR2_SDR_MODE           512
#define ADR1_SDR_SETUP          1000//507
#define ADR2_SDR_SETUP          2000
//-----------------------------------------------------------------------------
void EEP_Load( void ){
  

  EEP_SdrMode_Load();

  EEP_Freq_Load();
  //!! set real band from freq

  EEP_SdrSetup_Load();

  
  SdrStr.ValMode = 0;
  SdrStr.ValCount = 0;
   SdrStr.Tmp = 0;
  SdrStr.fTmp = 0;
  
 // SdrSetup.FFT_Size  = 256;
 // SdrSetup.Sample_Rate = 96000;
  
  //SdrStr.SdrSetup.SpecScale = 1;
  //SdrStr.SdrMode.BYPS = 0;
  SdrStr.SdrMode.RTX_Mode = 0; 
}

//-----------------------------------------------------------------------------
unsigned char CRC8_calc(unsigned char crc, unsigned char *message,unsigned int messagesize)
{
volatile int      i;
volatile unsigned char msg;

    while(messagesize-->0){

       msg = *message++;

       for (i=0; i<8; i++)
       {
           if ( (crc ^ msg) &1 )
           {
               crc ^= 0x18;
               crc = ( crc >> 1 ) | 0x80;
           }
           else
               crc = crc >> 1;

           msg = msg >> 1;
       }//for
    }//while
    return crc;
}
//-----------------------------------------------------------------------------

void EEP_Freq_Load( void ){

  unsigned char crc,crc_static,crc_calc=0;
  unsigned long Freq; 
  
  I2C_EE_BufferRead(EEP_START+0, (unsigned char*)&Freq, 4 );
  I2C_EE_BufferRead(EEP_START+4, (unsigned char*)&crc, 1 );
  I2C_EE_BufferRead(EEP_START+5, (unsigned char*)&crc_static, 1 );

  unsigned char*ptr = (unsigned char*)&Freq;
  for( int i=0; i<4; ++i ){
    crc_calc += *(ptr+i);
  }
  if( (crc_calc == crc)&&(crc_static == SAVE_NUMCRC) ){
     SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] =  Freq;
    return;
  }
  
 
  I2C_EE_BufferRead(EEP_START+6, (unsigned char*)&Freq, 4 );
  I2C_EE_BufferRead(EEP_START+10, (unsigned char*)&crc, 1 );
  I2C_EE_BufferRead(EEP_START+11, (unsigned char*)&crc_static, 1 );
  crc_calc=0;
  ptr = (unsigned char*)&Freq;
  for( int i=0; i<4; ++i ){
    crc_calc += *(ptr+i);
  }
  if( (crc_calc == crc)&&(crc_static == SAVE_NUMCRC) ){
    SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] =  Freq;
    return;
  }

  TM_ILI9341_Fill(BLACK);
  TM_ILI9341_Puts(190, 0, "Eep Freq Err p1&p2" , &TM_Font_11x18, WHITE, BLACK);
  sprintf( text, "CRCr = %02X CRCc = %02X", crc, crc_calc  );
  TM_ILI9341_Puts(170, 0, text , &TM_Font_11x18, WHITE, BLACK);
  while( KEY_Current[ SEL ] == 0 );
  TM_ILI9341_Puts(150, 0, "Reload..." , &TM_Font_11x18, WHITE, BLACK);
  delay_ms(500);
  TM_ILI9341_Fill(BLACK);
  
  SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] =  (BandTable[SdrStr.SdrMode.Band][0]+BandTable[SdrStr.SdrMode.Band][1])/2;
  SaveFreq = 1;
}
//-----------------------------------------------------------------------------
void EEP_SdrMode_Load( void ){
  unsigned char crc_calc;
  unsigned char *ptr;
  
  I2C_EE_BufferRead( ADR1_SDR_MODE, (unsigned char*)&SdrStr.SdrMode,sizeof(SDR_MODE) );
  crc_calc=CRC8_calc( SAVE_NUMCRC, (unsigned char*)&SdrStr.SdrMode, sizeof(SDR_MODE)-1 );
  if( (crc_calc == SdrStr.SdrMode.crc_sum)&&(SdrStr.SdrMode.crc_static == SAVE_NUMCRC) ){
    return;
  }
  
  I2C_EE_BufferRead( ADR2_SDR_MODE, (unsigned char*)&SdrStr.SdrMode,sizeof(SDR_MODE) );
  crc_calc=CRC8_calc( SAVE_NUMCRC, (unsigned char*)&SdrStr.SdrMode, sizeof(SDR_MODE)-1 );
  if( (crc_calc == SdrStr.SdrMode.crc_sum)&&(SdrStr.SdrMode.crc_static == SAVE_NUMCRC) ){
    return;
  }

  TM_ILI9341_Fill(BLACK);
  TM_ILI9341_Puts(190, 0, "Eep SdrMode Err p1&p2" , &TM_Font_11x18, WHITE, BLACK);
  sprintf( text, "CRCr = %02X CRCc = %02X", SdrStr.SdrMode.crc_sum, crc_calc  );
  TM_ILI9341_Puts(170, 0, text , &TM_Font_11x18, WHITE, BLACK);
  while( KEY_Current[ SEL ] == 0 );
  TM_ILI9341_Puts(150, 0, "Reload..." , &TM_Font_11x18, WHITE, BLACK);
  delay_ms(500);
  TM_ILI9341_Fill(BLACK);

  
  SdrStr.SdrMode.Band = 2;
  
  for( int i=0; i<TotalBend; ++i  ){
    SdrStr.SdrMode.Freq[i] = ( BandTable[i][0]+BandTable[i][1])/2;
    SdrStr.SdrMode.Filter_SSB[i] = 4;
    SdrStr.SdrMode.Filter_AMFM[i] = 3;
    SdrStr.SdrMode.ATT[i] = 0;
    SdrStr.SdrMode.PRE[i] = 0;
  }
  
  for( int i=0; i<3 ;++i ){
    SdrStr.SdrMode.Mode[i] = LSB;
    SdrStr.SdrMode.AGC_RX[i] = 1;
  }
  for( int i=3; i<10;++i ){
    SdrStr.SdrMode.Mode[i] = USB;
    SdrStr.SdrMode.AGC_RX[i] = 1;
  }
  for( int i=10; i<TotalBend ;++i ){
    SdrStr.SdrMode.Mode[i] = AM;
    SdrStr.SdrMode.AGC_RX[i] = 1;
  }
  SdrStr.SdrMode.AGC_TX = 4;
  
  SdrStr.SdrMode.ANF = 0;
  SdrStr.SdrMode.DNR = 0;
  SdrStr.SdrMode.NB = 0;
  SdrStr.SdrMode.NB_LEVEL = 0;

  SdrStr.SdrMode.BYPS = 0;
  SdrStr.SdrMode.Moni = 0;
  SdrStr.SdrMode.Moni_vol = 100;//(100-SdrStr.SdrMode.Moni_vol)att0dB


  SdrStr.SdrMode.TX_PWR = 2;
  SdrStr.SdrMode.TX_ECHO = 0;
  SdrStr.SdrMode.Echo_del  = 3800;
  SdrStr.SdrMode.Echo_vol = 0.12;
  SdrStr.SdrMode.Echo_rpt = 0.12;
  
  SdrStr.SdrMode.TX_EQ = 0;
  SdrStr.SdrMode.RX_EQ = 0;
  
  /*for( int i=0; i<5; ++i ){ // [-9..9]
    SdrStr.SdrMode.TX_SEQ[i] = 0;
    SdrStr.SdrMode.RX_SEQ[i] = 0;
  }*/
  SdrStr.SdrMode.TX_SEQ[0] =SdrStr.SdrMode.RX_SEQ[0] = 4;
  SdrStr.SdrMode.TX_SEQ[1] =SdrStr.SdrMode.RX_SEQ[1] = 7;
  SdrStr.SdrMode.TX_SEQ[2] =SdrStr.SdrMode.RX_SEQ[2] =-2;
  SdrStr.SdrMode.TX_SEQ[3] =SdrStr.SdrMode.RX_SEQ[3] = 1;
  SdrStr.SdrMode.TX_SEQ[4] =SdrStr.SdrMode.RX_SEQ[4] =-9;

  SdrStr.SdrMode.RTX_Mode = 0;
  
  SdrStr.SdrMode.Menu_Sel = 0;

  

  
  SdrStr.SdrMode.crc_static = SAVE_NUMCRC;
  SdrStr.SdrMode.crc_sum=0;
  for( int i=0; i<sizeof(SDR_MODE)-1; ++i )
    SdrStr.SdrMode.crc_sum += *(ptr+i);   
  SaveSdrMode = 1;
  

}
//-----------------------------------------------------------------------------
void EEP_SdrSetup_Load( void ){
  
  unsigned char crc_calc;
  unsigned char *ptr;
  
  I2C_EE_BufferRead( ADR1_SDR_SETUP, (unsigned char*)&  SdrStr.SdrSetup,sizeof(SDR_SETUP) );
  crc_calc=0;
  ptr = (unsigned char*)&  SdrStr.SdrSetup;
  for( int i=0; i<sizeof(SDR_SETUP)-1; ++i ){
    crc_calc += *(ptr+i);
  }
  if( (crc_calc ==   SdrStr.SdrSetup.crc_sum)&&(  SdrStr.SdrSetup.crc_static == SAVE_NUMCRC) ){
    SdrStr.SdrSetup.FILTER_AMFM[7] = SdrStr.SdrSetup.Sample_Rate/2;
    return;
  }
  
  I2C_EE_BufferRead( ADR2_SDR_SETUP, (unsigned char*)&  SdrStr.SdrSetup,sizeof(SDR_SETUP) );
  crc_calc=0;
  ptr = (unsigned char*)&  SdrStr.SdrSetup;
  for( int i=0; i<sizeof(SDR_SETUP)-1; ++i ){
    crc_calc += *(ptr+i);
  }
  if( (crc_calc ==   SdrStr.SdrSetup.crc_sum)&&(  SdrStr.SdrSetup.crc_static == SAVE_NUMCRC) ){
    SdrStr.SdrSetup.FILTER_AMFM[7] = SdrStr.SdrSetup.Sample_Rate/2;
    return;
  }
  
  TM_ILI9341_Fill(BLACK);
  TM_ILI9341_Puts(190, 0, "Eep SdrSetup Err p1&p2" , &TM_Font_11x18, WHITE, BLACK);
  sprintf( text, "CRCr = %02X CRCc = %02X", SdrStr.SdrSetup.crc_sum, crc_calc  );
  TM_ILI9341_Puts(170, 0, text , &TM_Font_11x18, WHITE, BLACK);
  while( KEY_Current[ SEL ] == 0 );
  TM_ILI9341_Puts(150, 0, "Reload..." , &TM_Font_11x18, WHITE, BLACK);
  delay_ms(500);
  TM_ILI9341_Fill(BLACK);

  SdrStr.SdrSetup.FFT_Size = 2048;
  SdrStr.SdrSetup.FFT_Spect_beta_up = 0.12;
  SdrStr.SdrSetup.FFT_Spect_beta_dwn = 0.78;
  SdrStr.SdrSetup.FFT_Spect_ofset = 43;//43F24/70

  SdrStr.SdrSetup.Sample_Rate = 48000;
  SdrStr.SdrSetup.SpecScale = 1;

  
  SdrStr.SdrSetup.DDS_type = 3;//0-AD9850 1-AD9851 2-AD9951 3-Si5351
  SdrStr.SdrSetup.DDS_mul[0] = 1;
  SdrStr.SdrSetup.DDS_mul[1] = 6;
  SdrStr.SdrSetup.DDS_mul[2] = 20;
  SdrStr.SdrSetup.DDS_mul[3] = 1;
  
  SdrStr.SdrSetup.DDS_mck[0] = 124999318;
  SdrStr.SdrSetup.DDS_mck[1] = 29999947;
  SdrStr.SdrSetup.DDS_mck[2] = 16000000;
  SdrStr.SdrSetup.DDS_mck[3] = 25012700;


  SdrStr.SdrSetup.RX_GAIN = 1000;
  SdrStr.SdrSetup.MIC_GAIN = 50; 


  

  
  SdrStr.SdrSetup.FILTER_SSB[0][0] = 450;SdrStr.SdrSetup.FILTER_SSB[0][1] = 750;
  SdrStr.SdrSetup.FILTER_SSB[1][0] = 200;SdrStr.SdrSetup.FILTER_SSB[1][1] = 1000;
  SdrStr.SdrSetup.FILTER_SSB[2][0] = 200;SdrStr.SdrSetup.FILTER_SSB[2][1] = 2300;
  SdrStr.SdrSetup.FILTER_SSB[3][0] = 200;SdrStr.SdrSetup.FILTER_SSB[3][1] = 2600;
  SdrStr.SdrSetup.FILTER_SSB[4][0] = 120;SdrStr.SdrSetup.FILTER_SSB[4][1] = 2820;
  SdrStr.SdrSetup.FILTER_SSB[5][0] = 120;SdrStr.SdrSetup.FILTER_SSB[5][1] = 3020;
  SdrStr.SdrSetup.FILTER_SSB[6][0] = 20;SdrStr.SdrSetup.FILTER_SSB[6][1] = 3120;
  SdrStr.SdrSetup.FILTER_SSB[7][0] = 0;SdrStr.SdrSetup.FILTER_SSB[7][1] = 17000;

  SdrStr.SdrSetup.FILTER_AMFM[0] = 2700;
  SdrStr.SdrSetup.FILTER_AMFM[1] = 3100;
  SdrStr.SdrSetup.FILTER_AMFM[2] = 4000;
  SdrStr.SdrSetup.FILTER_AMFM[3] = 6000;
  SdrStr.SdrSetup.FILTER_AMFM[4] = 8000;
  SdrStr.SdrSetup.FILTER_AMFM[5] = 10000;
  SdrStr.SdrSetup.FILTER_AMFM[6] = 12000;
  SdrStr.SdrSetup.FILTER_AMFM[7] = 24000;
  
  for( int i=0; i<TotalBend; ++i  ){
    SdrStr.SdrSetup.rx_mag[i] = 1.0;
    SdrStr.SdrSetup.rx_ph[i] = 0;

    SdrStr.SdrSetup.tx_mag[i] = 1.0;
    SdrStr.SdrSetup.tx_ph[i] = 0;
  }

  SdrStr.SdrSetup.tx_mix = 50;
  SdrStr.SdrSetup.tx_fm_dev = 53;//-+2.5kHz
  SdrStr.SdrSetup.tx_am_mod = 50;//0.5
  
  SdrStr.SdrSetup.DNR_Ord = 48;
  SdrStr.SdrSetup.DNR_Del = 96;
  SdrStr.SdrSetup.DNR_Rate   = 0.00001;
  SdrStr.SdrSetup.DNR_Leak = 0.000001;

  SdrStr.SdrSetup.crc_static = SAVE_NUMCRC;
  SdrStr.SdrSetup.crc_sum=0;
  for( int i=0; i<sizeof(SDR_SETUP)-1; ++i )
    SdrStr.SdrSetup.crc_sum += *(ptr+i);   
  SaveSdrSetup = 1;
  

}
//-----------------------------------------------------------------------------
void EEP_Save( void ){

  if( SaveFreq ){
    unsigned char crc_static = SAVE_NUMCRC;
    unsigned char crc_sum = 0;
    unsigned long Freq = SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ];
        
    unsigned char *ptr = (unsigned char*)&Freq;
    for( int i=0; i<4; ++i ){
      crc_sum += *(ptr+i);
    }
    I2C_EE_BufferWrite(EEP_START+0, (unsigned char*)&Freq, 4 );
    I2C_EE_BufferWrite(EEP_START+4, (unsigned char*)&crc_sum, 1 );
    I2C_EE_BufferWrite(EEP_START+5, (unsigned char*)&crc_static, 1 );
    I2C_EE_BufferWrite(EEP_START+6, (unsigned char*)&Freq, 4 );
    I2C_EE_BufferWrite(EEP_START+10, (unsigned char*)&crc_sum, 1 );
    I2C_EE_BufferWrite(EEP_START+11, (unsigned char*)&crc_static, 1 );
    SaveFreq = 0;
  }
      
  if( SaveSdrMode ){
    SdrStr.SdrMode.crc_sum=CRC8_calc( SAVE_NUMCRC, (unsigned char*)&SdrStr.SdrMode, sizeof(SDR_MODE)-1 );
    /*SdrStr.SdrMode.crc_sum = 0;
    unsigned char *ptr = (unsigned char*)&SdrStr.SdrMode;
    for( int i=0; i<sizeof(SDR_MODE)-1; ++i ){
      SdrStr.SdrMode.crc_sum += *(ptr+i);
    }*/
    I2C_EE_BufferWrite(ADR1_SDR_MODE, (unsigned char*)&SdrStr.SdrMode,sizeof(SDR_MODE) );
    I2C_EE_BufferWrite(ADR2_SDR_MODE, (unsigned char*)&SdrStr.SdrMode,sizeof(SDR_MODE) );
    SaveSdrMode = 0;
  }
  if( SaveSdrSetup ){
    SdrStr.SdrSetup.crc_sum = 0;
    unsigned char *ptr = (unsigned char*)&SdrStr.SdrSetup;
    for( int i=0; i<sizeof(SDR_SETUP)-1; ++i ){
      SdrStr.SdrSetup.crc_sum += *(ptr+i);
    }
    I2C_EE_BufferWrite(ADR1_SDR_SETUP, (unsigned char*)&SdrStr.SdrSetup,sizeof(SDR_SETUP) );
    I2C_EE_BufferWrite(ADR2_SDR_SETUP, (unsigned char*)&SdrStr.SdrSetup,sizeof(SDR_SETUP) );
    SaveSdrSetup = 0;
  }  
}
//-----------------------------------------------------------------------------
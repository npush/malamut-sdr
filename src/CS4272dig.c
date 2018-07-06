#include "main.h"

void CS4272dig_init( void ){
  


  GPIO_InitTypeDef GPIO_InitStructure;
   RCC_AHB1PeriphClockCmd( CODEC_CDRST_CLK ,ENABLE);
  //====================== RESET==========================
    
  /* Audio reset pin configuration*/
  GPIO_InitStructure.GPIO_Pin = CODEC_CDRST_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(CODEC_CDRST_PORT, &GPIO_InitStructure);
  
  unsigned long nCount = 0xFFFFF;
  
  GPIO_WriteBit(CODEC_CDRST_PORT, CODEC_CDRST_PIN, Bit_SET);
  for (; nCount != 0; nCount--)
    asm("nop");
 
  GPIO_WriteBit(CODEC_CDRST_PORT, CODEC_CDRST_PIN, Bit_RESET);
  nCount = 0xFFFFF;//25ms
  for (; nCount != 0; nCount--)
    asm("nop");
 
  GPIO_WriteBit(CODEC_CDRST_PORT, CODEC_CDRST_PIN, Bit_SET);
  nCount = 0xF;
  for (; nCount != 0; nCount--)
    asm("nop");
      
 
  
  Spi_init();
   
  Spi_wr ( MODE_CONTROL_2,  0x03,  CODEC_CS1_PIN|CODEC_CS2_PIN );// Reserved        Reserved        Reserved        LOOP            MUTECA=B        FREEZE          CPEN            PDN
  Spi_wr ( MODE_CONTROL_1,  0x81,  CODEC_CS1_PIN|CODEC_CS2_PIN );// M1              M0              Ratio1          Ratio0          M/S             DAC_DIF2        DAC_DIF1        DAC_DIF0
  Spi_wr ( ADC_CONTROL,     0x10,  CODEC_CS1_PIN|CODEC_CS2_PIN );// Reserved        Reserved        Dither16        ADC_DIF0        MUTEA           MUTEB           HPFDisableA     HPFDisableB
  Spi_wr ( MODE_CONTROL_2,  0x02,  CODEC_CS1_PIN|CODEC_CS2_PIN );// Reserved        Reserved        Reserved        LOOP            MUTECA=B        FREEZE          CPEN            PDN
  //Spi_wr ( DAC_CONTROL,     0xF0,  CODEC_CS1_PIN|CODEC_CS2_PIN  );// AMUTE           FILT_SEL        DEM1            DEM0            RMP_UP          RMP_DN          INV_B           INV_A
  
}
void CodecTXPwrSet( void ){

/*
  %     p/      vol
100%    1       0db     4
80%     1.26    -2db    3
50%     2       -6dB    2
25%     4       -12B    1       
10%     10      -20dB   0
 
*/ 
  
  
  if(SdrStr.SdrMode.TX_PWR == 0){//10%
    Spi_wr ( DAC_VOLUME_B,  20,  CODEC_CS_TX );
    Spi_wr ( DAC_VOLUME_A,  20,  CODEC_CS_TX );
  }
  if(SdrStr.SdrMode.TX_PWR == 1){//25%
    Spi_wr ( DAC_VOLUME_B,  12,  CODEC_CS_TX );
    Spi_wr ( DAC_VOLUME_A,  12,  CODEC_CS_TX );
  }
  if(SdrStr.SdrMode.TX_PWR == 2){//50%
    Spi_wr ( DAC_VOLUME_B,  6,  CODEC_CS_TX );
    Spi_wr ( DAC_VOLUME_A,  6,  CODEC_CS_TX );
  }
  if(SdrStr.SdrMode.TX_PWR == 3){//80%
    Spi_wr ( DAC_VOLUME_B,  2,  CODEC_CS_TX );
    Spi_wr ( DAC_VOLUME_A,  2,  CODEC_CS_TX );
  }
  if(SdrStr.SdrMode.TX_PWR == 4){//100%
    Spi_wr ( DAC_VOLUME_B,  0,  CODEC_CS_TX );
    Spi_wr ( DAC_VOLUME_A,  0,  CODEC_CS_TX );
  }
}
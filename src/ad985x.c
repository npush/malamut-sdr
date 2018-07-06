#include "main.h"
#include "Si5351.h"
//#include "Si5351_NT7S.h"
//-----------------------------------------------------------------------------
void AD9850_init( void ){
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD  ,ENABLE);
  
 
  GPIO_InitStructure.GPIO_Pin = DDS_Sck|DDS_Mosi|DDS_UpDate|DDS_Rst|DDS_Cs;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_ResetBits( GPIOD, DDS_Sck|DDS_Mosi|DDS_UpDate|DDS_Rst|DDS_Cs);

  delay_ms(10);
  GPIO_SetBits(GPIOD,DDS_Rst); //????
  delay_ms(10);
  GPIO_ResetBits(GPIOD,DDS_Rst);
  delay_ms(10);

  for( int i=0; i<48; ++i){
    
    asm("nop");  asm("nop");  asm("nop");
    asm("nop");  asm("nop");  asm("nop");
    GPIO_SetBits( GPIOD, DDS_Sck);  
    asm("nop");  asm("nop");  asm("nop");
    asm("nop");  asm("nop");  asm("nop");
    GPIO_ResetBits( GPIOD, DDS_Sck);  
    asm("nop");  asm("nop");  asm("nop");
    asm("nop");  asm("nop");  asm("nop");
  }
  GPIO_SetBits( GPIOD, DDS_UpDate); 
  asm("nop");  asm("nop");  asm("nop");
  asm("nop");  asm("nop");  asm("nop");
  asm("nop");  asm("nop");  asm("nop");
  GPIO_ResetBits( GPIOD, DDS_UpDate); 
  asm("nop");  asm("nop");  asm("nop");
  asm("nop");  asm("nop");  asm("nop");
  asm("nop");  asm("nop");  asm("nop");  
  
  SdrStr.SdrSetup.koeff_DDS =  (float)(SdrStr.SdrSetup.DDS_mck[SdrStr.SdrSetup.DDS_type]*SdrStr.SdrSetup.DDS_mul[SdrStr.SdrSetup.DDS_type]) / 4294967295.0;
  
}
//-----------------------------------------------------------------------------
void ad9850_freq( unsigned long freq ){
  
  unsigned long  temp=(unsigned long)(( double)freq/SdrStr.SdrSetup.koeff_DDS);
  
 /* GPIO_ResetBits( GPIOD, DDS_UpDate); 
  asm("nop");  asm("nop");  asm("nop");
  asm("nop");  asm("nop");  asm("nop");
  asm("nop");  asm("nop");  asm("nop");*/

  for( int i=0; i<32; ++i){
    
    if( (temp&0x1)==0x1)
      GPIO_SetBits( GPIOD, DDS_Mosi); 
    else
      GPIO_ResetBits( GPIOD, DDS_Mosi); 

    asm("nop");  asm("nop");  asm("nop");
    asm("nop");  asm("nop");  asm("nop");
    GPIO_SetBits( GPIOD, DDS_Sck);  
    asm("nop");  asm("nop");  asm("nop");
    asm("nop");  asm("nop");  asm("nop");
    GPIO_ResetBits( GPIOD, DDS_Sck);  
    asm("nop");  asm("nop");  asm("nop");
    asm("nop");  asm("nop");  asm("nop");

    temp >>=1;
  }


  temp = 0;

  //if(SdrSetup.DDS_type == 0)
  //  temp = 0;

  if((SdrStr.SdrSetup.DDS_type == 1)&&(SdrStr.SdrSetup.DDS_mul[1]==6))
    temp = 1;


  for( int i=0; i<8; ++i){
    if( (temp&0x1)==0x1)
      GPIO_SetBits( GPIOD, DDS_Mosi); 
    else
      GPIO_ResetBits( GPIOD, DDS_Mosi); 

    asm("nop");  asm("nop");  asm("nop");
    asm("nop");  asm("nop");  asm("nop");
    GPIO_SetBits( GPIOD, DDS_Sck);  
    asm("nop");  asm("nop");  asm("nop");
    asm("nop");  asm("nop");  asm("nop");
    GPIO_ResetBits( GPIOD, DDS_Sck);  
    asm("nop");  asm("nop");  asm("nop");
    asm("nop");  asm("nop");  asm("nop");

    temp >>=1;
  
  }

  
  asm("nop");  asm("nop");  asm("nop");
  asm("nop");  asm("nop");  asm("nop");
  asm("nop");  asm("nop");  asm("nop");

  //GPIO_SetBits( GPIOD, DDS_UpDate); 
  
  GPIO_SetBits( GPIOD, DDS_UpDate); 
  asm("nop");  asm("nop");  asm("nop");
  asm("nop");  asm("nop");  asm("nop");
  asm("nop");  asm("nop");  asm("nop");
  GPIO_ResetBits( GPIOD, DDS_UpDate); 
  asm("nop");  asm("nop");  asm("nop");
  asm("nop");  asm("nop");  asm("nop");
  asm("nop");  asm("nop");  asm("nop");  
  
}
//-----------------------------------------------------------------------------




//**********************************************************
#define   ad9951_CFR1 0x00
#define   ad9951_CFR2 0x01
#define   ad9951_ASF  0x02
#define   ad9951_ARR  0x03
#define   ad9951_FTW0 0x04
#define   ad9951_POW0 0x05
//**********************************************************
//-----------------------------------------------------------------------------
void AD995x_init( void ){
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD  ,ENABLE);
  
 
  GPIO_InitStructure.GPIO_Pin = DDS_Sck|DDS_Mosi|DDS_UpDate|DDS_Rst|DDS_Cs;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_ResetBits( GPIOD, DDS_Sck|DDS_Mosi|DDS_UpDate|DDS_Rst|DDS_Cs);

  
  delay_ms(10);
  GPIO_SetBits(GPIOD,DDS_Rst); 
  delay_ms(10);
  GPIO_ResetBits(GPIOD,DDS_Rst);
  delay_ms(25);
  
  
  SdrStr.SdrSetup.koeff_DDS =  (float)(SdrStr.SdrSetup.DDS_mck[SdrStr.SdrSetup.DDS_type]*SdrStr.SdrSetup.DDS_mul[SdrStr.SdrSetup.DDS_type]) / 4294967295.0;
  
 ad995x_freq( 0 );
 
  unsigned long data;
  
  data = 0x00200008;
  AD995x_write( ad9951_CFR1, (unsigned char*)&data, 4 );
 
  data = 0x040200 | ((SdrStr.SdrSetup.DDS_mul[SdrStr.SdrSetup.DDS_type]<<3)&0xFF);
  AD995x_write( ad9951_CFR2, (unsigned char*)&data, 3 );//40
 
  data = 0xC450;
  AD995x_write( ad9951_ASF, (unsigned char*)&data, 2 );//C450
 
  data = 0xFC;
  AD995x_write( ad9951_ARR, (unsigned char*)&data, 1 );
  
  //SetMode_AD9951( Freq_AD9951 );
  //AD995x_write( ad9951_FTW0, (unsigned char*)&f, 4 );
  ad995x_freq( 0 );
 
  data = 0x0000;
  AD995x_write( ad9951_POW0, (unsigned char*)&data, 2 );
  
 
  delay_ms(50);
}
//-----------------------------------------------------------------------------
void AD995x_write( unsigned char reg, unsigned char *data, int leght )
{
  unsigned long  temp, k;

  GPIO_ResetBits( GPIOD, DDS_Cs); 
  temp = reg;

  for( int i=0; i<8; ++i){
    
    if( (temp&0x80)==0x80)
      GPIO_SetBits( GPIOD, DDS_Mosi); 
    else
      GPIO_ResetBits( GPIOD, DDS_Mosi); 

    asm("nop");  asm("nop");  asm("nop");
    asm("nop");  asm("nop");  asm("nop");
    GPIO_SetBits( GPIOD, DDS_Sck);  
    asm("nop");  asm("nop");  asm("nop");
    asm("nop");  asm("nop");  asm("nop");
    GPIO_ResetBits( GPIOD, DDS_Sck);  
    asm("nop");  asm("nop");  asm("nop");
    asm("nop");  asm("nop");  asm("nop");

    temp <<=1;
  }
  
  memcpy( (unsigned char*)&temp, data, leght );
  k=0x01<<((leght*8)-1);
  
  for( int i=0; i<leght*8; ++i){
    
    if( (temp&k)==k)
      GPIO_SetBits( GPIOD, DDS_Mosi); 
    else
      GPIO_ResetBits( GPIOD, DDS_Mosi); 

    asm("nop");  asm("nop");  asm("nop");
    asm("nop");  asm("nop");  asm("nop");
    GPIO_SetBits( GPIOD, DDS_Sck);  
    asm("nop");  asm("nop");  asm("nop");
    asm("nop");  asm("nop");  asm("nop");
    GPIO_ResetBits( GPIOD, DDS_Sck);  
    asm("nop");  asm("nop");  asm("nop");
    asm("nop");  asm("nop");  asm("nop");

    temp <<=1;
  }

  GPIO_SetBits( GPIOD, DDS_Cs); 
  
  asm("nop");  asm("nop");  asm("nop");
  GPIO_SetBits( GPIOD, DDS_UpDate); 
  asm("nop");  asm("nop");  asm("nop");
  GPIO_ResetBits( GPIOD, DDS_UpDate); 
  asm("nop");  asm("nop");  asm("nop");
  asm("nop");  asm("nop");  asm("nop");
}
//-----------------------------------------------------------------------------
void ad995x_freq( unsigned long freq ){
  
  unsigned long  temp=(unsigned long)(( double)freq/SdrStr.SdrSetup.koeff_DDS);
  
  AD995x_write( ad9951_FTW0, (unsigned char*)&temp, 4 );
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void Si5351_init( void ){
  si5351_init();
}
//-----------------------------------------------------------------------------
void Si5351_freq( unsigned long freq ){
  si5351_set_freq(freq, SI5351_CLK0);
}
//-----------------------------------------------------------------------------


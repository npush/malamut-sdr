#include "main.h"

//-----------------------------------------------------------------------------
void DDS_init( void ){
  if( SdrStr.SdrSetup.DDS_type < 2 ){//AD9850 AD9851
    /*si5351_clock_enable( SI5351_CLK0, 0 );
    si5351_clock_enable( SI5351_CLK1, 0 );
    si5351_clock_enable( SI5351_CLK2, 0 );*/
    AD9850_init();
  }
  if( SdrStr.SdrSetup.DDS_type == 2 ){
    /*si5351_clock_enable( SI5351_CLK0, 0 );
    si5351_clock_enable( SI5351_CLK1, 0 );
    si5351_clock_enable( SI5351_CLK2, 0 );*/
    AD995x_init();
  }
  if( SdrStr.SdrSetup.DDS_type == 3 ){
    Si5351_init();
    Si5351_freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] * 2 );
   /* si5351_clock_enable( SI5351_CLK0, 1 );
    si5351_clock_enable( SI5351_CLK1, 0 );
    si5351_clock_enable( SI5351_CLK2, 0 );*/
  }
    
}
//-----------------------------------------------------------------------------
void DDS_freq( unsigned long freq ){
  if( SdrStr.SdrSetup.DDS_type < 2 )//AD9850 AD9851
    ad9850_freq( freq );
  if( SdrStr.SdrSetup.DDS_type == 2 )
   ad995x_freq( freq );
  if( SdrStr.SdrSetup.DDS_type == 3 )
   ;//Si5351_freq( freq );
 
  BPF_set();
}
//-----------------------------------------------------------------------------

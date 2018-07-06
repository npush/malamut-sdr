#include "main.h"

#define SmetrX  44
#define SmetrY  153

float s_metr_strong=0;
unsigned long sm_time;
//------------------------------------------------------------------------------
void S_meter_line(void){
  TM_ILI9341_DrawLine( SmetrX, SmetrY-3 , SmetrX+104, SmetrY-3, WHITE);
  TM_ILI9341_DrawLine( SmetrX+105, SmetrY-3 , SmetrX+208, SmetrY-3, RED);
  /*TM_ILI9341_DrawLine( SmetrX, SmetrY-5 , SmetrX+104, SmetrY-5, WHITE);
  TM_ILI9341_DrawLine( SmetrX+105, SmetrY-5 , SmetrX+208, SmetrY-5, RED);*/
  
  TM_ILI9341_DrawLine( SmetrX, SmetrY+7 , SmetrX+104, SmetrY+7, WHITE);
  TM_ILI9341_DrawLine( SmetrX+105, SmetrY+7 , SmetrX+208, SmetrY+7, RED);
  /*TM_ILI9341_DrawLine( SmetrX, SmetrY+9 , SmetrX+104, SmetrY+9, WHITE);
  TM_ILI9341_DrawLine( SmetrX+105, SmetrY+9 , SmetrX+208, SmetrY+9, RED);*/
  
  int x=SmetrX;
  for( int i=0;i<10;++i ){
    TM_ILI9341_DrawPixel( x, SmetrY-4, WHITE );
    TM_ILI9341_DrawPixel( x, SmetrY-5, WHITE );
    TM_ILI9341_DrawPixel( x, SmetrY+8, WHITE );
    TM_ILI9341_DrawPixel( x, SmetrY+9, WHITE );
    if( i&0x01 )
      x+=12;
    else
      x+=11;
  }
  
  x=SmetrX+105+17;
  for( int i=1;i<7;++i ){
    TM_ILI9341_DrawPixel( x, SmetrY-4, RED );
    TM_ILI9341_DrawPixel( x, SmetrY-5, RED );
    TM_ILI9341_DrawPixel( x, SmetrY+8, RED );
    TM_ILI9341_DrawPixel( x, SmetrY+9, RED );
      x+=17;
  }
}
//------------------------------------------------------------------------------
void S_meter(void){
  
  char text_agc[8];
  float s_metr_get;
  float s_metr_calc;
  
  s_metr_get=(unsigned int)(20.0f*log10f((SdrStr.Smetr)));
#ifdef F24  
  s_metr_get-=46;
#else
  s_metr_get-=90;
#endif
  
  if (s_metr_get<=0)
   s_metr_get=0;

  /*SmetrDraw( s_metr_strong );
  if( (s_metr_strong_temp > s_metr_strong) &&( Interval_RTC_Get( sm_time ) < 300 ))
    return;
  sm_time = RTC_Get();
  s_metr_strong_temp = s_metr_strong;*/

  float  alpha_dwn;
  alpha_dwn = 0.03;
  SmetrDrawPeak( s_metr_strong, BLACK );
  if( s_metr_get >  s_metr_strong){
     s_metr_strong = s_metr_get;
     sm_time = RTC_Get();
  }else{
    if( Interval_RTC_Get( sm_time ) > 1000 )
      s_metr_strong = s_metr_get*alpha_dwn + s_metr_strong*(1.0-alpha_dwn);
  }
  SmetrDraw( s_metr_get );
  SmetrDrawPeak( s_metr_strong, WHITE );

  

#ifdef F24
  s_metr_calc= s_metr_strong;
  if( s_metr_calc <36 ){
    s_metr_calc/=3.88888;
    sprintf( text_agc,"S%d  ", (unsigned long)s_metr_calc );
  }
  else{
    s_metr_calc = ((s_metr_calc - 35 )/6.16666);
    sprintf( text_agc,"S+%d ", (unsigned long)(s_metr_calc*10.0) );
 }
#else
   s_metr_calc= s_metr_strong;
  if( s_metr_calc <41 ){
    s_metr_calc/=4.44444444;
    sprintf( text_agc,"S%d  ", (unsigned long)s_metr_calc );
  }
  else{
    s_metr_calc = ((s_metr_strong - 40 )/6.5);
    sprintf( text_agc,"S+%d ", (unsigned long)(s_metr_calc*10.0) );
 }
#endif
 
 //lcd_text(3,0, text_agc);
  if( strlen(text_agc)>3 )
    text_agc[4] = 0;
  TM_ILI9341_Puts(145, 0, text_agc, &TM_Font_11x18, WHITE, BLACK);
}
//------------------------------------------------------------------------------
int sdraw_a=0, sdraw_b=0;
void SmetrDraw( float data ){

#ifdef F24  
  sdraw_a = (int)data*3;
#else
  sdraw_a = (int)data*2.63;
#endif
  
  if(sdraw_a >208)
    sdraw_a = 208;
  
  if(sdraw_a < 0 )
    sdraw_a = 1;
  
  if( sdraw_a >  sdraw_b){
    //TM_ILI9341_DrawFilledRectangle( x+sdraw_b, y, x+sdraw_a, y+5, GREEN );
    for( int i=0; i<5; ++i )
    SmetrLineUP_Spectr2( SmetrX+sdraw_b, SmetrX+sdraw_a, SmetrY+i, Waterfall_gamma  );
  }
  else{
    TM_ILI9341_DrawFilledRectangle( SmetrX+sdraw_a, SmetrY, SmetrX+sdraw_b, SmetrY+5, BLACK );
  }
  sdraw_b = sdraw_a;
}
//------------------------------------------------------------------------------
void SmetrDrawPeak( float data, int color ){
  int s_meter_peak;
#ifdef F24  
  s_meter_peak = (int)data*3;
#else
  s_meter_peak = (int)data*2.63;
#endif
  
  if(s_meter_peak >208)
    s_meter_peak = 208;
  
  if(s_meter_peak < 0 )
    s_meter_peak = 1;
  
  LineUP( SmetrX+s_meter_peak,  SmetrY-1,  SmetrY+6, color);
  LineUP( SmetrX+1+s_meter_peak,  SmetrY-1,  SmetrY+6, color);
}
//------------------------------------------------------------------------------
void SmetrLineUP_Spectr2(int x1, int x2, int y, unsigned short *color){
  TM_ILI9341_SendCommand(ILI9341_MAC);
#ifdef ROTATE_LCD
  TM_ILI9341_SendData(0xA8);
#else
  TM_ILI9341_SendData(0x68);
#endif
  unsigned short tmp_color, p;
  
  TM_ILI9341_SetCursorPosition(x1, y, 320, 320);
  //TM_ILI9341_SendCommand(ILI9341_GRAM);
  ILI9341_WRX_RESET;
  ILI9341_CS_RESET;
  TM_SPI_Send(ILI9341_SPI, ILI9341_GRAM);
  ILI9341_WRX_SET;
  for( int i=x1; i<=x2; ++i ){
    p=i+60;
    if( p >255 )p=255;
    tmp_color = color[ p ];
    TM_SPI_Send(ILI9341_SPI, *(((unsigned char*)&tmp_color)+1));
    TM_SPI_Send(ILI9341_SPI, *(((unsigned char*)&tmp_color)+0));
  }
  ILI9341_CS_SET;

  TM_ILI9341_SendCommand(ILI9341_MAC);
#ifdef ROTATE_LCD
  TM_ILI9341_SendData(0xA8);
#else
  TM_ILI9341_SendData(0x68);
#endif
}
//------------------------------------------------------------------------------












//------------------------------------------------------------------------------
__IO float /*swr,*/ power=0,power_t=0 ;
__IO unsigned long SWD_FT=0, SWD_RT=0, ADC_REV, ADC_FWD;

void SWR_meter_draw( void ){
  SWD_FT=0, SWD_RT=0;
  power=0, power_t=0 ;
}
void SWR_meter( void ){

  ADC_FWD = adc_FWD;
  ADC_REV = adc_REV;
  

  float x = adc_delta_swr *(float)ADC_FWD;
  //power = (4.1294*x-0.645882352941)*exp(((x-0.2)*(x-1.9)*(1/(1.7*x))));
  power = ((x*10.0)*(x*10.0))/50.0;
  if( power < 0 )
    power = 0;

  if( power < power_t )
    power = power_t*0.995 + power*0.005;
  power_t = power;
  
  //return;
  

//  sprintf(text,  "S%1.1f %03d %03d P%1.1f ", swr, ADC_FWD, ADC_REV  , power);
 // sprintf(text,  "P%1.1f  Vf=%1.3f ", power, (float)ADC_FWD*adc_delta_swr);
  //sprintf(text,  "SWR=%1.1f P=%1.1f ", swr, power);
  //TM_ILI9341_Puts(60, 0, text, &TM_Font_11x18, WHITE, BLACK);
  
  
  int serINT=0;
  if( ADC_FWD > 50 ){
    if(ADC_REV >= ADC_FWD)
      serINT = 1000;
    else
      serINT = (int)(  10.0f*((float)(ADC_FWD+ADC_REV)/(float)(ADC_FWD-ADC_REV))  );
  }
  else
    serINT = 1;
  
  if( serINT >999 )
    sprintf(text,  "SWR=>100");
  else
    sprintf(text,  "SWR=%2d.%d ", serINT/10, serINT%10);
  TM_ILI9341_Puts(60, 0, text, &TM_Font_11x18, WHITE, BLACK);

  
  sprintf(text,  "P=%1.1f ", power);
  TM_ILI9341_Puts(60, 144, text, &TM_Font_11x18, WHITE, BLACK);
  

  
  int x_meter = 0;
  int y_fwd=35;
  int y_rev=25;
  int xa, xb, color, cnt;
  
  
  if( ADC_FWD > SWD_FT ){
    color = GREEN;
    xa = SWD_FT/16;
    xb = ADC_FWD/16;
  }else{
    color = BLACK;
    xb = SWD_FT/16;
    xa = ADC_FWD/16;
  }
  cnt = (xb-xa)*5;
  SWD_FT = ADC_FWD;
  
  TM_ILI9341_SendCommand(ILI9341_MAC);
  #ifdef ROTATE_LCD
  TM_ILI9341_SendData(0xA8);
  #else
  TM_ILI9341_SendData(0x68);
  #endif

  TM_ILI9341_SetCursorPosition(x_meter+xa, y_fwd, x_meter+xb-1, y_fwd+5-1);
  ILI9341_WRX_RESET;
  ILI9341_CS_RESET;
  TM_SPI_Send(ILI9341_SPI, ILI9341_GRAM);
  ILI9341_WRX_SET;
  for( int i=0; i<=cnt; ++i ){
    TM_SPI_Send(ILI9341_SPI, *(((unsigned char*)&color)+1));
    TM_SPI_Send(ILI9341_SPI, *(((unsigned char*)&color)+0));
  }
  ILI9341_CS_SET;

  TM_ILI9341_SendCommand(ILI9341_MAC);
  #ifdef ROTATE_LCD
  TM_ILI9341_SendData(0xA8);
  #else
  TM_ILI9341_SendData(0x68);
  #endif  
  
  if( ADC_REV > SWD_RT ){
    color = GREEN;
    xa = SWD_RT/16;
    xb = ADC_REV/16;
  }else{
    color = BLACK;
    xb = SWD_RT/16;
    xa = ADC_REV/16;
  }
  cnt = (xb-xa)*5;
  SWD_RT = ADC_REV;
  
  TM_ILI9341_SendCommand(ILI9341_MAC);
  #ifdef ROTATE_LCD
  TM_ILI9341_SendData(0xA8);
  #else
  TM_ILI9341_SendData(0x68);
  #endif

  TM_ILI9341_SetCursorPosition(x_meter+xa, y_rev, x_meter+xb-1, y_rev+5-1);
  ILI9341_WRX_RESET;
  ILI9341_CS_RESET;
  TM_SPI_Send(ILI9341_SPI, ILI9341_GRAM);
  ILI9341_WRX_SET;
  for( int i=0; i<=cnt; ++i ){
    TM_SPI_Send(ILI9341_SPI, *(((unsigned char*)&color)+1));
    TM_SPI_Send(ILI9341_SPI, *(((unsigned char*)&color)+0));
  }
  ILI9341_CS_SET;

  TM_ILI9341_SendCommand(ILI9341_MAC);
  #ifdef ROTATE_LCD
  TM_ILI9341_SendData(0xA8);
  #else
  TM_ILI9341_SendData(0x68);
  #endif  
}
//------------------------------------------------------------------------------

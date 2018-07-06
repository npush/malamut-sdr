#include "main.h"

//-----------------------------------------------------------------------------
void Clock_init( void ){
  
  RTC_InitTypeDef rtc;
  
  RCC_BackupResetCmd(DISABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_AHB1Periph_BKPSRAM, ENABLE);

  PWR_BackupAccessCmd(ENABLE);

  //RCC_BackupResetCmd(ENABLE);
  //RCC_BackupResetCmd(DISABLE);

  RCC_LSEConfig(RCC_LSE_ON);
  #ifdef pcbASDR_V1
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
  #endif
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  RCC_RTCCLKCmd(ENABLE);
  RTC_WaitForSynchro();

  RTC_StructInit(&rtc);
  rtc.RTC_HourFormat = RTC_HourFormat_24;
  rtc.RTC_AsynchPrediv = 0x7F;
  rtc.RTC_SynchPrediv = 0xFF;
  RTC_Init(&rtc);
  
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_DateTypeDef RTC_DateStruct;
  RTC_GetTime(RTC_HourFormat_24, &RTC_TimeStruct);
  RTC_GetDate(RTC_HourFormat_24, &RTC_DateStruct);

//  Lcd_Clock( RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes,':' );  
  Lcd_Clock_wDate( RTC_DateStruct.RTC_Date, RTC_DateStruct.RTC_Month, RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, ':'  );
}
//-----------------------------------------------------------------------------
unsigned long clock_update;
void Clock( void ){
  if( Interval_RTC_Get(clock_update )<1000)
    return;
  unsigned char c=':';
  if( clock_update&0x01  )
    c=' ';
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_DateTypeDef RTC_DateStruct;
  RTC_GetTime(RTC_HourFormat_24, &RTC_TimeStruct);
  RTC_GetDate(RTC_HourFormat_24, &RTC_DateStruct);
  //Lcd_Clock( RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, c );
  Lcd_Clock_wDate( RTC_DateStruct.RTC_Date, RTC_DateStruct.RTC_Month, RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, c  );
  clock_update = RTC_Get();
  if( c==' ' )
    clock_update&=(~0x01);
  else
    clock_update|=1;
}
//-----------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------
void initRTC()
{
  RTC_InitTypeDef rtc;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  PWR_BackupAccessCmd(ENABLE);

  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);

  RCC_LSICmd(ENABLE);
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  RCC_RTCCLKCmd(ENABLE);

  RTC_StructInit(&rtc);
  rtc.RTC_HourFormat = RTC_HourFormat_24;
  rtc.RTC_SynchPrediv = 0x7FFF;
  RTC_Init(&rtc);
}
//-----------------------------------------------------------------------------
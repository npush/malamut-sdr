#ifndef __SYSTIME_H_
#define __SYSTIME_H_

void SysTime_init( void );
unsigned long RTC_Get( void );
unsigned long Interval_RTC_Get( unsigned long time_point );
void delay_ms( unsigned int tm );


#endif

#include "main.h"

//------------------------------------------------------------------------------
void SysTime_init( void ){
  //SysTick_Config(168000);
}
//------------------------------------------------------------------------------
volatile unsigned long time_tic;
void SysTick_Handler( void ){

}
//------------------------------------------------------------------------------
unsigned long RTC_Get( void )
{
    return time_tic;
}
//------------------------------------------------------------------------------

unsigned long Interval_RTC_Get( unsigned long time_point )
{
	unsigned long currentTime;
	unsigned long deltaTime;

	currentTime=RTC_Get();
    if (time_point>currentTime)
		deltaTime=(unsigned long)abs(currentTime + 0xffffffff - time_point);
	else
        deltaTime=(unsigned long)(currentTime-time_point);

    return deltaTime;
}
//------------------------------------------------------------------------------
void delay_ms( unsigned int tm )
{
  unsigned  long i;
    i = RTC_Get( );
  while( Interval_RTC_Get( i )<= tm);
}
//------------------------------------------------------------------------------


#ifndef __LCD_H_
#define __LCD_H_

#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_fonts.h"

void Lcd_init( void );
void Lcd_Spectr_init( int type );
void Lcd_Freq( int freq );
void Lcd_Spectr(int wf_x, int wf_y, int wf_l, int tx_rx);

void LcdMode( void );
void LcdFilters( void );
void LcdFiltersSet( void );

void Lcd_Clock( unsigned char h, unsigned char m, unsigned char c );
void Lcd_Clock_wDate( unsigned char d,unsigned char mth,unsigned char h, unsigned char m, unsigned char c );

void Lcd_Dbg( void );

void Lcd_DCVolt( void );

void Lcd_Pre( void );
void Lcd_Att( void );
void Lcd_Anf( void );
void Lcd_Dnr( void );
void Lcd_Nb( void );
void Lcd_Agc( void );
void Lcd_Moni( void );
void Lcd_Echo( void );
void Lcd_Eq( void );
void Lcd_RfPwr( void );


void LcdPwrSet( void );

#endif

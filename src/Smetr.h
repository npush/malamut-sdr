#ifndef __SMETR_H
#define __SMETR_H

void S_meter_line(void);
void S_meter(void);
void SmetrDraw( float data );
void SmetrDrawPeak( float data, int color );

void SmetrLineUP_Spectr2(int x1, int x2, int y, unsigned short *color);

void SWR_meter_draw( void );
void SWR_meter( void );
#endif
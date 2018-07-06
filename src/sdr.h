#ifndef __SDR_H_
#define __SDR_H_

#define FFT_FILTER_SIZE 2048


#define LedTx_CLK       RCC_AHB1Periph_GPIOD
#define LedTx_PORT      GPIOD
#define LedTx_PIN       GPIO_Pin_8

#define LedRx_CLK       RCC_AHB1Periph_GPIOD
#define LedRx_PORT      GPIOD
#define LedRx_PIN       GPIO_Pin_8

//==================================================
#pragma pack(1)
typedef struct _COMPLEX_FLOAT{
 float re;
 float im;
}COMPLEX_float;
//==================================================
#pragma pack(1)
typedef struct _COMPLEX_LONG{
 long re;
 long im;
}COMPLEX_long;
//==================================================
//==================================================
#pragma pack(1)
typedef struct _COMPLEX_SHORT{
 short re;
 short im;
}COMPLEX_short;
//==================================================
//==================================================

void SDR_init( void );
void sdr_rtx_mode( int st_tx );

void InitTimeInt1ms( void );

#endif
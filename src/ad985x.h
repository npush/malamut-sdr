#ifndef __AD9850_H_
#define __AD9850_H_

#include "pcb.h"

#ifdef pcbASDR_V1
  #define DDS_Cs        GPIO_Pin_12
  #define DDS_Sck       GPIO_Pin_13
  #define DDS_Mosi      GPIO_Pin_14
  #define DDS_UpDate    GPIO_Pin_15
  #define DDS_Rst       GPIO_Pin_11
#else
  #define DDS_Cs        GPIO_Pin_11
  #define DDS_Sck       GPIO_Pin_9
  #define DDS_Mosi      GPIO_Pin_10
  #define DDS_UpDate    GPIO_Pin_11
  #define DDS_Rst       GPIO_Pin_11
#endif
void AD9850_init( void );
void ad9850_freq( unsigned long freq );

void AD995x_init( void );
void AD995x_write( unsigned char reg, unsigned char *data, int leght );
void ad995x_freq( unsigned long freq );

void Si5351_init( void );
void Si5351_freq( unsigned long freq );
#endif

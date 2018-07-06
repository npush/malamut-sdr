#ifndef __CS4272_I2S_H_
#define __CS4272_I2S_H_


void CS4272_I2S_init( void );


void RX_DMAInit( unsigned char* pDMAbuf_IN, unsigned char* pDMAbuf_OUT, int sizeDMAbuf );
void DMA1_Stream7_IRQHandler( void );

void TX_DMAInit( unsigned char* pDMAbuf_IN, unsigned char* pDMAbuf_OUT, int sizeDMAbuf );
void DMA1_Stream4_IRQHandler( void );



void CS_RX_Init(  unsigned char* pDMAbuf_IN, unsigned char* pDMAbuf_OUT, int sizeDMAbuf );
void CS_RX_DeInit( void );
void CS_TX_Init(  unsigned char* pDMAbuf_IN, unsigned char* pDMAbuf_OUT, int sizeDMAbuf );
void CS_TX_DeInit( void );

#endif
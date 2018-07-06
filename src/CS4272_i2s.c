#include "main.h"


unsigned char need_next = 0;

//-----------------------------------------------------------------------------
void CS4272_I2S_init( void ){
  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA| RCC_AHB1Periph_GPIOB| RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE ,ENABLE);
  
 
  /* CODEC_I2S pins configuration: WS, SCK and SD pins */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;//  CK/extSD/SD-RXcodac
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ;//  WS-RXcodac
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Connect pins to I2S peripheral  */
  GPIO_PinAFConfig( GPIOA, GPIO_PinSource15, GPIO_AF_SPI3);
  GPIO_PinAFConfig( GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
  GPIO_PinAFConfig( GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
  GPIO_PinAFConfig( GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

 /* CODEC_I2S pins configuration: MCK pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;// MCK-RXcodac
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* Connect pins to I2S peripheral  */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_SPI3);
  
  /* CODEC_I2S pins configuration: WS, SCK and SD pins */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//  CK/extSD/SD-TXcodac
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;//  WS-TXcodac
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Connect pins to I2S peripheral  */
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource12, GPIO_AF_SPI2);
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

 /* CODEC_I2S pins configuration: MCK pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;// MCK-TXcodac
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* Connect pins to I2S peripheral  */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_SPI2);
  
   
  

}
//-----------------------------------------------------------------------------
void RX_DMAInit( unsigned char* pDMAbuf_IN, unsigned char* pDMAbuf_OUT, int sizeDMAbuf ){
  DMA_InitTypeDef DMA_InitStructure; 
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);


  DMA_Cmd(DMA1_Stream7, DISABLE);
  DMA_DeInit(DMA1_Stream7);
  while (DMA_GetCmdStatus(DMA1_Stream7) != DISABLE)
      {}
  DMA_ClearFlag(DMA1_Stream7, 0xFFFFFFFF);
  
  DMA_DeInit(DMA1_Stream7);
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;
  DMA_InitStructure.DMA_PeripheralBaseAddr =(unsigned int)&(SPI3->DR);
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&pDMAbuf_OUT[0];
  DMA_InitStructure.DMA_BufferSize = (uint32_t)sizeDMAbuf;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  //DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  //DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  //DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  //DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
  
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream7, &DMA_InitStructure);
  DMA_Cmd(DMA1_Stream7, ENABLE);


  DMA_Cmd(DMA1_Stream2, DISABLE);
  DMA_DeInit(DMA1_Stream2);
  while (DMA_GetCmdStatus(DMA1_Stream2) != DISABLE)
      {}  
  DMA_ClearFlag(DMA1_Stream2, 0xFFFFFFFF);

  DMA_DeInit(DMA1_Stream2);
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;
  DMA_InitStructure.DMA_PeripheralBaseAddr =(unsigned int)&(I2S3ext->DR);
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&pDMAbuf_IN[0];
  DMA_InitStructure.DMA_BufferSize = (uint32_t)sizeDMAbuf;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  //DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// Byte HalfWord Word
  //DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
 
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  if( pDMAbuf_IN != NULL ){
    DMA_Init(DMA1_Stream2, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream2, ENABLE);
  }

  

  DMA_ITConfig(DMA1_Stream7, DMA_IT_TC|DMA_IT_HT, ENABLE);
  NVIC_InitTypeDef   NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RXDMACodac_Pre;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = RXDMACodac_Sub;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  
  SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Tx, ENABLE);

  I2S3ext->CR2|=SPI_CR2_RXDMAEN;//
  I2S3ext->I2SCFGR|=SPI_I2SCFGR_I2SE;//включил SPI


}
//-----------------------------------------------------------------------------
void DMA1_Stream7_IRQHandler( void ){

  if (DMA_GetFlagStatus(DMA1_Stream7, DMA_FLAG_TCIF7) != RESET)
  {
    DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);

    if( SdrStr.SdrMode.RTX_Mode == 0) // no moni  change
      need_next = 1;
  }


  if (DMA_GetFlagStatus(DMA1_Stream7, DMA_FLAG_HTIF7) != RESET)
  {

    DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_HTIF7);

    if( SdrStr.SdrMode.RTX_Mode == 0) // no moni change
      need_next = 2;
  }
}
//-----------------------------------------------------------------------------







//-----------------------------------------------------------------------------

void TX_DMAInit( unsigned char* pDMAbuf_IN, unsigned char* pDMAbuf_OUT, int sizeDMAbuf ){
  DMA_InitTypeDef DMA_InitStructure; 
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);


  //DMA_Cmd(DMA1_Stream4, DISABLE);
 // DMA_DeInit(DMA1_Stream4);
  
  DMA_Cmd(DMA1_Stream4, DISABLE);
  DMA_DeInit(DMA1_Stream4);
  while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE)
      {}
  DMA_ClearFlag(DMA1_Stream4, 0xFFFFFFFF);
  
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;
  DMA_InitStructure.DMA_PeripheralBaseAddr =(unsigned int)&(SPI2->DR);
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&pDMAbuf_OUT[0];
  DMA_InitStructure.DMA_BufferSize = (uint32_t)sizeDMAbuf;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  //DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  //DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
 
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream4, &DMA_InitStructure);
  DMA_Cmd(DMA1_Stream4, ENABLE);


 // DMA_Cmd(DMA1_Stream3, DISABLE);
  //DMA_DeInit(DMA1_Stream3);
  DMA_Cmd(DMA1_Stream3, DISABLE);
  DMA_DeInit(DMA1_Stream3);
  while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE)
      {}
  DMA_ClearFlag(DMA1_Stream3, 0xFFFFFFFF);
  
  DMA_InitStructure.DMA_Channel = DMA_Channel_3;
  DMA_InitStructure.DMA_PeripheralBaseAddr =(unsigned int)&(I2S2ext->DR);
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&pDMAbuf_IN[0];
  DMA_InitStructure.DMA_BufferSize = (uint32_t)sizeDMAbuf;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  //DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  //DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
 
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream3, &DMA_InitStructure);
  DMA_Cmd(DMA1_Stream3, ENABLE);

  

  DMA_ITConfig(DMA1_Stream4, DMA_IT_TC|DMA_IT_HT, ENABLE);
  NVIC_InitTypeDef   NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TXDMACodac_Pre;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = TXDMACodac_Sub;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  
  SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);

  I2S2ext->CR2|=SPI_CR2_RXDMAEN;//
  I2S2ext->I2SCFGR|=SPI_I2SCFGR_I2SE;//включил SPI


}
//-----------------------------------------------------------------------------
void DMA1_Stream4_IRQHandler( void ){

  if (DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_TCIF4) != RESET)
  {
    DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);


    need_next = 1;
  }


  if (DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_HTIF4) != RESET)
  {

    DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_HTIF4);


    need_next = 2;
  }
     
}
//-----------------------------------------------------------------------------









//-----------------------------------------------------------------------------

void CS_RX_Init(  unsigned char* pDMAbuf_IN, unsigned char* pDMAbuf_OUT, int sizeDMAbuf ){
  
  I2S_InitTypeDef I2S_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3|RCC_APB1Periph_SPI2, ENABLE);

  SPI_I2S_DeInit(SPI3);
  I2S_InitStructure.I2S_AudioFreq = SdrStr.SdrSetup.Sample_Rate;
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_32b;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
  I2S_Init(SPI3, &I2S_InitStructure);
  I2S_FullDuplexConfig(I2S3ext, &I2S_InitStructure);
  
  I2S_Cmd(SPI3, ENABLE);

  //spi_wr ( ADC_CONTROL,     0x10,  GPIO_Pin_10 );

  
  RX_DMAInit( pDMAbuf_IN, pDMAbuf_OUT, sizeDMAbuf);
}
//-----------------------------------------------------------------------------
void CS_RX_DeInit( void ){
  
  //spi_wr ( ADC_CONTROL,     0x1c,  GPIO_Pin_10 );
  
  DMA_Cmd(DMA1_Stream2, DISABLE);
  DMA_Cmd(DMA1_Stream7, DISABLE);

  DMA_DeInit(DMA1_Stream2);
  DMA_DeInit(DMA1_Stream7);
    
  while (DMA_GetCmdStatus(DMA1_Stream2) != DISABLE)
      {}
  
  while (DMA_GetCmdStatus(DMA1_Stream7) != DISABLE)
      {}

  SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Tx, DISABLE);

  SPI_I2S_DeInit(SPI3);
  I2S3ext->CR2=0;//
  I2S3ext->I2SCFGR=0;//включил SPI


}
//-----------------------------------------------------------------------------
void CS_TX_Init(  unsigned char* pDMAbuf_IN, unsigned char* pDMAbuf_OUT, int sizeDMAbuf ){

  I2S_InitTypeDef I2S_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3|RCC_APB1Periph_SPI2, ENABLE);
  SPI_I2S_DeInit(SPI2);
  I2S_InitStructure.I2S_AudioFreq = SdrStr.SdrSetup.Sample_Rate;
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_32b;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
  I2S_Init(SPI2, &I2S_InitStructure);
  I2S_FullDuplexConfig(I2S2ext, &I2S_InitStructure);
  
  I2S_Cmd(SPI2, ENABLE);
  
  TX_DMAInit( pDMAbuf_IN, pDMAbuf_OUT, sizeDMAbuf);
}
//-----------------------------------------------------------------------------
void CS_TX_DeInit( void ){

  
  DMA_Cmd(DMA1_Stream4, DISABLE);
  DMA_Cmd(DMA1_Stream3, DISABLE);

  DMA_DeInit(DMA1_Stream4);
  DMA_DeInit(DMA1_Stream3);
    
  while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE)
      {}
  
  while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE)
      {}

  SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, DISABLE);

  SPI_I2S_DeInit(SPI2);
  I2S2ext->CR2=0;//
  I2S2ext->I2SCFGR=0;//включил SPI
}
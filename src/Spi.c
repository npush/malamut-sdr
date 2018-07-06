#include "main.h"

//-----------------------------------------------------------------------------

void Spi_init(void){

  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd( CODEC_CS1_CLK|CODEC_CS2_CLK ,ENABLE);
  /* Audio AD0(i2c) pin configuration*/
  GPIO_InitStructure.GPIO_Pin = CODEC_CS1_PIN|CODEC_CS2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(CODEC_CS2_PORT, &GPIO_InitStructure);
  GPIO_WriteBit(CODEC_CS2_PORT, CODEC_CS1_PIN|CODEC_CS2_PIN, Bit_SET);
  
  
      /* Peripheral Clock Enable -------------------------------------------------*/
  /* Enable the SPI clock */
  SPIx_CLK_INIT(SPIx_CLK, ENABLE);
  
  /* Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK, ENABLE);

  /* SPI GPIO Configuration --------------------------------------------------*/
  /* Connect SPI pins to AF5 */  
  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);
    /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPIx);
  SPI_InitTypeDef  SPI_InitStructure;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;

  
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPIx, &SPI_InitStructure);
  SPI_Cmd(SPIx, ENABLE);
}
//-----------------------------------------------------------------------------

void Spi_wr( unsigned char data1,  unsigned char data2 , int cs_pin ){

  GPIO_WriteBit(CODEC_CS2_PORT, cs_pin, Bit_RESET);
      
SPI1->DR=0x20;
while ((SPI1->SR&SPI_SR_TXE)!=SPI_SR_TXE);
while ((SPI1->SR&SPI_SR_BSY)!=0); 
    
SPI1->DR=data1;
while ((SPI1->SR&SPI_SR_TXE)!=SPI_SR_TXE);
while ((SPI1->SR&SPI_SR_BSY)!=0); 
    
SPI1->DR=data2;
while ((SPI1->SR&SPI_SR_TXE)!=SPI_SR_TXE);
while ((SPI1->SR&SPI_SR_BSY)!=0); 

  unsigned long nCount = 0xFF;
  
for (; nCount != 0; nCount--)
    asm("nop");
 
GPIO_WriteBit(CODEC_CS2_PORT, cs_pin, Bit_SET);
nCount = 0xFF;
for (; nCount != 0; nCount--)
    asm("nop");

}
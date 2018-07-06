#include "main.h"
//-----------------------------------------------------------------------------
void Gpio_init( void ){
  
  GPIO_InitTypeDef      GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd( MIX_RX_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin = MIX_RX_PIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(MIX_RX_PORT, &GPIO_InitStructure);

  RCC_AHB1PeriphClockCmd( MIX_TX_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin = MIX_TX_PIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(MIX_TX_PORT, &GPIO_InitStructure);


  MIX_RX_ON();
  MIX_TX_OFF();


  RCC_AHB1PeriphClockCmd( AF_MUTE_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Pin = AF_MUTE_PIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(AF_MUTE_PORT, &GPIO_InitStructure);
  
  AF_MUTE_ON(); 
  AF_MUTE_OFF();

  
  
  RCC_AHB1PeriphClockCmd(TX_EN_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin = TX_EN_PIN;
  GPIO_Init(TX_EN_PORT, &GPIO_InitStructure);
  TX_OFF();
  
  
  RCC_AHB1PeriphClockCmd(LedCpu_CLK, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin = LedCpu_PIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
 
  GPIO_Init(LedCpu_PORT, &GPIO_InitStructure);
  GPIO_WriteBit(LedCpu_PORT, LedCpu_PIN, Bit_SET  );

  
}
//-----------------------------------------------------------------------------

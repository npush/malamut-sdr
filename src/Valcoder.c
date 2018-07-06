#include "main.h"

//------------------------------------------------------------------------------
void Valcoder_init( void ){
 
  GPIO_InitTypeDef GPIO_InitStruct;
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  RCC_AHB1PeriphClockCmd(Valcoder_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_Pin = Valcoder_PINA| Valcoder_PINB;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(Valcoder_PORT, &GPIO_InitStruct);

  
  /* Tell system that you will use PB12 for EXTI_Line12 */
  SYSCFG_EXTILineConfig(Valcoder_EXTI, Valcoder_EXTI_PinSource);
 
  EXTI_InitStruct.EXTI_Line = Valcoder_EXTI_Line;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
#ifdef VALCODE_X2
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//EXTI_Trigger_Rising;//EXTI_Trigger_Falling
#else  
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//EXTI_Trigger_Rising;//EXTI_Trigger_Falling
#endif
  EXTI_Init(&EXTI_InitStruct);

  NVIC_InitStruct.NVIC_IRQChannel = Valcoder_IRQChannel;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = VALCODER_Pre;//VALCODER
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = VALCODER_Sub;//VALCODER
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
}
//------------------------------------------------------------------------------
#ifdef pcbASDR_V1
void EXTI3_IRQHandler(void) {
#else
void EXTI15_10_IRQHandler(void) {
#endif
//SdrMode.ValCount;
  int k;
  if (EXTI_GetITStatus(Valcoder_EXTI_Line) != RESET) {

    if( SdrStr.ValMode ){
      if (GPIO_ReadInputDataBit(Valcoder_PORT,Valcoder_PINA) == Bit_SET)
        SdrStr.ValCount++;
      else
        SdrStr.ValCount--;
    }
    else
    {
      if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] < AM)
        k=10;
      else
        k=100;
      
      if( KEY_Current[ KeyB ] == 1 )
        k=10000;
      
      if (GPIO_ReadInputDataBit(Valcoder_PORT,Valcoder_PINA) == Bit_SET)
        SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ]+=k;
      else
        SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ]-=k;

      
      DDS_freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] * 2 );
      SaveFreq = 1;
    }
    
    EXTI_ClearITPendingBit(Valcoder_EXTI_Line);
  }
}
//------------------------------------------------------------------------------
void CheckBand( void ){
}
//------------------------------------------------------------------------------

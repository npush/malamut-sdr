#ifndef __VALCODER_H
#define __VALCODER_H

#ifdef pcbASDR_V1
  #define Valcoder_CLK				RCC_AHB1Periph_GPIOE
  #define Valcoder_PORT				GPIOE
  #define Valcoder_PINA				GPIO_Pin_2
  #define Valcoder_PINB				GPIO_Pin_3

  #define Valcoder_EXTI                         EXTI_PortSourceGPIOE
  #define Valcoder_EXTI_PinSource               EXTI_PinSource3
  #define Valcoder_EXTI_Line                    EXTI_Line3
  #define Valcoder_IRQChannel                   EXTI3_IRQn
#else
  #define Valcoder_CLK				RCC_AHB1Periph_GPIOA
  #define Valcoder_PORT				GPIOA
  #define Valcoder_PINA				GPIO_Pin_11
  #define Valcoder_PINB				GPIO_Pin_12

  #define Valcoder_EXTI                         EXTI_PortSourceGPIOA
  #define Valcoder_EXTI_PinSource               EXTI_PinSource12
  #define Valcoder_EXTI_Line                    EXTI_Line12
  #define Valcoder_IRQChannel                   EXTI15_10_IRQn
#endif
void Valcoder_init( void );
void CheckBand( void );
#endif

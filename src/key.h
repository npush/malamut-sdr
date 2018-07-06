#ifndef __KEY_H_
#define __KEY_H_

#define TotalKey        7

#ifdef pcbASDR_V1
  #define KEY_A_CLK       RCC_AHB1Periph_GPIOE
  #define KEY_A_PORT      GPIOE
  #define KEY_A_PIN       GPIO_Pin_15

  #define KEY_B_CLK       RCC_AHB1Periph_GPIOE
  #define KEY_B_PORT      GPIOE
  #define KEY_B_PIN       GPIO_Pin_14

  #define KEY_C_CLK       RCC_AHB1Periph_GPIOE
  #define KEY_C_PORT      GPIOE
  #define KEY_C_PIN       GPIO_Pin_13

  #define KEY_D_CLK       RCC_AHB1Periph_GPIOE
  #define KEY_D_PORT      GPIOE
  #define KEY_D_PIN       GPIO_Pin_12

  #define KEY_E_CLK       RCC_AHB1Periph_GPIOE
  #define KEY_E_PORT      GPIOE
  #define KEY_E_PIN       GPIO_Pin_11


  #define KEY_BP_CLK       RCC_AHB1Periph_GPIOE
  #define KEY_BP_PORT      GPIOE
  #define KEY_BP_PIN       GPIO_Pin_10

  #define KEY_BM_CLK       RCC_AHB1Periph_GPIOE
  #define KEY_BM_PORT      GPIOE
  #define KEY_BM_PIN       GPIO_Pin_9

  #define PTT_CLK       RCC_AHB1Periph_GPIOB
  #define PTT_PORT      GPIOB
  #define PTT_PIN       GPIO_Pin_11
#else
  #define KEY_A_CLK       RCC_AHB1Periph_GPIOA
  #define KEY_A_PORT      GPIOA
  #define KEY_A_PIN       GPIO_Pin_10

  #define KEY_B_CLK       RCC_AHB1Periph_GPIOC
  #define KEY_B_PORT      GPIOC
  #define KEY_B_PIN       GPIO_Pin_8

  #define KEY_C_CLK       RCC_AHB1Periph_GPIOA
  #define KEY_C_PORT      GPIOA
  #define KEY_C_PIN       GPIO_Pin_8

  #define KEY_D_CLK       RCC_AHB1Periph_GPIOC
  #define KEY_D_PORT      GPIOC
  #define KEY_D_PIN       GPIO_Pin_9

  #define KEY_E_CLK       RCC_AHB1Periph_GPIOA
  #define KEY_E_PORT      GPIOA
  #define KEY_E_PIN       GPIO_Pin_9


  #define KEY_BP_CLK       RCC_AHB1Periph_GPIOD
  #define KEY_BP_PORT      GPIOD
  #define KEY_BP_PIN       GPIO_Pin_15

  #define KEY_BM_CLK       RCC_AHB1Periph_GPIOD
  #define KEY_BM_PORT      GPIOD
  #define KEY_BM_PIN       GPIO_Pin_14

  #define PTT_CLK       RCC_AHB1Periph_GPIOE
  #define PTT_PORT      GPIOE
  #define PTT_PIN       GPIO_Pin_13
#endif

enum{ SEL, KeyA, KeyB, KeyC, KeyD, BANDP, BANDM };
void Key_init( void );
void InitTime3Int1ms( void );

#endif

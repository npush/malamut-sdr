#ifndef __BPF_H_
#define __BPF_H_

#ifdef pcbASDR_V1
  #define BPF_PORT_CLK    RCC_AHB1Periph_GPIOD
  #define BPF_PORT        GPIOD
  #define BPF1_PIN        GPIO_Pin_7
  #define BPF2_PIN        GPIO_Pin_6
  #define BPF3_PIN        GPIO_Pin_5
  #define BPF4_PIN        GPIO_Pin_4
  #define BPF5_PIN        GPIO_Pin_3
  #define BPF6_PIN        GPIO_Pin_2
  #define BPF7_PIN        GPIO_Pin_1
  #define BPF8_PIN        GPIO_Pin_0

  #define PRE_CLK         RCC_AHB1Periph_GPIOA
  #define PRE_PORT        GPIOA
  #define PRE_PIN         GPIO_Pin_10
  #define PRE_ON()        GPIO_SetBits(PRE_PORT, PRE_PIN)       //GPIO_WriteBit(PRE_PORT, PRE_PIN, Bit_SET)
  #define PRE_OFF()       GPIO_ResetBits(PRE_PORT, PRE_PIN)     //GPIO_WriteBit(PRE_PORT, PRE_PIN, Bit_RESET)

  #define ATT_CLK         RCC_AHB1Periph_GPIOA
  #define ATT_PORT        GPIOA
  #define ATT1_PIN        GPIO_Pin_9
  #define ATT2_PIN        GPIO_Pin_8
  #define ATT8_ON()       {GPIO_WriteBit(ATT_PORT, ATT1_PIN, Bit_SET );GPIO_WriteBit(ATT_PORT, ATT2_PIN, Bit_RESET );}
  #define ATT16_ON()      {GPIO_WriteBit(ATT_PORT, ATT2_PIN, Bit_SET );GPIO_WriteBit(ATT_PORT, ATT1_PIN, Bit_RESET );}
  #define ATT24_ON()      GPIO_WriteBit(ATT_PORT, ATT1_PIN|ATT2_PIN, Bit_SET  )
  #define ATT_OFF()       GPIO_WriteBit(ATT_PORT, ATT1_PIN|ATT2_PIN, Bit_RESET  )

#else

  #define BPF_PORT_CLK    RCC_AHB1Periph_GPIOA 
  #define BPF_PORT        GPIOA      
  #define BPF_DATA        GPIO_Pin_2
  #define BPF_CLK         GPIO_Pin_0
  #define BPF_L           GPIO_Pin_1
    
  #define PRE_ON()       asm("nop")
  #define PRE_OFF()      asm("nop")

  #define ATT8_ON()       
  #define ATT16_ON()      
  #define ATT24_ON()      
  #define ATT_OFF()       
#endif

void BPF_init( void );
void BPF_set( void );

void PreSet(void);
void AttSet(void);
#endif

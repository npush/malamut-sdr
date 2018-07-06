#ifndef __GPIO_H_
#define __GPIO_H_

#ifdef pcbASDR_V1
  #define MIX_RX_CLK       RCC_AHB1Periph_GPIOC
  #define MIX_RX_PORT      GPIOC
  #define MIX_RX_PIN       GPIO_Pin_9
  #define MIX_RX_ON()      GPIO_WriteBit(MIX_RX_PORT, MIX_RX_PIN, Bit_RESET  )
  #define MIX_RX_OFF()     GPIO_WriteBit(MIX_RX_PORT, MIX_RX_PIN, Bit_SET  )

  #define MIX_TX_CLK       RCC_AHB1Periph_GPIOC
  #define MIX_TX_PORT      GPIOC
  #define MIX_TX_PIN       GPIO_Pin_8
  #define MIX_TX_ON()      GPIO_WriteBit(MIX_TX_PORT, MIX_TX_PIN, Bit_RESET  )
  #define MIX_TX_OFF()     GPIO_WriteBit(MIX_TX_PORT, MIX_TX_PIN, Bit_SET  )

  #define TX_EN_CLK       RCC_AHB1Periph_GPIOD
  #define TX_EN_PORT      GPIOD
  #define TX_EN_PIN       GPIO_Pin_10

  #define LedCpu_CLK       RCC_AHB1Periph_GPIOD
  #define LedCpu_PORT      GPIOD
  #define LedCpu_PIN       GPIO_Pin_8

#else
  #define MIX_RX_CLK       RCC_AHB1Periph_GPIOE
  #define MIX_RX_PORT      GPIOE
  #define MIX_RX_PIN       GPIO_Pin_15
  #define MIX_RX_ON()      GPIO_WriteBit(MIX_RX_PORT, MIX_RX_PIN, Bit_RESET  )
  #define MIX_RX_OFF()     GPIO_WriteBit(MIX_RX_PORT, MIX_RX_PIN, Bit_SET  )

  #define MIX_TX_CLK       RCC_AHB1Periph_GPIOE
  #define MIX_TX_PORT      GPIOE
  #define MIX_TX_PIN       GPIO_Pin_14
  #define MIX_TX_ON()      GPIO_WriteBit(MIX_TX_PORT, MIX_TX_PIN, Bit_RESET  )
  #define MIX_TX_OFF()     GPIO_WriteBit(MIX_TX_PORT, MIX_TX_PIN, Bit_SET  )

  #define TX_EN_CLK       RCC_AHB1Periph_GPIOC
  #define TX_EN_PORT      GPIOC
  #define TX_EN_PIN       GPIO_Pin_3

#endif

#define AF_MUTE_CLK     RCC_AHB1Periph_GPIOE
#define AF_MUTE_PORT    GPIOE
#define AF_MUTE_PIN     GPIO_Pin_1
#define AF_MUTE_ON()    GPIO_WriteBit(AF_MUTE_PORT, AF_MUTE_PIN, Bit_SET  )
#define AF_MUTE_OFF()   GPIO_WriteBit(AF_MUTE_PORT, AF_MUTE_PIN, Bit_RESET  )

#define TX_ON()         GPIO_WriteBit(TX_EN_PORT, TX_EN_PIN, Bit_SET  );
#define TX_OFF()        GPIO_WriteBit(TX_EN_PORT, TX_EN_PIN, Bit_RESET  );


void Gpio_init( void );

#endif
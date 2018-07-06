#ifndef __CS4272DIG_H
#define __CS4272DIG_H

#include "pcb.h"
#ifdef  pcbASDR_V1
  #define CODEC_CDRST_CLK     RCC_AHB1Periph_GPIOC
  #define CODEC_CDRST_PORT    GPIOC
  #define CODEC_CDRST_PIN     GPIO_Pin_1

  #define CODEC_CS1_CLK       RCC_AHB1Periph_GPIOC
  #define CODEC_CS1_PORT      GPIOC
  #define CODEC_CS1_PIN       GPIO_Pin_13
  #define CODEC_CS_TX        GPIO_Pin_13

  #define CODEC_CS2_CLK       RCC_AHB1Periph_GPIOC
  #define CODEC_CS2_PORT      GPIOC
  #define CODEC_CS2_PIN       GPIO_Pin_0
  #define CODEC_CS_RX        GPIO_Pin_0
#else
  #define CODEC_CDRST_CLK     RCC_AHB1Periph_GPIOE
  #define CODEC_CDRST_PORT    GPIOE
  #define CODEC_CDRST_PIN     GPIO_Pin_9

  #define CODEC_CS1_CLK       RCC_AHB1Periph_GPIOE
  #define CODEC_CS1_PORT      GPIOE
  #define CODEC_CS1_PIN       GPIO_Pin_10

  #define CODEC_CS2_CLK       RCC_AHB1Periph_GPIOE
  #define CODEC_CS2_PORT      GPIOE
  #define CODEC_CS2_PIN       GPIO_Pin_11
#endif

//memory address pointers
#define MODE_CONTROL_1  0x1 //memory address pointer for modeControl1
#define DAC_CONTROL     0x2 //memory address pointer for DACControl
#define DAC_VOLUME      0x3 //memory address pointer for DACVolume and Mixing
#define DAC_VOLUME_A    0x4 //memory address pointer for DACVolumeA
#define DAC_VOLUME_B    0x5 //memory address pointer for DACVolumeB
#define ADC_CONTROL     0x6 //memory address pointer for ADC Control
#define MODE_CONTROL_2  0x7 //memory address pointer for ModeControlTwo

void CS4272dig_init( void );
void CodecTXPwrSet( void );

#endif

#include "main.h"


uint32_t KEY_CLK[] ={ KEY_A_CLK, KEY_B_CLK, KEY_C_CLK, KEY_D_CLK, KEY_E_CLK, KEY_BP_CLK, KEY_BM_CLK };
GPIO_TypeDef* KEY_PORT[] ={ KEY_A_PORT, KEY_B_PORT, KEY_C_PORT, KEY_D_PORT, KEY_E_PORT, KEY_BP_PORT, KEY_BM_PORT };
uint16_t KEY_PIN[] ={ KEY_A_PIN, KEY_B_PIN, KEY_C_PIN, KEY_D_PIN, KEY_E_PIN, KEY_BP_PIN, KEY_BM_PIN };

__IO uint8_t KEY_Done[ TotalKey ];
__IO uint8_t KEY_Current[ TotalKey ];
//-----------------------------------------------------------------------------
void Key_init( void ){
  
  GPIO_InitTypeDef      GPIO_InitStructure;
  for( int i=0; i<TotalKey;++i ){
    RCC_AHB1PeriphClockCmd( KEY_CLK[ i ], ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin = KEY_PIN[ i ];
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(KEY_PORT[ i ], &GPIO_InitStructure);
    GPIO_WriteBit(KEY_PORT[ i ], KEY_PIN[ i ], Bit_SET  );
  }


  RCC_AHB1PeriphClockCmd( PTT_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin = PTT_PIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(PTT_PORT, &GPIO_InitStructure);
  GPIO_WriteBit(PTT_PORT, PTT_PIN, Bit_SET  );
  
  InitTime3Int1ms();
}
//-----------------------------------------------------------------------------
void InitTime3Int1ms( void )
{
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

  /* Enable the TIM2 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Timer1ms_Key_Pre;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = Timer1ms_Key_Sub;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
   
  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 1 MHz down to 1 KHz (1 ms)
  //TIM_TimeBaseStructure.TIM_Prescaler = 108/*84*/ - 1; // 24 MHz Clock down to 1 MHz (adjust per your clock)  84-168 108-216
  TIM_TimeBaseStructure.TIM_Prescaler = PLL_N/4;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  /* TIM IT enable */
  
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  /* TIM2 enable counter */
  TIM_Cmd(TIM3, ENABLE);
}
//-----------------------------------------------------------------------------
unsigned char ptt_pre=0;
void TIM3_IRQHandler(void){
  
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    
    if( GPIO_ReadInputDataBit(PTT_PORT, PTT_PIN) == Bit_RESET){
      if( ptt_pre == 0 ){
        WaterfallON = 0;
        SdrStr.TX_Tone = 0;
        SdrStr.SdrMode.RTX_Mode = 1;
        ptt_pre = 1;
      }
    }else{
      if( ptt_pre == 1 ){
        if(MenuSetup ==0)//no inside menusetup
          WaterfallON = 1;
        SdrStr.TX_Tone = 0;
        SdrStr.SdrMode.RTX_Mode = 0;
        ptt_pre = 0;
      }
    }
    
    
    for( int i=0; i<TotalKey;++i ){
      if( GPIO_ReadInputDataBit(KEY_PORT[ i ], KEY_PIN[ i ]) == Bit_RESET){
          KEY_Current[ i ] = 1;
      
      }else{
          KEY_Current[ i ] = 0;
          KEY_Done[ i ] = 0;
      }
    }
    

  time_tic++;
  
 /* if( GPIO_ReadInputDataBit(LedRx_PORT, LedRx_PIN  ) == Bit_RESET )
    GPIO_WriteBit(LedRx_PORT,LedRx_PIN,Bit_SET);
  else
    GPIO_WriteBit(LedRx_PORT,LedRx_PIN,Bit_RESET);*/
  
    
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
}
//-----------------------------------------------------------------------------
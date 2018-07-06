#include "main.h"
//------------------------------------------------------------------------------


__IO unsigned char bpf_temp;

void BPF_init( void ){
  bpf_temp = 100;
 
  //======================= BPF ==========================
  GPIO_InitTypeDef      GPIO_InitStruct;
  RCC_AHB1PeriphClockCmd(BPF_PORT_CLK, ENABLE);
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
#ifdef pcbASDR_V1
  GPIO_InitStruct.GPIO_Pin = BPF8_PIN|BPF7_PIN|BPF6_PIN|BPF5_PIN|BPF4_PIN|BPF3_PIN|BPF2_PIN|BPF1_PIN;
  GPIO_Init(BPF_PORT, &GPIO_InitStruct);
  GPIO_WriteBit(BPF_PORT, BPF8_PIN|BPF7_PIN|BPF6_PIN|BPF5_PIN|BPF4_PIN|BPF3_PIN|BPF2_PIN|BPF1_PIN, Bit_RESET  );
#else
  GPIO_InitStruct.GPIO_Pin = BPF_DATA|BPF_CLK|BPF_L;
  GPIO_Init(BPF_PORT, &GPIO_InitStruct);
  GPIO_WriteBit(BPF_PORT, BPF_DATA|BPF_CLK|BPF_L, Bit_RESET  );
#endif

  //======================================================
  
  BPF_set();
  
  #ifdef pcbASDR_V1
  RCC_AHB1PeriphClockCmd(PRE_CLK, ENABLE);
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Pin = PRE_PIN;
  GPIO_Init(PRE_PORT, &GPIO_InitStruct);
  PRE_OFF();
  
  
    
  RCC_AHB1PeriphClockCmd(ATT_CLK, ENABLE);
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Pin = ATT1_PIN|ATT2_PIN;
  GPIO_Init(ATT_PORT, &GPIO_InitStruct);
  ATT_OFF();  
  #endif
}
//------------------------------------------------------------------------------
void BPF_set( void ){
  
  unsigned char bpf=100;
  if( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] < 2000000 )//LPF-2.7 BPF-0.0-2.0
    bpf = 0;
  else
    if( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] < 3800000 )//LPF-5.0 BPF-2.0-3.9
      bpf = 1;
    else
      if( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] < 7200000 )//LPF-7.7 BPF-3.9-7.5
        bpf = 2;
      else
        if( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] < 10300000 )//LPF-14.5 BPF-6.2-10.4  
          bpf = 3;
        else
          if( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] < 15000000 )//LPF-14.5 BPF-9.5-15
            bpf = 4;
          else
            if( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] < 20800000 )//LPF-28.8 BPF-14-22
              bpf = 5;
            else
              bpf = 6;                                   //LPF-28.8  BPF-20-33
              
             /* if( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] < 33000000 )//LPF-28.8  BPF-20-33
                bpf = 6;
              else{
                bpf = 6;
                SdrStr.SdrMode.BYPS = 1;
              }*/

           
  if( bpf != bpf_temp ){
#ifdef pcbASDR_V1
  GPIO_WriteBit(BPF_PORT, BPF8_PIN|BPF7_PIN|BPF6_PIN|BPF5_PIN|BPF4_PIN|BPF3_PIN|BPF2_PIN|BPF1_PIN, Bit_RESET  );
  if( SdrStr.SdrMode.BYPS )
    GPIO_WriteBit(BPF_PORT, BPF8_PIN, Bit_SET  );
  else  
    GPIO_WriteBit(BPF_PORT, BPF1_PIN>>bpf, Bit_SET  );

#else
    for( int i=0; i<8;++i ){
      if( (7-i)== bpf)
        GPIO_SetBits( BPF_PORT, BPF_DATA );
      else
        GPIO_ResetBits( BPF_PORT, BPF_DATA );
      GPIO_SetBits( BPF_PORT, BPF_CLK );
      GPIO_ResetBits( BPF_PORT, BPF_CLK );
    }
    GPIO_SetBits( BPF_PORT, BPF_L );
    GPIO_ResetBits( BPF_PORT, BPF_L );
   bpf_temp = bpf;
#endif   
  }
  
}
//------------------------------------------------------------------------------
void PreSet(void){
  if((SdrStr.SdrMode.PRE[SdrStr.SdrMode.Band])==1){
    PRE_OFF();
    PRE_ON();
  }
  else{
    PRE_OFF();
  }
}
//------------------------------------------------------------------------------
void AttSet(void){
  if( SdrStr.SdrMode.ATT[ SdrStr.SdrMode.Band ] == 0)
    ATT_OFF();
  if( SdrStr.SdrMode.ATT[ SdrStr.SdrMode.Band ] == 1)
    ATT8_ON();
  if( SdrStr.SdrMode.ATT[ SdrStr.SdrMode.Band ] == 2)
    ATT16_ON();
  if( SdrStr.SdrMode.ATT[ SdrStr.SdrMode.Band ] == 3)
    ATT24_ON();
}
//------------------------------------------------------------------------------

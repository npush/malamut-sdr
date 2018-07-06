#include "main.h"

unsigned long adc_REV=0, adc_FWD=0, adc_REVt=0, adc_FWDt=0, adc_VLT=0, adc_VLTt=0, adc_CNT=0,adc1_CNT=0;
unsigned long  PTT_SW1=0, PTT_SW1t=0, PTT_SW2=0, PTT_SW2t=0;
float adc_delta, adc_delta_swr;
COMPLEX_short adc_buf[100];
//-----------------------------------------------------------------------------
void ADC_init( void ){
  
  adc_delta = 3.130/4096.0*11.0;
  adc_delta_swr = 3.130/4096.0;
  
  
  adc_REV = 0;
  adc_FWD = 0;
  adc_VLT = 0;
   
  GPIO_InitTypeDef      GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStruct;
  
  memset( (unsigned char*)&adc_buf, 0, sizeof( adc_buf ) );
  
  #ifdef  pcbASDR_V1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
  #else
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
  #endif
  
  NVIC_InitStruct.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = ADC_Pre;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = ADC_Sub;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
 

   // RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2, ENABLE); // Воспользуемся одним общим для всех тактированием АЦП
  //RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC1EN, ENABLE);  // Это цифровое тактирование с возможностю выбора своего тактирования для каждого АЦП
                                                         // мы такой режим тактирования использовать не будем

  ADC_InitTypeDef       ADC_InitStructure;
  ADC_StructInit(&ADC_InitStructure); //Сбрасывает настройки перед новой инициализации структуры
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_CommonStructInit(&ADC_CommonInitStructure);
  ADC_DeInit();// сбрасываем настройки всех! АЦП

  //Настройка AЦП (ADC1)
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; //Разрешение данных АЦП 12 бит (можно не писать, по умолчанию всегда 12 бит)
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;          //Не используем режим сканирования регулярных каналов (сторчку можно не писать)
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;     //Разрешаем непрерывную конверсию данных
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //Не используем внешнее прерывание по переднему/заднему сигналу
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //Сдвиг данных вправо (строчку можно опустить, по умолчанию и так выравнивание в право)
  ADC_InitStructure.ADC_NbrOfConversion = 0;
  //Дополнительные настройки АЦП
  ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult_InjecSimult;
//  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2; //Пределитель частоты работы АЦП от частоты МК (по умолчанию Div2, либо можно её не писать)
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8; //Пределитель частоты работы АЦП от частоты МК (по умолчанию Div2, либо можно её не писать)
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //Не используем DMA в мультире режиме АЦП
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //Задержка при комбинированном использвании АЦП (эту строчку можно не писать)

  //*******************************************************
  ADC_Init(ADC2,&ADC_InitStructure);
  ADC_CommonInit(&ADC_CommonInitStructure);

  ADC_InjectedSequencerLengthConfig( ADC2, 3 );
  #ifdef  pcbASDR_V1
    ADC_InjectedChannelConfig(ADC2, ADC_Channel_0, 3, ADC_SampleTime_480Cycles);
    ADC_InjectedChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_480Cycles);
    ADC_InjectedChannelConfig(ADC2, ADC_Channel_2, 2, ADC_SampleTime_480Cycles);
  #else  
    ADC_InjectedChannelConfig(ADC2, ADC_Channel_11, 1, ADC_SampleTime_480Cycles);
    ADC_InjectedChannelConfig(ADC2, ADC_Channel_10, 2, ADC_SampleTime_480Cycles);
    ADC_InjectedChannelConfig(ADC2, ADC_Channel_3,  3, ADC_SampleTime_480Cycles);
  #endif  
  ADC_ExternalTrigInjectedConvConfig( ADC2,ADC_ExternalTrigInjecConv_Ext_IT15);
 
  //Запускам АЦП1
  ADC_ITConfig(ADC2, ADC_IT_JEOC, ENABLE);
  ADC_Cmd(ADC2,ENABLE);

  //Запускаем преобразование  инжектированного канала
  ADC_AutoInjectedConvCmd( ADC2, ENABLE );
  ADC_SoftwareStartInjectedConv(ADC2);

  ADC_SoftwareStartConv(ADC2);  
 //*******************************************************
  
 //*******************************************************
  ADC_Init(ADC1,&ADC_InitStructure);
  ADC_CommonInit(&ADC_CommonInitStructure);
  ADC_InjectedSequencerLengthConfig( ADC1, 2 );
  ADC_InjectedChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_480Cycles);
  ADC_InjectedChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_480Cycles);
  ADC_ExternalTrigInjectedConvConfig( ADC1,ADC_ExternalTrigInjecConv_Ext_IT15);
 
  //Запускам АЦП1
  ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);
  ADC_Cmd(ADC1,ENABLE);

  //Запускаем преобразование  инжектированного канала
  ADC_AutoInjectedConvCmd( ADC1, ENABLE );
  ADC_SoftwareStartInjectedConv(ADC1);

  ADC_SoftwareStartConv(ADC1);  
 //*******************************************************

}
//-----------------------------------------------------------------------------
#define ADC1_NAK 250
#define ADC_NAK 40
void ADC_IRQHandler(void){
 //ADC_SampleTime_144Cycles 100cnt          16.7ms 
/*
  ADC_ClearITPendingBit(ADC2, ADC_IT_JEOC);
  adc_REVt += ADC_GetInjectedConversionValue( ADC2, ADC_InjectedChannel_1 );
  adc_FWDt += ADC_GetInjectedConversionValue( ADC2, ADC_InjectedChannel_2 );
  adc_VLTt += ADC_GetInjectedConversionValue( ADC2, ADC_InjectedChannel_3 );
  adc_CNT++;
  if( adc_CNT >= ADC_NAK ){
    adc_REV = adc_REVt/ADC_NAK;
    adc_FWD = adc_FWDt/ADC_NAK;
    adc_VLT = adc_VLTt/ADC_NAK;
    adc_REVt = 0;
    adc_FWDt = 0;
    adc_VLTt = 0;
    adc_CNT = 0;
  }
  */
  
  if (ADC_GetITStatus(ADC1, ADC_IT_JEOC) != RESET){
    PTT_SW1t += ADC_GetInjectedConversionValue( ADC1, ADC_InjectedChannel_1 );
    PTT_SW2t += ADC_GetInjectedConversionValue( ADC1, ADC_InjectedChannel_2 );
    adc1_CNT++;
    if( adc1_CNT >= ADC1_NAK ){
      PTT_SW1 = PTT_SW1t/ADC1_NAK;
      PTT_SW1t = 0;
      PTT_SW2 = PTT_SW2t/ADC1_NAK;
      PTT_SW2t = 0;
      adc1_CNT = 0;
    }
    ADC_ClearITPendingBit(ADC1, ADC_IT_JEOC);
  }
  
  if (ADC_GetITStatus(ADC2, ADC_IT_JEOC) != RESET){ 
    if(SdrStr.SdrMode.RTX_Mode==1){
      adc_buf[adc_CNT].re = ADC_GetInjectedConversionValue( ADC2, ADC_InjectedChannel_1 );
      adc_buf[adc_CNT].im = ADC_GetInjectedConversionValue( ADC2, ADC_InjectedChannel_2 );
    }
    adc_VLTt += ADC_GetInjectedConversionValue( ADC2, ADC_InjectedChannel_3 );
    
    
    adc_CNT++;
    if( adc_CNT >= ADC_NAK ){
      if(SdrStr.SdrMode.RTX_Mode==1){
        adc_REVt = 0;
        adc_FWDt = 0;
        for(int i=0;i<ADC_NAK; ++i){
          adc_REVt = MAX( adc_REVt, adc_buf[i].re );
          adc_FWDt = MAX( adc_FWDt, adc_buf[i].im );
        }
        adc_REV = (unsigned long)((float)adc_REV*0.1+(float)adc_REVt*0.9);
        adc_FWD = (unsigned long)((float)adc_FWD*0.1+(float)adc_FWDt*0.9);
      }

      adc_VLT = adc_VLTt/ADC_NAK;
      adc_VLTt = 0;
      adc_CNT = 0;
    }
    ADC_ClearITPendingBit(ADC2, ADC_IT_JEOC);
  }
}
//-----------------------------------------------------------------------------
/*                  SW1/SW2
          a    b           a    b
          266/2550         266/1600
          
           a       
 b      1560    2480    3230    3828

140     1       2       3       A

1450    4       5       6       B

2460    7       8       9       C

3230    *       0       #       D

3780    P1      P2      P3      P4

*/
unsigned short ptt_a, ptt_b;
unsigned char ptt_st, ptt_sw, ptt_sw_temp;
unsigned long ptt_time;
void PTT_SW_Scan( void ){
  GPIO_InitTypeDef      GPIO_InitStructure;
  if( (PTT_SW1 > 4000)||(PTT_SW2 > 4000) ){
    ptt_st = 0;
    ptt_sw = PTT_NO;
    ptt_sw_temp = PTT_NO;
    
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin = KEY_B_PIN|KEY_C_PIN|KEY_D_PIN|KEY_E_PIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(KEY_A_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(KEY_A_PORT, GPIO_InitStructure.GPIO_Pin, Bit_SET  );
    
    return;
  }
  
  if( ptt_st == 2 )
    return;
  
  if( ptt_st == 0  ){
    ptt_a = PTT_SW1;
    ptt_b = PTT_SW2;
    
    ptt_time = RTC_Get(); 
    ptt_st = 1;
    return;
  }
  

  if( Interval_RTC_Get( ptt_time ) < 10 )
    return;
 
  if( ((ptt_a-9) > PTT_SW1)||(PTT_SW1 > (ptt_a+9)) ){
    ptt_st = 0;
    return;
  }
  
  if( ((ptt_b-9) > PTT_SW2)||(PTT_SW2 > (ptt_b+9)) ){
    ptt_st = 0;
    return;
  }

  
#define ptt_avg 120
  ptt_a = 287; ptt_b = 1537;
  if( ( ((ptt_a-ptt_avg) < PTT_SW1)&&(PTT_SW1 < (ptt_a+ptt_avg)) ) && ( ((ptt_b-ptt_avg) < PTT_SW2)&&(PTT_SW2 < (ptt_b+ptt_avg)) ))
    ptt_sw = PTT_UP;
  
  ptt_a = 287; ptt_b = 2500;
  if( ( ((ptt_a-ptt_avg) < PTT_SW1)&&(PTT_SW1 < (ptt_a+ptt_avg)) ) && ( ((ptt_b-ptt_avg) < PTT_SW2)&&(PTT_SW2 < (ptt_b+ptt_avg)) ))
    ptt_sw = PTT_DWN;
    
  //-------------------------------------------------------
  ptt_a = 3828; ptt_b = 140;
  if( ( ((ptt_a-ptt_avg) < PTT_SW1)&&(PTT_SW1 < (ptt_a+ptt_avg)) ) && ( ((ptt_b-ptt_avg) < PTT_SW2)&&(PTT_SW2 < (ptt_b+ptt_avg)) )){
    ptt_sw = PTT_A;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Pin = KEY_B_PIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(KEY_A_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(KEY_A_PORT, GPIO_InitStructure.GPIO_Pin, Bit_RESET  );
  }
  ptt_a = 3828; ptt_b = 1450;
  if( ( ((ptt_a-ptt_avg) < PTT_SW1)&&(PTT_SW1 < (ptt_a+ptt_avg)) ) && ( ((ptt_b-ptt_avg) < PTT_SW2)&&(PTT_SW2 < (ptt_b+ptt_avg)) )){
    ptt_sw = PTT_B;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Pin = KEY_C_PIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(KEY_A_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(KEY_A_PORT, GPIO_InitStructure.GPIO_Pin, Bit_RESET  );
  }
  ptt_a = 3828; ptt_b = 2460;
  if( ( ((ptt_a-ptt_avg) < PTT_SW1)&&(PTT_SW1 < (ptt_a+ptt_avg)) ) && ( ((ptt_b-ptt_avg) < PTT_SW2)&&(PTT_SW2 < (ptt_b+ptt_avg)) )){
    ptt_sw = PTT_C;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Pin = KEY_D_PIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(KEY_A_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(KEY_A_PORT, GPIO_InitStructure.GPIO_Pin, Bit_RESET  );
  }
  ptt_a = 3828; ptt_b = 3230;
  if( ( ((ptt_a-ptt_avg) < PTT_SW1)&&(PTT_SW1 < (ptt_a+ptt_avg)) ) && ( ((ptt_b-ptt_avg) < PTT_SW2)&&(PTT_SW2 < (ptt_b+ptt_avg)) )){
    ptt_sw = PTT_D;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Pin = KEY_E_PIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(KEY_A_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(KEY_A_PORT, GPIO_InitStructure.GPIO_Pin, Bit_RESET  );
  }
  //-------------------------------------------------------
  
  
  
  ptt_a = 1560; ptt_b = 3765;
  if( ( ((ptt_a-ptt_avg) < PTT_SW1)&&(PTT_SW1 < (ptt_a+ptt_avg)) ) && ( ((ptt_b-ptt_avg) < PTT_SW2)&&(PTT_SW2 < (ptt_b+ptt_avg)) ))
    ptt_sw = PTT_P1;

  ptt_a = 2485; ptt_b = 3765;
  if( ( ((ptt_a-ptt_avg) < PTT_SW1)&&(PTT_SW1 < (ptt_a+ptt_avg)) ) && ( ((ptt_b-ptt_avg) < PTT_SW2)&&(PTT_SW2 < (ptt_b+ptt_avg)) ))
    ptt_sw = PTT_P2;
  
  if( ptt_sw == PTT_UP ){
    SdrStr.SdrMode.Menu_Sel++;
    if( SdrStr.SdrMode.Menu_Sel == MENUSELEND )
      SdrStr.SdrMode.Menu_Sel = 0;
    MenuChoiseLcd();
    SaveSdrMode = 1;
  }
  if( ptt_sw == PTT_DWN ){
    SdrStr.SdrMode.Menu_Sel--;
    if( SdrStr.SdrMode.Menu_Sel >= MENUSELEND )
      SdrStr.SdrMode.Menu_Sel = MENUSELEND-1;
    MenuChoiseLcd();
    SaveSdrMode = 1;
  }
  
  if( ptt_sw == PTT_P1 ){
    unsigned char*pUchar;
    if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] < AM )
      pUchar = &SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ];
    else
      pUchar = &SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ];
    
    if( (*pUchar) == 0 )
      *pUchar = TotalFilter;
    (*pUchar)--;
    
    
    SaveSdrMode = 1;
    LcdMode();
    LcdFilters();
    if( SdrStr.SdrMode.Menu_Sel == FILTm_FILTp_FLITlo_FILThi )
      LcdFiltersSet();
  }
  if( ptt_sw == PTT_P2 ){
    unsigned char*pUchar;
    if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] < AM )
      pUchar = &SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ];
    else
      pUchar = &SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ];

    (*pUchar)++;
    if( (*pUchar) == TotalFilter )
      *pUchar = 0;
    
    SaveSdrMode = 1;
    LcdMode();
    LcdFilters();
    if( SdrStr.SdrMode.Menu_Sel == FILTm_FILTp_FLITlo_FILThi )
      LcdFiltersSet();
  }
 
  ptt_st = 2;
    
  
  
  
}
//-----------------------------------------------------------------------------

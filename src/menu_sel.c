#include "main.h"

void (*Menu_Sel_Key)() = &MenuSel_1;

//----------------------------------------------------------------------------
void MenuSelScan( void ){
  MenuChoise();
  Menu_Sel_Key();
}
//----------------------------------------------------------------------------

unsigned long SelTime;
void MenuChoise( void ){

  if( KEY_Current[ SEL ] == 0 )
    return;
  if( KEY_Done[ SEL ] == 1 ){
    if( Interval_RTC_Get(SelTime)>2000 )
    {
      if(SdrStr.SdrMode.Menu_Sel == 0)
        SdrStr.SdrMode.Menu_Sel = MENUSELEND-1;
      else
        SdrStr.SdrMode.Menu_Sel--;
      TM_ILI9341_DrawFilledRectangleFast( 0,20, 256, 80, BLACK );
   
      MenuSetupSetDisp();
      MenuSetupDisp();
      
      MenuSetupDrawDwnKey();
      WaterfallON = 0;
      MenuSetup = 1;
      SaveSdrMode = 1;
    }
    return;
  }
  
  SdrStr.SdrMode.Menu_Sel++;
  if( SdrStr.SdrMode.Menu_Sel == MENUSELEND )
    SdrStr.SdrMode.Menu_Sel = 0;
  
  MenuChoiseLcd();
  SaveSdrMode = 1;
 
  KEY_Done[ SEL ] = 1;
  SelTime = RTC_Get();
}
//----------------------------------------------------------------------------
void MenuChoiseLcd( void ){
  //sprintf( text,"                             ");//29
  
  if( SdrStr.SdrMode.Menu_Sel == MODEm_MODEp_BANDm_BANDp ){
    sprintf( text,"MODE-   MODE+    BAND-  BAND+");
    TM_ILI9341_Puts(0, 0, text, &TM_Font_11x18, YELLOW, BLACK);
    Menu_Sel_Key = &MenuSel_1;
  }else
  
    if( SdrStr.SdrMode.Menu_Sel == FILTm_FILTp_FLITlo_FILThi ){
    //sprintf( text,"FILT+   FILT+    SetLo  SetHi");
    //sprintf( text,"FILT+   FILT+    0.25    2.75");
    sprintf( text,"FILT-   FILT+    ");
    TM_ILI9341_Puts(0, 0, text, &TM_Font_11x18, YELLOW, BLACK);
    LcdFiltersSet();
    Menu_Sel_Key = &MenuSel_2;
  }else
  
    if( SdrStr.SdrMode.Menu_Sel == PRE_ATT_RF_BYPS ){
    sprintf( text,"PRE     ATT     RF_OFF   BYPS");
    TM_ILI9341_Puts(0, 0, text, &TM_Font_11x18, YELLOW, BLACK);
    Menu_Sel_Key = &MenuSel_3;
  }else
  
    if( SdrStr.SdrMode.Menu_Sel == ANF_DNR_NB_NBL ){
    sprintf( text,"ANF      DNR      NB      NBL");
    TM_ILI9341_Puts(0, 0, text, &TM_Font_11x18, YELLOW, BLACK);
    Menu_Sel_Key = &MenuSel_4;
  }else
  
    if( SdrStr.SdrMode.Menu_Sel == AGC_SQL_EQRX_ECHO ){
    sprintf( text,"AGC     SQL      EQ_RX       ");
    TM_ILI9341_Puts(0, 0, text, &TM_Font_11x18, YELLOW, BLACK);
    sprintf(text,  "Sp%01d ", SdrStr.SdrSetup.SpecScale );
    TM_ILI9341_Puts(0, 278, text, &TM_Font_11x18, YELLOW, BLACK);
    Menu_Sel_Key = &MenuSel_5;
  }else
  
    if( SdrStr.SdrMode.Menu_Sel == PWR_TONE_DTONE_TUNE ){
    sprintf( text,"PWR       TONE   DTONE   TUNE");
    TM_ILI9341_Puts(0, 0, text, &TM_Font_11x18, YELLOW, BLACK);
    LcdPwrSet();
    Menu_Sel_Key = &MenuSel_6;
  }else
  
    if( SdrStr.SdrMode.Menu_Sel == VOX_MONI_EQTX_ECHO ){
    sprintf( text,"VOX    MONI     EQ_TX    ECHO");
    TM_ILI9341_Puts(0, 0, text, &TM_Font_11x18, YELLOW, BLACK);
    Menu_Sel_Key = &MenuSel_7;
  }
}
//----------------------------------------------------------------------------
void MenuSel_1( void ){//MODEm_MODEp_BANDm_BANDp
  //===========================================================
  if( (KEY_Current[ KeyA ] == 1 )&&( KEY_Done[ KeyA ] == 0 )){
    if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] == 0 )
      SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] = TotalMode;
    SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]--;

    LcdMode();
    LcdFilters();
    Lcd_Freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] );
    SaveSdrMode = 1;
    KEY_Done[ KeyA ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyB ] == 1 )&&( KEY_Done[ KeyB ] == 0 )){
    
    SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ]++;
    if( SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] == TotalMode )
      SdrStr.SdrMode.Mode[ SdrStr.SdrMode.Band ] = 0;

    LcdMode();
    LcdFilters();
    Lcd_Freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] );
    SaveSdrMode = 1;
    KEY_Done[ KeyB ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyC ] == 1 )&&( KEY_Done[ KeyC ] == 0 )){
    if( SdrStr.SdrMode.Band == 0 )
        SdrStr.SdrMode.Band = TotalBend;
    SdrStr.SdrMode.Band--;

    BendSet();
    SaveFreq = 1;
    SaveSdrMode = 1;
    
    /*LcdMode();
    LcdFilters(); */
    KEY_Done[ KeyC ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyD ] == 1 )&&( KEY_Done[ KeyD ] == 0 )){
    SdrStr.SdrMode.Band++;
    if( SdrStr.SdrMode.Band >= TotalBend )
      SdrStr.SdrMode.Band = 0;

    
    BendSet();
    SaveFreq = 1;
    SaveSdrMode = 1;

   /* LcdMode();
    LcdFilters();*/
    
    KEY_Done[ KeyD ] = 1;
  }
  //===========================================================
}
//----------------------------------------------------------------------------
void MenuSel_2( void ){//FILTm_FILTp_FLITlo_FILThi
  //===========================================================
  if( (KEY_Current[ KeyA ] == 1 )&&( KEY_Done[ KeyA ] == 0 )){
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
    LcdFiltersSet();  
    KEY_Done[ KeyA ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyB ] == 1 )&&( KEY_Done[ KeyB ] == 0 )){
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
    LcdFiltersSet();  
    KEY_Done[ KeyB ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyC ] == 1 )&&( KEY_Done[ KeyC ] == 0 )){
  
    KEY_Done[ KeyC ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyD ] == 1 )&&( KEY_Done[ KeyD ] == 0 )){
  
    KEY_Done[ KeyD ] = 1;
  }
  //===========================================================
}
//----------------------------------------------------------------------------
void MenuSel_3( void ){//PRE_ATT_RF_BYPS
  //===========================================================
  if( (KEY_Current[ KeyA ] == 1 )&&( KEY_Done[ KeyA ] == 0 )){
    if( SdrStr.SdrMode.PRE[ SdrStr.SdrMode.Band ] == 0 )
      SdrStr.SdrMode.PRE[ SdrStr.SdrMode.Band ] = 1;
    else
      SdrStr.SdrMode.PRE[ SdrStr.SdrMode.Band ] = 0;
    SaveSdrMode = 1;
    PreSet();
    Lcd_Pre();  
    KEY_Done[ KeyA ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyB ] == 1 )&&( KEY_Done[ KeyB ] == 0 )){
    SdrStr.SdrMode.ATT[ SdrStr.SdrMode.Band ]++;
    if( SdrStr.SdrMode.ATT[ SdrStr.SdrMode.Band ] >3){
      SdrStr.SdrMode.ATT[ SdrStr.SdrMode.Band ]  = 0;
    }
    SaveSdrMode = 1;
    AttSet();
    Lcd_Att();  
    KEY_Done[ KeyB ] = 1;
  }
  //===========================================================  
  if( (KEY_Current[ KeyC ] == 1 )&&( KEY_Done[ KeyC ] == 0 )){
    SdrStr.Tmp++;
    if( SdrStr.Tmp>=4 )
      SdrStr.Tmp = 0;
    
    if( SdrStr.Tmp == 0 ){
      MIX_RX_ON();
      DDS_freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ]*2 );
    }
    if( SdrStr.Tmp == 1 ){
      MIX_RX_OFF();
      DDS_freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ]*2 );
    }
    if( SdrStr.Tmp == 2 ){
      MIX_RX_ON();
      DDS_freq( 0 );
    }
    if( SdrStr.Tmp == 3 ){
      MIX_RX_OFF();
      DDS_freq( 0 );
    }

    SaveSdrMode = 1;
    Lcd_Dbg();  
    KEY_Done[ KeyC ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyD ] == 1 )&&( KEY_Done[ KeyD ] == 0 )){
    if( SdrStr.SdrMode.BYPS == 0 ){
      SdrStr.SdrMode.BYPS = 1;
      TM_ILI9341_Puts(175, 0, "BPS" , &TM_Font_7x10, RED, BLACK);
    }
    else{
      SdrStr.SdrMode.BYPS = 0;
      TM_ILI9341_Puts(175, 0, "   " , &TM_Font_7x10, RED, BLACK);
    }
    BPF_set();
    KEY_Done[ KeyD ] = 1;
  }
  //===========================================================
}
//----------------------------------------------------------------------------
void MenuSel_4( void ){//ANF_DNR_NB_NBL
  //===========================================================
  if( (KEY_Current[ KeyA ] == 1 )&&( KEY_Done[ KeyA ] == 0 )){
     if( SdrStr.SdrMode.ANF == 0 )
      SdrStr.SdrMode.ANF = 1;
    else
      SdrStr.SdrMode.ANF = 0;
    
    //ANF_filter_init();
      
   // Exchange.ANF = SdrStr.SdrMode.ANF;
    SaveSdrMode = 1;
    Lcd_Anf(); 
    KEY_Done[ KeyA ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyB ] == 1 )&&( KEY_Done[ KeyB ] == 0 )){
    if( SdrStr.SdrMode.DNR == 0 )
      SdrStr.SdrMode.DNR = 1;
    else
      SdrStr.SdrMode.DNR = 0;
    //Exchange.DNR = SdrStr.SdrMode.DNR;
    SaveSdrMode = 1;
    Lcd_Dnr();  
    KEY_Done[ KeyB ] = 1;
  }
  //===========================================================  
  if( (KEY_Current[ KeyC ] == 1 )&&( KEY_Done[ KeyC ] == 0 )){
    SdrStr.SdrMode.NB++;
    if( SdrStr.SdrMode.NB >= 4 )
      SdrStr.SdrMode.NB = 0;
   // Exchange.NB = SdrStr.SdrMode.NB;
    SaveSdrMode = 1;
    Lcd_Nb();
    KEY_Done[ KeyC ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyD ] == 1 )&&( KEY_Done[ KeyD ] == 0 )){
  
    KEY_Done[ KeyD ] = 1;
  }
  //===========================================================
}
//----------------------------------------------------------------------------
void MenuSel_5( void ){//AGC_SQL_EQRX_ECHO
  //===========================================================
  if( (KEY_Current[ KeyA ] == 1 )&&( KEY_Done[ KeyA ] == 0 )){
    if( SdrStr.SdrMode.RTX_Mode ){
      SdrStr.SdrMode.AGC_TX++;
      if(SdrStr.SdrMode.AGC_TX > 5)
        SdrStr.SdrMode.AGC_TX = 0;
      //Exchange.AGC_TX = SdrStr.SdrMode.AGC_TX;
      dspAgc_setGain( SdrStr.SdrSetup.MIC_GAIN );
    }else{
      SdrStr.SdrMode.AGC_RX[ SdrStr.SdrMode.Band ]++;
      if(SdrStr.SdrMode.AGC_RX[ SdrStr.SdrMode.Band ] > 5)
        SdrStr.SdrMode.AGC_RX[ SdrStr.SdrMode.Band ] = 0;
     // Exchange.AGC_RX = SdrStr.SdrMode.AGC_RX;
      dspAgc_setGain( SdrStr.SdrSetup.RX_GAIN );
    }
    SaveSdrMode = 1;
    Lcd_Agc();
    KEY_Done[ KeyA ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyB ] == 1 )&&( KEY_Done[ KeyB ] == 0 )){
  
    KEY_Done[ KeyB ] = 1;
  }
  //===========================================================  
  if( (KEY_Current[ KeyC ] == 1 )&&( KEY_Done[ KeyC ] == 0 )){
    if( SdrStr.SdrMode.RX_EQ)
      SdrStr.SdrMode.RX_EQ = 0;
    else
      SdrStr.SdrMode.RX_EQ = 1;
   // Exchange.RX_EQ = SdrStr.SdrMode.RX_EQ;
    SaveSdrMode = 1;
    Lcd_Eq();  
    KEY_Done[ KeyC ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyD ] == 1 )&&( KEY_Done[ KeyD ] == 0 )){
  
    SdrStr.SdrSetup.SpecScale <<=1;
    if( SdrStr.SdrSetup.SpecScale==16 )
      SdrStr.SdrSetup.SpecScale = 1;
    
    SaveSdrSetup = 1;
    
    sprintf(text,  "Sp%01d ", SdrStr.SdrSetup.SpecScale );
    TM_ILI9341_Puts(0, 278, text, &TM_Font_11x18, YELLOW, BLACK);
    
   // Exchange.SpecScale = SdrStr.SdrSetup.SpecScale;
    KEY_Done[ KeyD ] = 1;
  }
  //===========================================================
}
//----------------------------------------------------------------------------
void MenuSel_6( void ){//PWR_TONE_DTONE_TUNE
  //===========================================================
  if( (KEY_Current[ KeyA ] == 1 )&&( KEY_Done[ KeyA ] == 0 )){
   
    SdrStr.SdrMode.TX_PWR++;
    if( SdrStr.SdrMode.TX_PWR > 4 )
      SdrStr.SdrMode.TX_PWR = 0;
    SaveSdrMode = 1;
    //Exchange.TX_PWR = SdrStr.SdrMode.TX_PWR;
    LcdPwrSet();
    CodecTXPwrSet();
    KEY_Done[ KeyA ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyB ] == 1 )&&( KEY_Done[ KeyB ] == 0 )){
    if( SdrStr.SdrMode.RTX_Mode == 0){
      WaterfallON = 0;
      SdrStr.TX_Tone = 1;
      SdrStr.SdrMode.RTX_Mode = 1;
      //Exchange.RTX_Mode = SdrStr.SdrMode.RTX_Mode;
    }else{
      WaterfallON = 1;
      SdrStr.TX_Tone = 0;
      SdrStr.SdrMode.RTX_Mode = 0;
     // Exchange.RTX_Mode = SdrStr.SdrMode.RTX_Mode;
    }  
    KEY_Done[ KeyB ] = 1;
  }
  //===========================================================  
  if( (KEY_Current[ KeyC ] == 1 )&&( KEY_Done[ KeyC ] == 0 )){
    if( SdrStr.SdrMode.RTX_Mode == 0){
      WaterfallON = 0;
      SdrStr.TX_Tone = 2;
      SdrStr.SdrMode.RTX_Mode = 1;
     // Exchange.RTX_Mode = SdrStr.SdrMode.RTX_Mode;
    }else{
      WaterfallON = 1;
      SdrStr.TX_Tone = 0;
      SdrStr.SdrMode.RTX_Mode = 0;
     // Exchange.RTX_Mode = SdrStr.SdrMode.RTX_Mode;
    }  
    KEY_Done[ KeyC ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyD ] == 1 )&&( KEY_Done[ KeyD ] == 0 )){
      if( SdrStr.SdrMode.RTX_Mode == 0){
      WaterfallON = 0;
      SdrStr.TX_Tone = 0;
      SdrStr.SdrMode.RTX_Mode = 1;
      //Exchange.RTX_Mode = SdrStr.SdrMode.RTX_Mode;
    }else{
      WaterfallON = 1;
      SdrStr.TX_Tone = 0;
      SdrStr.SdrMode.RTX_Mode = 0;
     // Exchange.RTX_Mode = SdrStr.SdrMode.RTX_Mode;
    } 
    KEY_Done[ KeyD ] = 1;
  }
  //===========================================================
}
//----------------------------------------------------------------------------
void MenuSel_7( void ){//VOX_MONI_EQTX_ECHO
  //===========================================================
  if( (KEY_Current[ KeyA ] == 1 )&&( KEY_Done[ KeyA ] == 0 )){
  
    KEY_Done[ KeyA ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyB ] == 1 )&&( KEY_Done[ KeyB ] == 0 )){
    if(SdrStr.SdrMode.Moni == 0){
      SdrStr.SdrMode.Moni  = 1;
      if(SdrStr.SdrMode.RTX_Mode==1)
        Spi_wr ( DAC_VOLUME_A,  0,  CODEC_CS_RX );
    }
    else{
      SdrStr.SdrMode.Moni  = 0;
      if(SdrStr.SdrMode.RTX_Mode==1)
        Spi_wr ( DAC_VOLUME_A,  0x80,  CODEC_CS_RX );
    }
    //Exchange.Moni = SdrStr.SdrMode.Moni;
    SaveSdrMode = 1;
    Lcd_Moni();  
    KEY_Done[ KeyB ] = 1;
  }
  //===========================================================  
  if( (KEY_Current[ KeyC ] == 1 )&&( KEY_Done[ KeyC ] == 0 )){
    if( SdrStr.SdrMode.TX_EQ)
      SdrStr.SdrMode.TX_EQ = 0;
    else
      SdrStr.SdrMode.TX_EQ = 1;
   // Exchange.TX_EQ = SdrStr.SdrMode.TX_EQ;
    SaveSdrMode = 1;
    Lcd_Eq();  
    KEY_Done[ KeyC ] = 1;
  }
  //===========================================================
  if( (KEY_Current[ KeyD ] == 1 )&&( KEY_Done[ KeyD ] == 0 )){
    if( SdrStr.SdrMode.TX_ECHO)
      SdrStr.SdrMode.TX_ECHO = 0;
    else
      SdrStr.SdrMode.TX_ECHO = 1;
    //Exchange.TX_ECHO = SdrStr.SdrMode.TX_ECHO;
    SaveSdrMode = 1;
    Lcd_Echo();  
    KEY_Done[ KeyD ] = 1;
  }
  //===========================================================
}
//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
void BandKey( void ){
  
  if(( KEY_Done[ BANDP ] == 1 )||( KEY_Done[ BANDM ] == 1 ))
    return;
  if(( KEY_Current[ BANDP ] == 0 )&&( KEY_Current[ BANDM ] == 0 ))
    return;
  
  
  if( KEY_Current[ BANDP ] == 1 ){
    SdrStr.SdrMode.Band++;
    if( SdrStr.SdrMode.Band >= TotalBend )
      SdrStr.SdrMode.Band = 0;
    
    BendSet();
  }  
  
  
  if( KEY_Current[ BANDM ] == 1 ){
    if( SdrStr.SdrMode.Band == 0 )
        SdrStr.SdrMode.Band = TotalBend;
      SdrStr.SdrMode.Band--;
    
    BendSet();
  }
  
  KEY_Done[ BANDP ] = 1;
  KEY_Done[ BANDM ] = 1;
}
//----------------------------------------------------------------------------
void BendSet( void ){
  if( (SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] <BandTable[SdrStr.SdrMode.Band][0] )||(SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] > BandTable[SdrStr.SdrMode.Band][1] ) )
    SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] =  (BandTable[SdrStr.SdrMode.Band][0]+BandTable[SdrStr.SdrMode.Band][1])/2;

  if( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ]%50 !=0 )
    SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] -= SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ]%50;
  
  DDS_freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] * 2 );
  Lcd_Freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] );
  
 
  
  SaveFreq = 1;
  SaveSdrMode = 1;
  
  Lcd_Pre();
  PreSet();
  
  Lcd_Att();
  AttSet(); 

  LcdMode();
  LcdFilters();  
  Lcd_Agc();
  
  SdrStr.FmPll_Init = 1;
  
  if(MenuSetup == 0){
    if(  SdrStr.SdrMode.Menu_Sel == FILTm_FILTp_FLITlo_FILThi);
      MenuChoiseLcd();
  }
}
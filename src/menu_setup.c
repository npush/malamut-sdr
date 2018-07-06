#include "main.h"
#include "menu_subsetup.h"

unsigned char MenuSetupSelPozition = 0;
unsigned char MenuSetupSelSubPozition = 0;


#pragma pack (1)
const MENU_SETUP_CHAR MenuSetupChar[]={
  {"SDR",               NULL, SdrDispSetupMenu},
  {"IQ Imb[BAND]",      NULL, IQDispSetupMenu},
  {"Spektr",            NULL, SpecDispSetupMenu},
  {"Clock",             NULL, ClockDispSetupMenu},
  {"DDS",               NULL, DdsDispSetupMenu},
  {"RX Gain",           NULL, RxgDispSetupMenu},
  {"MIC Gain",          NULL, MicgDispSetupMenu},
  {"Filter SSB",        NULL, FSsbDispSetupMenu},
  {"Filter AMFM",       NULL, FamfmDispSetupMenu},
  {"AGC",               NULL, NULL},
  {"Equal RX",          NULL, EqRxDispSetupMenu},
  {"Equal TX",          NULL, EqTxDispSetupMenu},
  {"Echo  TX ",         NULL, EchoDispSetupMenu},
  {"VOX Level",         NULL, NULL},
  {"ANF coeff",         NULL, NULL},//coefficient
  {"DNR coeff",         NULL, DnrDispSetupMenu},
  {"S-meter",           NULL, NULL},
  {"Moni Vol",          NULL, MoniVolDispSetupMenu},
  {"TX Setup",          NULL, TxSetupDispSetupMenu},
  {"Temp",              NULL, TempDispSetupMenu}
};
void (*Menu_Setup_Selected)();


#pragma pack (1)
typedef struct  _sMenuSetupDispStr{
  unsigned char TotalSelPoz;
  unsigned char * SelPos;

  unsigned char SetVar;
  unsigned char ValEN;
  unsigned long TimeBlinkSetVar;
  unsigned char ukaz;//MenuSetupDisp
  
  PMENU_SETUP_CHAR MenuSetupCharStr;
}_sMenuSetupDispStr;

_sMenuSetupDispStr MenuSetupDispStr;

//----------------------------------------------------------------------------
void MenuSetupDrawDwnKey( void ){
  sprintf( text,"Up      Dwn      Set     EXIT");//29
  TM_ILI9341_Puts(0, 0, text, &TM_Font_11x18, YELLOW, BLACK);
  Menu_Setup_Selected = NULL;
}
//----------------------------------------------------------------------------
void MenuSetupSetDisp( void ){
  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)MenuSetupChar;
  MenuSetupDispStr.TotalSelPoz = sizeof( MenuSetupChar )/sizeof(MENU_SETUP_CHAR);
  MenuSetupDispStr.SelPos = &MenuSetupSelPozition;
  MenuSetupDispStr.SetVar = 0;
}
void MenuSetupDisp( void ){
  //int ukaz;
  MenuSetupDispStr.ukaz = 0;
  int strl,i,m;
  m=min(3,MenuSetupDispStr.TotalSelPoz);
  TM_ILI9341_DrawFilledRectangleFast( 0,20, 256, 80, BLACK );
//  for( i = 0; i<MenuSetupDispStr.TotalMaxDispSelPoz; ++i ){
  for( i = 0; i<m; ++i ){
    
    MenuSetupDispStr.ukaz = (*MenuSetupDispStr.SelPos) + i;
    if( MenuSetupDispStr.ukaz >= MenuSetupDispStr.TotalSelPoz )
      MenuSetupDispStr.ukaz -= MenuSetupDispStr.TotalSelPoz;
    
    sprintf( text,"%2d", MenuSetupDispStr.ukaz+1 );
    strl = strlen( text );
    sprintf( &text[strl]," %s", MenuSetupDispStr.MenuSetupCharStr[MenuSetupDispStr.ukaz].SelName );
    
    if( MenuSetupDispStr.MenuSetupCharStr[MenuSetupDispStr.ukaz].VarDispFunck != NULL ){
      strl = strlen( text );
      MenuSetupDispStr.MenuSetupCharStr[MenuSetupDispStr.ukaz].VarDispFunck(&text[strl]);
    }
    TM_ILI9341_Puts(60-(i*19), 15, text, &TM_Font_11x18, WHITE, BLACK);

  }  
  //TM_ILI9341_DrawTriangleRight( 3,63,WHITE );
  TM_ILI9341_Puts(60, 0, ">", &TM_Font_11x18, WHITE, BLACK);  
}
//----------------------------------------------------------------------------
void MenuSetupDispBlink( void ){
  int strl = 3+strlen( MenuSetupDispStr.MenuSetupCharStr[(*MenuSetupDispStr.SelPos)].SelName );
  
  TM_ILI9341_DrawFilledRectangleFast( 15+strl*11,60, 256, 80, BLACK );
  
  MenuSetupDispStr.ukaz = (*MenuSetupDispStr.SelPos);//for massive 
  MenuSetupDispStr.MenuSetupCharStr[(*MenuSetupDispStr.SelPos)].VarDispFunck(text);
  if( MenuSetupDispStr.SetVar == 0  )
    TM_ILI9341_Puts(60, 15+strl*11, text, &TM_Font_11x18, WHITE, BLACK);
  if( MenuSetupDispStr.SetVar == 1  )
    TM_ILI9341_Puts(60, 15+strl*11, text, &TM_Font_11x18, RED, BLACK);
  if( MenuSetupDispStr.SetVar == 2  )
    TM_ILI9341_Puts(60, 15+strl*11, text, &TM_Font_11x18, GREEN, BLACK);
}
//----------------------------------------------------------------------------
void MenuSetupScan( void ){
 
  
 /* if( Menu_Setup_Selected!=NULL ){
    Menu_Setup_Selected();
    return;
  }*/
  
  if( (KEY_Current[ KeyA ] == 1 )&&( KEY_Done[ KeyA ] == 0 )){//Up
    if( MenuSetupDispStr.SetVar == 0 ){
      if( (*MenuSetupDispStr.SelPos) == 0 )
        (*MenuSetupDispStr.SelPos) = MenuSetupDispStr.TotalSelPoz;
      (*MenuSetupDispStr.SelPos)--;
      MenuSetupDisp();
    }
    else{
      if(MenuSetupDispStr.MenuSetupCharStr[(*MenuSetupDispStr.SelPos)].SetFunck!=NULL)
        MenuSetupDispStr.MenuSetupCharStr[(*MenuSetupDispStr.SelPos)].SetFunck(1);
      MenuSetupDispBlink();
    }
    KEY_Done[ KeyA ]  = 1;
  }
  if( (KEY_Current[ KeyB ] == 1 )&&( KEY_Done[ KeyB ] == 0 )){//Dwn
    if( MenuSetupDispStr.SetVar ==0 ){
      (*MenuSetupDispStr.SelPos)++;
      if( (*MenuSetupDispStr.SelPos) == MenuSetupDispStr.TotalSelPoz )
        (*MenuSetupDispStr.SelPos) = 0;
      MenuSetupDisp();
    }
    else{
      if(MenuSetupDispStr.MenuSetupCharStr[(*MenuSetupDispStr.SelPos)].SetFunck!=NULL)
        MenuSetupDispStr.MenuSetupCharStr[(*MenuSetupDispStr.SelPos)].SetFunck(-1);
      MenuSetupDispBlink();
    }
    KEY_Done[ KeyB ]  = 1;
  }  
  
  
  
  if( (KEY_Current[ KeyC ] == 1 )&&( KEY_Done[ KeyC ] == 0 )){//Set
    if( MenuSetupDispStr.SelPos == (&MenuSetupSelPozition)  ){
  
      MenuSetupDispStr.ukaz = SdrStr.SdrMode.RTX_Mode;
      EEP_Load();
      SdrStr.SdrMode.RTX_Mode = MenuSetupDispStr.ukaz; 
      //Exchange.RTX_Mode = SdrStr.SdrMode.RTX_Mode;
  
      if( MenuSetupDispStr.MenuSetupCharStr[(*MenuSetupDispStr.SelPos)].SetFunck != NULL ){
        MenuSetupDispStr.MenuSetupCharStr[(*MenuSetupDispStr.SelPos)].SetFunck();
        MenuSetupDisp();
        SdrStr.ValMode = 0;
        SdrStr.ValCount = 0;
      }
    }else{
      if( MenuSetupDispStr.SetVar ==0 ){
        MenuSetupDispStr.TimeBlinkSetVar = RTC_Get();
        MenuSetupDisp();
        if( MenuSetupDispStr.ValEN )
          SdrStr.ValMode = 1;
        SdrStr.ValCount = 0;
        MenuSetupDispStr.SetVar = 1;
      }else{
        MenuSetupDisp();
        SdrStr.ValMode = 0;
        SdrStr.ValCount = 0;
        MenuSetupDispStr.SetVar = 0;
        SaveSdrMode = 1;
        SaveSdrSetup = 1;
      }
    }
    KEY_Done[ KeyC ]  = 1;
  }

  if( MenuSetupDispStr.SetVar > 0 ){//blink
    if( Interval_RTC_Get( MenuSetupDispStr.TimeBlinkSetVar )>250 ){
      if( MenuSetupDispStr.SetVar == 1 )
        MenuSetupDispStr.SetVar = 2;
      else
        MenuSetupDispStr.SetVar = 1;
      MenuSetupDispBlink();
      MenuSetupDispStr.TimeBlinkSetVar = RTC_Get();
    }
  }
  if( (SdrStr.ValMode == 1)&&(MenuSetupDispStr.SetVar) ){
    if(MenuSetupDispStr.MenuSetupCharStr[(*MenuSetupDispStr.SelPos)].SetFunck!=NULL){
      if( SdrStr.ValCount!=0 ){
        MenuSetupDispStr.MenuSetupCharStr[(*MenuSetupDispStr.SelPos)].SetFunck(0);
        MenuSetupDispBlink();
        SdrStr.ValCount = 0;
      }
    }
  }
  
  
  
  if( (KEY_Current[ KeyD ] == 1 )&&( KEY_Done[ KeyD ] == 0 )){//EXIT
    SdrStr.ValMode = 0;
    SdrStr.ValCount = 0;
    
    if( MenuSetupDispStr.SelPos == (&MenuSetupSelSubPozition)  ){
      MenuSetupSelSubPozition = 0;
      MenuSetupSetDisp();
      MenuSetupDisp();
    }
    else{
      TM_ILI9341_DrawFilledRectangleFast( 0,20, 256, 80, BLACK );
      if( SdrStr.SdrMode.RTX_Mode == 0 )
        WaterfallON = 1;
      MenuSetup = 0;
      MenuChoiseLcd();      
    }
    KEY_Done[ KeyD ]  = 1;
  }
}
//----------------------------------------------------------------------------






//----------------------------------------------------------------------------
void FFT_Size_Disp( char *txt ){
  sprintf( txt, "....%2d ", SdrStr.SdrSetup.FFT_Size );
}

void Sample_Rate_Disp( char *txt ){
  sprintf( txt, "...%2dk ", SdrStr.SdrSetup.Sample_Rate/1000 );
}


void FFT_Size_Set( int p ){
  if( p>0 ){
    if( SdrStr.SdrSetup.FFT_Size < 2048 )
      SdrStr.SdrSetup.FFT_Size *=2;
  }
  if(p<0){
    if( SdrStr.SdrSetup.FFT_Size >256 )
      SdrStr.SdrSetup.FFT_Size /=2;
  }
  
  //Exchange.FFT_Size = SdrStr.SdrSetup.FFT_Size;
  SDR_init();

}


void Sample_Rate_Set( int p ){
  SdrStr.SdrSetup.FILTER_AMFM[7] = 24000;
  
    if( p>0 ){
      if( SdrStr.SdrSetup.Sample_Rate == 8000 )
        SdrStr.SdrSetup.Sample_Rate = 11025;
      else
        if( SdrStr.SdrSetup.Sample_Rate == 11025 )
          SdrStr.SdrSetup.Sample_Rate = 16000;
        else
          if( SdrStr.SdrSetup.Sample_Rate == 16000 )
            SdrStr.SdrSetup.Sample_Rate = 22050;
          else
          if( SdrStr.SdrSetup.Sample_Rate == 22050 )
            SdrStr.SdrSetup.Sample_Rate = 32000;
          else
            if( SdrStr.SdrSetup.Sample_Rate == 32000 )
              SdrStr.SdrSetup.Sample_Rate = 44100;
            else
              if( SdrStr.SdrSetup.Sample_Rate == 44100 )
                SdrStr.SdrSetup.Sample_Rate = 48000;
              else
                if( SdrStr.SdrSetup.Sample_Rate == 48000 ){
                  SdrStr.SdrSetup.Sample_Rate = 96000;
                  SdrStr.SdrSetup.FILTER_AMFM[7] = SdrStr.SdrSetup.Sample_Rate/2;
                }
               /* else
                  if( SdrStr.SdrSetup.Sample_Rate == 96000 ){
                    SdrStr.SdrSetup.Sample_Rate = 192000;
                    SdrStr.SdrSetup.FILTER_AMFM[7] = SdrStr.SdrSetup.Sample_Rate/2;
                  }*/
    }
    if(p<0){
     /* if( SdrStr.SdrSetup.Sample_Rate == 192000 )
        SdrStr.SdrSetup.Sample_Rate = 96000;
      else*/
        if( SdrStr.SdrSetup.Sample_Rate == 96000 )
          SdrStr.SdrSetup.Sample_Rate = 48000;
        else
          if( SdrStr.SdrSetup.Sample_Rate == 48000 )
            SdrStr.SdrSetup.Sample_Rate = 44100;
          else
            if( SdrStr.SdrSetup.Sample_Rate == 44100 )
              SdrStr.SdrSetup.Sample_Rate = 32000;
            else
              if( SdrStr.SdrSetup.Sample_Rate == 32000 )
                SdrStr.SdrSetup.Sample_Rate = 22050;
              else
              if( SdrStr.SdrSetup.Sample_Rate == 22050 )
                SdrStr.SdrSetup.Sample_Rate = 16000;
              else
                if( SdrStr.SdrSetup.Sample_Rate == 16000 )
                  SdrStr.SdrSetup.Sample_Rate = 11025;
                else
                  if( SdrStr.SdrSetup.Sample_Rate == 11025 )
                    SdrStr.SdrSetup.Sample_Rate = 8000;      
      }
  //Exchange.Sample_Rate = SdrStr.SdrSetup.Sample_Rate;    
  SDR_init();

}

#define TotalSdrChar      2
const MENU_SETUP_CHAR SdrChar[]={
  {"FFT Size", FFT_Size_Disp, FFT_Size_Set},
  {"Smpl Rate", Sample_Rate_Disp, Sample_Rate_Set}
};
void SdrDispSetupMenu( void ){
  MenuSetupDispStr.TotalSelPoz = TotalSdrChar;
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 0;
  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)SdrChar;
}
//----------------------------------------------------------------------------
void Rx_Mag_Disp( char *txt ){
   sprintf( txt, "...%1.4f ", SdrStr.SdrSetup.rx_mag[SdrStr.SdrMode.Band] );
}
void Rx_Ph_Disp( char *txt ){
   sprintf( txt, "...%1.4f ", SdrStr.SdrSetup.rx_ph[SdrStr.SdrMode.Band] );
}
void Tx_Mag_Disp( char *txt ){
   sprintf( txt, "...%1.4f ", SdrStr.SdrSetup.tx_mag[SdrStr.SdrMode.Band] );
}
void Tx_Ph_Disp( char *txt ){
   sprintf( txt, "...%1.4f ", SdrStr.SdrSetup.tx_ph[SdrStr.SdrMode.Band] );
}

void Rx_Mag_Set( int p ){
  if( p==0 ){
    p=SdrStr.ValCount;
  }
  if(p!=0){
    SdrStr.SdrSetup.rx_mag[SdrStr.SdrMode.Band]+=((float)p/10000.0);
    if(SdrStr.SdrSetup.rx_mag[SdrStr.SdrMode.Band] < 0)
      SdrStr.SdrSetup.rx_mag[SdrStr.SdrMode.Band] = 0;
    //Exchange.rx_mag=SdrStr.SdrSetup.rx_mag[SdrStr.SdrMode.Band];

  }
}
void Rx_Ph_Set( int p ){
  if( p==0 ){
    p=SdrStr.ValCount;
  }
  if(p!=0){
    SdrStr.SdrSetup.rx_ph[SdrStr.SdrMode.Band]+=((float)p/10000.0);
    //Exchange.rx_ph=SdrStr.SdrSetup.rx_ph[SdrStr.SdrMode.Band];

  }
}
void Tx_Mag_Set( int p ){
  if( p==0 ){
    p=SdrStr.ValCount;
  }
  if(p!=0){
    SdrStr.SdrSetup.tx_mag[SdrStr.SdrMode.Band]+=((float)p/10000.0);
    if(SdrStr.SdrSetup.tx_mag[SdrStr.SdrMode.Band] < 0)
      SdrStr.SdrSetup.tx_mag[SdrStr.SdrMode.Band] = 0;
    //Exchange.tx_mag=SdrStr.SdrSetup.tx_mag[SdrStr.SdrMode.Band];

  }
}
void Tx_Ph_Set( int p ){
  if( p==0 ){
    p=SdrStr.ValCount;
  }
  if(p!=0){
    SdrStr.SdrSetup.tx_ph[SdrStr.SdrMode.Band]+=((float)p/10000.0);
    //Exchange.tx_ph=SdrStr.SdrSetup.tx_ph[SdrStr.SdrMode.Band];

  }
}
#define TotalIQChar      4
const MENU_SETUP_CHAR IQChar[]={
  {"Rx Mag", Rx_Mag_Disp, Rx_Mag_Set},
  {"Rx Ph ", Rx_Ph_Disp, Rx_Ph_Set},
  {"Tx Mag", Tx_Mag_Disp, Tx_Mag_Set},
  {"Tx Ph ", Tx_Ph_Disp, Tx_Ph_Set}
};

void IQDispSetupMenu( void ){
  MenuSetupDispStr.TotalSelPoz = TotalIQChar;
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 1;

  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)IQChar;  
}
//----------------------------------------------------------------------------
void Spect_del_DispUp( char *txt ){
  sprintf( txt, "....%1.2f ", SdrStr.SdrSetup.FFT_Spect_beta_up );
}
void Spect_del_DispDwn( char *txt ){
  sprintf( txt, "...%1.2f ", SdrStr.SdrSetup.FFT_Spect_beta_dwn );
}
void Spect_ofset_Disp( char *txt ){
  sprintf( txt, "...%d ", SdrStr.SdrSetup.FFT_Spect_ofset );
}
    
void Spect_del_SetUp( int p ){
  if( p==0 )
    p=SdrStr.ValCount;
    
  SdrStr.SdrSetup.FFT_Spect_beta_up += p*0.02;
  if( SdrStr.SdrSetup.FFT_Spect_beta_up>0.98 )
    SdrStr.SdrSetup.FFT_Spect_beta_up = 0.98;
  if(SdrStr.SdrSetup.FFT_Spect_beta_up < 0)
    SdrStr.SdrSetup.FFT_Spect_beta_up=0;
  
  //Exchange.FFT_Spect_beta_up = SdrStr.SdrSetup.FFT_Spect_beta_up;
}
void Spect_del_SetDwn( int p ){
  if( p==0 )
    p=SdrStr.ValCount;

  SdrStr.SdrSetup.FFT_Spect_beta_dwn += p*0.02;
  if( SdrStr.SdrSetup.FFT_Spect_beta_dwn>0.98 )
    SdrStr.SdrSetup.FFT_Spect_beta_dwn = 0.98;
  if(SdrStr.SdrSetup.FFT_Spect_beta_dwn < 0)
    SdrStr.SdrSetup.FFT_Spect_beta_dwn=0;
  
  //Exchange.FFT_Spect_beta_dwn = SdrStr.SdrSetup.FFT_Spect_beta_dwn;
}
void Spect_ofset_Set( int p ){
  if( p==0 )
    p=SdrStr.ValCount;

  SdrStr.SdrSetup.FFT_Spect_ofset+=p;
  if( p>0 ){
    if(SdrStr.SdrSetup.FFT_Spect_ofset > 200)
      SdrStr.SdrSetup.FFT_Spect_ofset = 200;
  }else{
    if(SdrStr.SdrSetup.FFT_Spect_ofset > 200)
      SdrStr.SdrSetup.FFT_Spect_ofset = 0;
    }
}
#define TotalSpecChar      3
const MENU_SETUP_CHAR SpecChar[]={
  {"Spect up", Spect_del_DispUp, Spect_del_SetUp},
  {"Spect dwn",Spect_del_DispDwn, Spect_del_SetDwn},
  {"Spect ofset",Spect_ofset_Disp, Spect_ofset_Set},
};
void SpecDispSetupMenu( void ){
  MenuSetupDispStr.TotalSelPoz = TotalSpecChar;
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 1;
  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)SpecChar;
}
//----------------------------------------------------------------------------
void RTC_HoursDisp(char *txt){
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_GetTime(RTC_HourFormat_24, &RTC_TimeStruct);
  sprintf( txt, "......%2d", RTC_TimeStruct.RTC_Hours );
}
void RTC_MinutesDisp(char *txt){
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_GetTime(RTC_HourFormat_24, &RTC_TimeStruct);
  sprintf( txt, "....%02d", RTC_TimeStruct.RTC_Minutes );
}
void RTC_H12Disp(char *txt){
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_GetTime(RTC_HourFormat_24, &RTC_TimeStruct);
  sprintf( txt, "........%s", RTC_TimeStruct.RTC_H12 ? ("12"):("24")  );
}
void RTC_DateDisp(char *txt){
  RTC_DateTypeDef RTC_DateStruct;
  RTC_GetDate(RTC_HourFormat_24, &RTC_DateStruct);
  sprintf( txt, ".......%2d", RTC_DateStruct.RTC_Date );
}
void RTC_MonthDisp(char *txt){
  RTC_DateTypeDef RTC_DateStruct;
  RTC_GetDate(RTC_HourFormat_24, &RTC_DateStruct);
  sprintf( txt, "......%2d", RTC_DateStruct.RTC_Month );
}
void RTC_YearDisp(char *txt){
  RTC_DateTypeDef RTC_DateStruct;
  RTC_GetDate(RTC_HourFormat_24, &RTC_DateStruct);
  sprintf( txt, ".......%02d", RTC_DateStruct.RTC_Year );
}
void RTC_HoursSet( int p ){
  RTC_TimeTypeDef RTC_TimeStruct;
  if( p==0 ){
    p=SdrStr.ValCount;
  }
  if( p==0 )
    return;

  RTC_GetTime(RTC_HourFormat_24, &RTC_TimeStruct);
  RTC_TimeStruct.RTC_Hours+=p;
  if((p>0)&&(RTC_TimeStruct.RTC_Hours>23))
    RTC_TimeStruct.RTC_Hours = 0;
  if((p<0)&&(RTC_TimeStruct.RTC_Hours>23))
    RTC_TimeStruct.RTC_Hours = 23;

  RTC_TimeStruct.RTC_Seconds = 0;
  RTC_SetTime( RTC_HourFormat_24, &RTC_TimeStruct );
  RTC_GetTime(RTC_HourFormat_24, &RTC_TimeStruct);

}
void RTC_MinutesSet( int p ){
  RTC_TimeTypeDef RTC_TimeStruct;
  if( p==0 ){
    p=SdrStr.ValCount;
  }
  if( p==0 )
    return;

  RTC_GetTime(RTC_HourFormat_24, &RTC_TimeStruct);
  RTC_TimeStruct.RTC_Minutes+=p;
  if((p>0)&&(RTC_TimeStruct.RTC_Minutes>59))
    RTC_TimeStruct.RTC_Minutes = 0;
  if((p<0)&&(RTC_TimeStruct.RTC_Minutes>59))
    RTC_TimeStruct.RTC_Minutes = 59;
  
  RTC_TimeStruct.RTC_Seconds = 0;
  RTC_SetTime( RTC_HourFormat_24, &RTC_TimeStruct );
  RTC_GetTime(RTC_HourFormat_24, &RTC_TimeStruct);

}

void RTC_DateSet( int p ){
  RTC_DateTypeDef RTC_DateStruct;
  if( p==0 ){
    p=SdrStr.ValCount;
    if(p>0)
      p=1;
    if(p<0)
      p=(-1);
  }
  if( p==0 )
    return;
  
  int dtm=31;
  RTC_GetDate(RTC_HourFormat_24, &RTC_DateStruct);
  RTC_DateStruct.RTC_Date+=p;
  if( RTC_DateStruct.RTC_Month == 2 ){
    if( ( RTC_DateStruct.RTC_Year%4)==0 )
      dtm = 29;
    else
      dtm = 28;
  }
  if((p>0)&&(RTC_DateStruct.RTC_Date>dtm))
    RTC_DateStruct.RTC_Date = 1;
  if((p<0)&&(RTC_DateStruct.RTC_Date==0))
    RTC_DateStruct.RTC_Date = dtm;
  
  RTC_SetDate( RTC_HourFormat_24, &RTC_DateStruct );
  RTC_GetDate(RTC_HourFormat_24, &RTC_DateStruct);   
  
  
  /* RTC_TimeTypeDef RTC_TimeStruct;
  RTC_TimeStruct.RTC_Seconds = 50;
  RTC_TimeStruct.RTC_Minutes = 59;
  RTC_TimeStruct.RTC_Hours = 23;
  RTC_SetTime( RTC_HourFormat_24, &RTC_TimeStruct );
  RTC_GetTime(RTC_HourFormat_24, &RTC_TimeStruct);*/
}
void RTC_MonthSet( int p ){
  RTC_DateTypeDef RTC_DateStruct;
  if( p==0 ){
    p=SdrStr.ValCount;
    if(p>0)
      p=1;
    if(p<0)
      p=(-1);
  }
  if( p==0 )
    return;

  RTC_GetDate(RTC_HourFormat_24, &RTC_DateStruct);
  RTC_DateStruct.RTC_Month+=p;
  if((p>0)&&(RTC_DateStruct.RTC_Month>12))
    RTC_DateStruct.RTC_Month = 1;
  if((p<0)&&(RTC_DateStruct.RTC_Month==0))
    RTC_DateStruct.RTC_Month = 12;
  
  RTC_SetDate( RTC_HourFormat_24, &RTC_DateStruct );
  RTC_GetDate(RTC_HourFormat_24, &RTC_DateStruct);  
}
void RTC_YearSet( int p ){
  RTC_DateTypeDef RTC_DateStruct;
  if( p==0 ){
    p=SdrStr.ValCount;
  }
  if( p==0 )
    return;
  
  RTC_GetDate(RTC_HourFormat_24, &RTC_DateStruct);
  RTC_DateStruct.RTC_Year+=p;
  if((p>0)&&(RTC_DateStruct.RTC_Year>99))
    RTC_DateStruct.RTC_Year = 0;
  if((p<0)&&(RTC_DateStruct.RTC_Year>99))
    RTC_DateStruct.RTC_Year = 99;
  
  RTC_SetDate( RTC_HourFormat_24, &RTC_DateStruct );
  RTC_GetDate(RTC_HourFormat_24, &RTC_DateStruct);  
}

#define TotalClockChar      6
const MENU_SETUP_CHAR ClockChar[]={
  {"RTC_Hours",         RTC_HoursDisp,  RTC_HoursSet},
  {"RTC_Minutes",       RTC_MinutesDisp, RTC_MinutesSet},
  {"RTC_H12",           RTC_H12Disp,    NULL},
  {"RTC_Date",          RTC_DateDisp,   RTC_DateSet},
  {"RTC_Month",         RTC_MonthDisp,  RTC_MonthSet},
  {"RTC_Year",          RTC_YearDisp,   RTC_YearSet}
};
void ClockDispSetupMenu( void ){
  MenuSetupDispStr.TotalSelPoz = TotalClockChar;
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 1;

  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)ClockChar;
}
void ClockSetupMenu( void ){

}
//----------------------------------------------------------------------------
void DDS_Type_Disp( char* txt ){
  if( SdrStr.SdrSetup.DDS_type == 0 )
    sprintf( txt, "...AD9850 "  );
  if( SdrStr.SdrSetup.DDS_type == 1 )
    sprintf( txt, "...AD9851 "  );
  if( SdrStr.SdrSetup.DDS_type == 2 )
    sprintf( txt, "...AD995x "  );
  if( SdrStr.SdrSetup.DDS_type == 3 )
    sprintf( txt, "...Si5351"  );
}
void DDS_Mck_Disp( char* txt ){
  sprintf( txt, "..%d", SdrStr.SdrSetup.DDS_mck[SdrStr.SdrSetup.DDS_type] );
}
void DDS_Mult_Disp( char* txt ){
    sprintf( txt, "...%d", SdrStr.SdrSetup.DDS_mul[SdrStr.SdrSetup.DDS_type]  );
}

void DDS_Type_Set( int p ){
  if( p==0 ){
    p=SdrStr.ValCount;
  }
  if( p==0 )
    return;

  SdrStr.SdrSetup.DDS_type +=p;
  if( (p<0)&&(SdrStr.SdrSetup.DDS_type > 3) )
    SdrStr.SdrSetup.DDS_type = 0;
  if(SdrStr.SdrSetup.DDS_type > 3)
    SdrStr.SdrSetup.DDS_type = 3;

  MenuSetupDisp();
  
  DDS_init();
  DDS_freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ]*2 );

}
void DDS_Mck_Set( int p ){
  if( p==0 ){
    p=SdrStr.ValCount;
  }
  if( p==0 )
    return;
  
  if( KEY_Current[ SEL ] == 1 )
   p*=10000;
       
  SdrStr.SdrSetup.DDS_mck[SdrStr.SdrSetup.DDS_type] +=p;
  
  DDS_init();
  DDS_freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ]*2 );
  if( SdrStr.SdrSetup.DDS_type == 3 ) 
    Si5351_freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] * 2 );

}
void DDS_Mult_Set(int p){
  if( p==0 ){
    p=SdrStr.ValCount;
  }
  if( p==0 )
    return;
  
  if( SdrStr.SdrSetup.DDS_type == 0 ){
    SdrStr.SdrSetup.DDS_mul[SdrStr.SdrSetup.DDS_type] = 1;
  }
  
  if( SdrStr.SdrSetup.DDS_type == 1 ){
    if( p>0 )
      SdrStr.SdrSetup.DDS_mul[SdrStr.SdrSetup.DDS_type] = 6;
    else
      SdrStr.SdrSetup.DDS_mul[SdrStr.SdrSetup.DDS_type] = 1;
  }

  if( SdrStr.SdrSetup.DDS_type == 2 ){
    SdrStr.SdrSetup.DDS_mul[SdrStr.SdrSetup.DDS_type] += p;
    if(  SdrStr.SdrSetup.DDS_mul[SdrStr.SdrSetup.DDS_type] > 20  )
      SdrStr.SdrSetup.DDS_mul[SdrStr.SdrSetup.DDS_type] = 20;
    if(  SdrStr.SdrSetup.DDS_mul[SdrStr.SdrSetup.DDS_type] < 4  )
      SdrStr.SdrSetup.DDS_mul[SdrStr.SdrSetup.DDS_type] = 4;
  }
  
  DDS_init();
  DDS_freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ]*2 );

}
#define TotalDdsChar      3
const MENU_SETUP_CHAR DdsChar[]={
  {"DDS Type",          DDS_Type_Disp, DDS_Type_Set},
  {"DDS Mck ",          DDS_Mck_Disp,  DDS_Mck_Set},
  {"DDS Mult",          DDS_Mult_Disp, DDS_Mult_Set},
};
void DdsDispSetupMenu( void ){
  MenuSetupDispStr.TotalSelPoz = TotalDdsChar;
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 1;

  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)DdsChar;
}
//----------------------------------------------------------------------------
void RXGainDisp( char* txt ){
    sprintf( txt, ".....%3d", SdrStr.SdrSetup.RX_GAIN );
}
void RXGainSet( int p ){
  if( p==0 ){
    p=SdrStr.ValCount;
  }

  SdrStr.SdrSetup.RX_GAIN+=p;
  if(SdrStr.SdrSetup.RX_GAIN>4095){
    if( p<0 )
      SdrStr.SdrSetup.RX_GAIN = 1;
    if(p>0)
      SdrStr.SdrSetup.RX_GAIN = 4095;
  }
  
  //Exchange.RX_GAIN = SdrStr.SdrSetup.RX_GAIN;
  //dspAgc_Init();
  if(SdrStr.SdrMode.RTX_Mode == 0)
    dspAgc_setGain( SdrStr.SdrSetup.RX_GAIN );

}

#define TotalRXGainChar      1
const MENU_SETUP_CHAR RXGainChar[]={
  {"RX Gain",         RXGainDisp, RXGainSet}
};
void RxgDispSetupMenu( void ){
  MenuSetupDispStr.TotalSelPoz = TotalRXGainChar;
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 1;
  
  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)RXGainChar;
}
//----------------------------------------------------------------------------
void MicGainDisp( char* txt ){
    sprintf( txt, ".....%3d", SdrStr.SdrSetup.MIC_GAIN );
}
void MicGainSet( int p ){
  if( p==0 ){
    p=SdrStr.ValCount;
  }

  SdrStr.SdrSetup.MIC_GAIN+=p;
  if(SdrStr.SdrSetup.MIC_GAIN>1024){
    if( p<0 )
      SdrStr.SdrSetup.MIC_GAIN = 1;
    if(p>0)
      SdrStr.SdrSetup.MIC_GAIN = 1024;
  }
  
  //Exchange.MIC_GAIN = SdrStr.SdrSetup.MIC_GAIN;
  if(SdrStr.SdrMode.RTX_Mode)
    dspAgc_setGain( SdrStr.SdrSetup.MIC_GAIN );
}

#define TotalMicGainChar      1
const MENU_SETUP_CHAR MicGainChar[]={
  {"MIC Gain",         MicGainDisp, MicGainSet}
};
void MicgDispSetupMenu( void ){
  MenuSetupDispStr.TotalSelPoz = TotalMicGainChar;
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 1;
  
  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)MicGainChar;
}
//----------------------------------------------------------------------------
void FSsbDwnDisp( char* txt ){
  sprintf( txt, "...%5d%c", SdrStr.SdrSetup.FILTER_SSB[MenuSetupDispStr.ukaz/2][0], ((MenuSetupDispStr.ukaz/2)== SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]) ? 'S':' ' );
}
void FSsbUpDisp( char* txt ){
    sprintf( txt, "...%5d%c", SdrStr.SdrSetup.FILTER_SSB[MenuSetupDispStr.ukaz/2][1], ((MenuSetupDispStr.ukaz/2)== SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]) ? 'S':' ' );
}
void FSsbDwnSet( int p ){
  if( p==0 ){
    p=SdrStr.ValCount;
  }
  
  SdrStr.SdrSetup.FILTER_SSB[*MenuSetupDispStr.SelPos/2][0] +=p;
  if(SdrStr.SdrSetup.FILTER_SSB[*MenuSetupDispStr.SelPos/2][0] >=SdrStr.SdrSetup.FILTER_SSB[*MenuSetupDispStr.SelPos/2][1]){
    if(p>0)
      SdrStr.SdrSetup.FILTER_SSB[*MenuSetupDispStr.SelPos/2][0] = SdrStr.SdrSetup.FILTER_SSB[*MenuSetupDispStr.SelPos/2][1]-1;
    if(p<0)
      SdrStr.SdrSetup.FILTER_SSB[*MenuSetupDispStr.SelPos/2][0] = 0;
  }
  LcdFilters();
 
}
void FSsbUpSet( int p ){
  if( p==0 ){
    p=SdrStr.ValCount;
  }
  
  SdrStr.SdrSetup.FILTER_SSB[*MenuSetupDispStr.SelPos/2][1] +=p;
  if(SdrStr.SdrSetup.FILTER_SSB[*MenuSetupDispStr.SelPos/2][1] >=24000){
    if(p>0)
      SdrStr.SdrSetup.FILTER_SSB[*MenuSetupDispStr.SelPos/2][1] = 24000;
    if(p<0)
      SdrStr.SdrSetup.FILTER_SSB[*MenuSetupDispStr.SelPos/2][1] = SdrStr.SdrSetup.FILTER_SSB[*MenuSetupDispStr.SelPos/2][0]+1;
  }
    if(SdrStr.SdrSetup.FILTER_SSB[*MenuSetupDispStr.SelPos/2][1] <= SdrStr.SdrSetup.FILTER_SSB[*MenuSetupDispStr.SelPos/2][0])
      SdrStr.SdrSetup.FILTER_SSB[*MenuSetupDispStr.SelPos/2][1] = SdrStr.SdrSetup.FILTER_SSB[*MenuSetupDispStr.SelPos/2][0]+1;
  LcdFilters();
}

#define TotalFScbChar      16

const MENU_SETUP_CHAR FSsbChar[]={
  {"Fil_1 Lo",         FSsbDwnDisp, FSsbDwnSet},
  {"Fil_1 Hi",         FSsbUpDisp,  FSsbUpSet},
  {"Fil_2 Lo",         FSsbDwnDisp, FSsbDwnSet},
  {"Fil_2 Hi",         FSsbUpDisp,  FSsbUpSet},
  {"Fil_3 Lo",         FSsbDwnDisp, FSsbDwnSet},
  {"Fil_3 Hi",         FSsbUpDisp,  FSsbUpSet},
  {"Fil_4 Lo",         FSsbDwnDisp, FSsbDwnSet},
  {"Fil_4 Hi",         FSsbUpDisp,  FSsbUpSet},
  {"Fil_5 Lo",         FSsbDwnDisp, FSsbDwnSet},
  {"Fil_5 Hi",         FSsbUpDisp,  FSsbUpSet},
  {"Fil_6 Lo",         FSsbDwnDisp, FSsbDwnSet},
  {"Fil_6 Hi",         FSsbUpDisp,  FSsbUpSet},
  {"Fil_7 Lo",         FSsbDwnDisp, FSsbDwnSet},
  {"Fil_7 Hi",         FSsbUpDisp,  FSsbUpSet},
  {"Fil_8 Lo",         FSsbDwnDisp, FSsbDwnSet},
  {"Fil_8 Hi",         FSsbUpDisp,  FSsbUpSet},
};
void FSsbDispSetupMenu( void ){
  MenuSetupDispStr.TotalSelPoz = TotalFScbChar;
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 1;
  
  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)FSsbChar;
}
//----------------------------------------------------------------------------


void FamfmUpDisp( char* txt ){
//    sprintf( txt, "...%5d ", SdrStr.SdrSetup.FILTER_AMFM[MenuSetupDispStr.ukaz]);
    sprintf( txt, "...%5d%c ", SdrStr.SdrSetup.FILTER_AMFM[MenuSetupDispStr.ukaz], ((MenuSetupDispStr.ukaz)== SdrStr.SdrMode.Filter_AMFM[ SdrStr.SdrMode.Band ]) ? 'S':' ');
    //sprintf( txt, "...%5d%c", SdrStr.SdrSetup.FILTER_SSB[MenuSetupDispStr.ukaz/2][0], ((MenuSetupDispStr.ukaz/2)== SdrStr.SdrMode.Filter_SSB[ SdrStr.SdrMode.Band ]) ? 'S':' ' );

}

void FamfmUpSet( int p ){
  if( p==0 ){
    p=SdrStr.ValCount;
  }
  
  SdrStr.SdrSetup.FILTER_AMFM[*MenuSetupDispStr.SelPos] +=p;
  if(SdrStr.SdrSetup.FILTER_AMFM[*MenuSetupDispStr.SelPos] >=24000){
    if(p>0)
      SdrStr.SdrSetup.FILTER_AMFM[*MenuSetupDispStr.SelPos] = 24000;
    if(p<0)
      SdrStr.SdrSetup.FILTER_AMFM[*MenuSetupDispStr.SelPos] = 0;
  }

  LcdFilters();


}

const MENU_SETUP_CHAR FamfmChar[]={
  {"Fil_1 Hi",         FamfmUpDisp,  FamfmUpSet},
  {"Fil_2 Hi",         FamfmUpDisp,  FamfmUpSet},
  {"Fil_3 Hi",         FamfmUpDisp,  FamfmUpSet},
  {"Fil_4 Hi",         FamfmUpDisp,  FamfmUpSet},
  {"Fil_5 Hi",         FamfmUpDisp,  FamfmUpSet},
  {"Fil_6 Hi",         FamfmUpDisp,  FamfmUpSet},
  {"Fil_7 Hi",         FamfmUpDisp,  FamfmUpSet},
  {"Fil_8 Hi",         FamfmUpDisp,  FamfmUpSet},
};
void FamfmDispSetupMenu( void ){
  MenuSetupDispStr.TotalSelPoz =  sizeof( FamfmChar )/sizeof(MENU_SETUP_CHAR);
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 1;
  
  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)FamfmChar;
}
//------------------------------------------------------------------------------
void EqRxDisp( char *txt ){
  sprintf( txt, "....%2d", SdrStr.SdrMode.RX_SEQ[MenuSetupDispStr.ukaz] );
}

void EqRxSet(int p){
  if( p==0 )
    p=SdrStr.ValCount;
  
  SdrStr.SdrMode.RX_SEQ[*MenuSetupDispStr.SelPos]+=p;
    
  if( SdrStr.SdrMode.RX_SEQ[*MenuSetupDispStr.SelPos] > 9 )
    SdrStr.SdrMode.RX_SEQ[*MenuSetupDispStr.SelPos] = 9;
  
  if( SdrStr.SdrMode.RX_SEQ[*MenuSetupDispStr.SelPos] < (-9) )
      SdrStr.SdrMode.RX_SEQ[*MenuSetupDispStr.SelPos] = (-9);

  //Exchange.RX_SEQ[*MenuSetupDispStr.SelPos] = SdrStr.SdrMode.RX_SEQ[*MenuSetupDispStr.SelPos];  
  
  if(SdrStr.SdrMode.RTX_Mode==0)
    Init_Equal(SdrStr.SdrMode.RX_SEQ);
}

#define TotalEqRxChar      5
const MENU_SETUP_CHAR EqRxChar[]={
  {"Eq    50 Hz",           EqRxDisp, EqRxSet},
  {"Eq   300 Hz",           EqRxDisp, EqRxSet},
  {"Eq 1.25 kHz",           EqRxDisp, EqRxSet},
  {"Eq  4.0 kHz",           EqRxDisp, EqRxSet},
  {"Eq 12.0 kHz",           EqRxDisp, EqRxSet},
};
void EqRxDispSetupMenu( void ){
  MenuSetupDispStr.TotalSelPoz = TotalEqRxChar;
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 1;
  
  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)EqRxChar;
}
//----------------------------------------------------------------------------
void EqTxDisp( char *txt ){
  sprintf( txt, "....%2d", SdrStr.SdrMode.TX_SEQ[MenuSetupDispStr.ukaz] );
}

void EqTxSet(int p){
  if( p==0 )
    p=SdrStr.ValCount;
 
  SdrStr.SdrMode.TX_SEQ[*MenuSetupDispStr.SelPos]+=p;
    
  if( SdrStr.SdrMode.TX_SEQ[*MenuSetupDispStr.SelPos] > 9 )
    SdrStr.SdrMode.TX_SEQ[*MenuSetupDispStr.SelPos] = 9;
  
  if( SdrStr.SdrMode.TX_SEQ[*MenuSetupDispStr.SelPos] < (-9) )
      SdrStr.SdrMode.TX_SEQ[*MenuSetupDispStr.SelPos] = (-9);

  //Exchange.TX_SEQ[*MenuSetupDispStr.SelPos] = SdrStr.SdrMode.TX_SEQ[*MenuSetupDispStr.SelPos];  
  
  if(SdrStr.SdrMode.RTX_Mode==1)
    Init_Equal(SdrStr.SdrMode.TX_SEQ);
}

#define TotalEqTxChar      5
const MENU_SETUP_CHAR EqTxChar[]={
  {"Eq    50 Hz",           EqTxDisp, EqTxSet},
  {"Eq   300 Hz",           EqTxDisp, EqTxSet},
  {"Eq 1.25 kHz",           EqTxDisp, EqTxSet},
  {"Eq  4.0 kHz",           EqTxDisp, EqTxSet},
  {"Eq 12.0 kHz",           EqTxDisp, EqTxSet},
};
void EqTxDispSetupMenu( void ){
  MenuSetupDispStr.TotalSelPoz = TotalEqTxChar;
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 1;
  
  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)EqTxChar;
}
//----------------------------------------------------------------------------
void EchoDelDisp( char *txt ){
 /* float msdel =( 1000.0 * (float)SdrStr.SdrMode.Echo_del ) / (float)SdrStr.SdrSetup.Sample_Rate;  // this no work !!!??
  memset( txt,0, sizeof(txt) );
  sprintf( txt, "....%6.2f", msdel );*/
  
  int  msdel = 10000*SdrStr.SdrMode.Echo_del/SdrStr.SdrSetup.Sample_Rate;
  sprintf( txt, "...%3d.%1d", msdel/10, msdel%10 );
  
  //sprintf( txt, "....%2d", SdrStr.SdrMode.Echo_del );
}
void EchoVolDisp( char *txt ){
  sprintf( txt, "....%4.2f", SdrStr.SdrMode.Echo_vol*2.0 );
}
void EchoRptDisp( char *txt ){
  sprintf( txt, "....%4.2f", SdrStr.SdrMode.Echo_rpt );
}


void EchoDelSet( int p ){
  if( p==0 ){
    p=SdrStr.ValCount;
  }

  
  SdrStr.SdrMode.Echo_del+=p;
  if( SdrStr.SdrMode.Echo_del  >= Buf_float_tmp2_size )
    SdrStr.SdrMode.Echo_del = Buf_float_tmp2_size-1;
  if(SdrStr.SdrMode.Echo_del < 1)
    SdrStr.SdrMode.Echo_del = 1;
  
  //Exchange.Echo_del = SdrStr.SdrMode.Echo_del;

}

void EchoVolSet( int p ){
  if( p==0 ){
    p=SdrStr.ValCount;
  }
  
  SdrStr.SdrMode.Echo_vol+=(p*0.005);
  if( SdrStr.SdrMode.Echo_vol  > .5 )
    SdrStr.SdrMode.Echo_vol = .5;
  if(SdrStr.SdrMode.Echo_vol < 0)
    SdrStr.SdrMode.Echo_vol = 0;
  
  //Exchange.Echo_vol = SdrStr.SdrMode.Echo_vol;

}
void EchoRptSet( int p ){
  if( p==0 ){
    p=SdrStr.ValCount;
  }

  SdrStr.SdrMode.Echo_rpt+=(p*0.01);
  if( SdrStr.SdrMode.Echo_rpt  > 1.0 )
    SdrStr.SdrMode.Echo_rpt = 1.0;
  if(SdrStr.SdrMode.Echo_rpt < 0)
    SdrStr.SdrMode.Echo_rpt = 0;
  
  //Exchange.Echo_rpt = SdrStr.SdrMode.Echo_rpt;

}

#define TotalEchoChar      3
const MENU_SETUP_CHAR EchoChar[]={
  {"Echo del",           EchoDelDisp, EchoDelSet},
  {"Echo vol",           EchoVolDisp, EchoVolSet},
  {"Echo rpt",           EchoRptDisp, EchoRptSet}
};

void EchoDispSetupMenu( void ){
  MenuSetupDispStr.TotalSelPoz = TotalEchoChar;
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 1;
  
  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)EchoChar;
}
//------------------------------------------------------------------------------
void DnrOrdDisp( char* txt ){
    sprintf( txt, "... %3d", SdrStr.SdrSetup.DNR_Ord);
}
void DnrDelDisp( char* txt ){
    sprintf( txt, "... %3d", SdrStr.SdrSetup.DNR_Del);
}
void DnrMuDisp( char* txt ){
    sprintf( txt, "... %3.6f", SdrStr.SdrSetup.DNR_Rate);
}
void DnrLeakDisp( char* txt ){
    sprintf( txt, "... %3.6f", SdrStr.SdrSetup.DNR_Leak);
}

void DnrOrdSet(int p){
  if( p==0 )
    p=SdrStr.ValCount;
  SdrStr.SdrSetup.DNR_Ord+=p;
  if( SdrStr.SdrSetup.DNR_Ord > 128 ){
    if( p>0 )
      SdrStr.SdrSetup.DNR_Ord = 128;
    else
      SdrStr.SdrSetup.DNR_Ord = 0;
  }
  DNR_filter_rst();
}
void DnrDelSet(int p){
  if( p==0 )
    p=SdrStr.ValCount;
  SdrStr.SdrSetup.DNR_Del+=p;
  if( SdrStr.SdrSetup.DNR_Del > 128 ){
    if( p>0 )
      SdrStr.SdrSetup.DNR_Del = 128;
    else
      SdrStr.SdrSetup.DNR_Del = 0;
  }
  DNR_filter_rst();
}
void DnrMuSet(int p){
  if( p==0 )
    p=SdrStr.ValCount;
  SdrStr.SdrSetup.DNR_Rate+=(p*0.000001);
  if(SdrStr.SdrSetup.DNR_Rate < 0.000001)  
    SdrStr.SdrSetup.DNR_Rate = 0.000001;  
  DNR_filter_rst();
}
void DnrLeakSet(int p){
  if( p==0 )
    p=SdrStr.ValCount;
  SdrStr.SdrSetup.DNR_Leak+=(p*0.000001);
  if(SdrStr.SdrSetup.DNR_Leak < 0.000001)  
    SdrStr.SdrSetup.DNR_Leak = 0.000001;
  DNR_filter_rst();
}
const MENU_SETUP_CHAR DnrChar[]={
  {"Ord ",         DnrOrdDisp,  DnrOrdSet},
  {"Del ",         DnrDelDisp,  DnrDelSet},
  {"Rate",         DnrMuDisp,   DnrMuSet},
  {"Leak",         DnrLeakDisp, DnrLeakSet},
};
void DnrDispSetupMenu( void ){
   MenuSetupDispStr.TotalSelPoz =  sizeof( DnrChar )/sizeof(MENU_SETUP_CHAR);
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 1;
  
  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)DnrChar;
}
//------------------------------------------------------------------------------
void MoniVolDisp( char *txt ){
  sprintf( txt, "...%d ", SdrStr.SdrMode.Moni_vol );
}

void MoniVolSet( int p ){
  if( p==0 )
    p=SdrStr.ValCount;

  SdrStr.SdrMode.Moni_vol+=p;
  if( p>0 ){
    if(SdrStr.SdrMode.Moni_vol > 100)
      SdrStr.SdrMode.Moni_vol = 100;
  }else{
    if(SdrStr.SdrMode.Moni_vol > 100)
      SdrStr.SdrMode.Moni_vol = 0;
    }
  if(SdrStr.SdrMode.RTX_Mode){
    if( SdrStr.SdrMode.Moni )
      Spi_wr ( DAC_VOLUME_A,  100-SdrStr.SdrMode.Moni_vol,  CODEC_CS_RX );//moni dB  
  }
}

const MENU_SETUP_CHAR MoniVolChar[]={
  {"Moni Vol",           MoniVolDisp,MoniVolSet},
};
void MoniVolDispSetupMenu( void ){
  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)MoniVolChar;
  MenuSetupDispStr.TotalSelPoz = sizeof( MoniVolChar )/sizeof(MENU_SETUP_CHAR);
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 1;
}
//------------------------------------------------------------------------------

void TxSetupDisp( char *txt ){
  sprintf( txt, "... %d%% ", SdrStr.SdrSetup.tx_mix );
}
void TxFMDDisp( char *txt ){
  sprintf( txt, "...%5.2f kHz", (float)(SdrStr.SdrSetup.tx_fm_dev*SdrStr.SdrSetup.Sample_Rate/SdrStr.SdrSetup.FFT_Size)*2.0/1000.0 );
}
void TxAMMDisp( char *txt ){
  sprintf( txt, "... %d%% ", SdrStr.SdrSetup.tx_am_mod );
}


void TxSetupVolSet( int p ){
  if( p==0 )
    p=SdrStr.ValCount;

  if( p>0 ){
    SdrStr.SdrSetup.tx_mix+=p;
    if(SdrStr.SdrSetup.tx_mix > 100)
      SdrStr.SdrSetup.tx_mix = 100;
  }else{
    if (((signed short)SdrStr.SdrSetup.tx_mix+p)< 0)
      SdrStr.SdrSetup.tx_mix = 0;
    else
      SdrStr.SdrSetup.tx_mix+=p;
  }
}

void TxFMDVolSet( int p ){
  if( p==0 )
    p=SdrStr.ValCount;

  if( p>0 ){
    SdrStr.SdrSetup.tx_fm_dev+=p;
    if(SdrStr.SdrSetup.tx_fm_dev > 250)
      SdrStr.SdrSetup.tx_fm_dev = 250;
  }else{
    if (((signed short)SdrStr.SdrSetup.tx_fm_dev+p)< 0)
      SdrStr.SdrSetup.tx_fm_dev = 0;
    else
      SdrStr.SdrSetup.tx_fm_dev+=p;
  }
}


void TxAMMVolSet( int p ){
  if( p==0 )
    p=SdrStr.ValCount;

  if( p>0 ){
    SdrStr.SdrSetup.tx_am_mod+=p;
    if(SdrStr.SdrSetup.tx_am_mod > 100)
      SdrStr.SdrSetup.tx_am_mod = 100;
  }else{
    if (((signed short)SdrStr.SdrSetup.tx_am_mod+p)< 0)
      SdrStr.SdrSetup.tx_am_mod = 0;
    else
      SdrStr.SdrSetup.tx_am_mod+=p;
  }
}

const MENU_SETUP_CHAR TxSetupChar[]={
  {"Mix out",          TxSetupDisp,TxSetupVolSet},
  {"FM dev ",           TxFMDDisp,TxFMDVolSet},
  {"AM mod ",           TxAMMDisp,TxAMMVolSet},
};
void TxSetupDispSetupMenu( void ){
  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)TxSetupChar;
  MenuSetupDispStr.TotalSelPoz = sizeof( TxSetupChar )/sizeof(MENU_SETUP_CHAR);
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 1;
}
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------

void TmpDisp(char *txt ){
   sprintf( txt, "...%e", SdrStr.fTmp );
  // sprintf( txt, "...%d", SdrStr.SdrSetup.SSB_Shift );

}
void TmpSet( int p ){
  if( p==0 ){
    p=SdrStr.ValCount;
  }
  /*SdrStr.fTmp+=((SdrStr.fTmp*0.01)*p);
  SdrStr.fTmp+=p;
  if( SdrStr.fTmp < 0 )
    SdrStr.fTmp =  0;*/
  
  /*SdrStr.SdrSetup.SSB_Shift+=(p*2);
  if(SdrStr.SdrSetup.SSB_Shift > 20){
    if( p>0 )
      SdrStr.SdrSetup.SSB_Shift = 20;
    else
      SdrStr.SdrSetup.SSB_Shift = 0;
  }*/
  
}
const MENU_SETUP_CHAR TmpChar[]={
  {"fTmp",           TmpDisp, TmpSet}
};
void TempDispSetupMenu( void ){
  MenuSetupDispStr.MenuSetupCharStr = (MENU_SETUP_CHAR*)TmpChar;
  MenuSetupDispStr.TotalSelPoz = sizeof( TmpChar )/sizeof(MENU_SETUP_CHAR);
  MenuSetupDispStr.SelPos = &MenuSetupSelSubPozition;
  MenuSetupDispStr.SetVar = 0;
  MenuSetupDispStr.ValEN = 1;
}
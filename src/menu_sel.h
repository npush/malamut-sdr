#ifndef __MENU_SEL_H_
#define __MENU_SEL_H_
enum{
  MODEm_MODEp_BANDm_BANDp = 0,
  FILTm_FILTp_FLITlo_FILThi,
  PRE_ATT_RF_BYPS,
  ANF_DNR_NB_NBL,
  AGC_SQL_EQRX_ECHO,
  PWR_TONE_DTONE_TUNE,
  VOX_MONI_EQTX_ECHO,

  MENUSELEND  
};
void MenuSelScan( void );
void MenuChoise( void );
void MenuChoiseLcd( void );

void MenuSel_1( void );
void MenuSel_2( void );
void MenuSel_3( void );
void MenuSel_4( void );
void MenuSel_5( void );
void MenuSel_6( void );
void MenuSel_7( void );


void BandKey( void );
void BendSet( void );

#endif

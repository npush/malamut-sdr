#ifndef __MENU_SUBSETUP_H_
#define __MENU_SUBSETUP_H_

//============================================================================
#pragma pack (1)
typedef struct _MENU_SETUP_CHAR{
  char SelName[64];
  void (*VarDispFunck)();
  void (*SetFunck)();
}MENU_SETUP_CHAR, *PMENU_SETUP_CHAR;

//==========================================================================

void SdrDispSetupMenu( void );

void IQDispSetupMenu( void );

void SpecDispSetupMenu( void );

void ClockDispSetupMenu( void );

void RxgDispSetupMenu( void );

void MicgDispSetupMenu( void );

void FSsbDispSetupMenu( void );

void FamfmDispSetupMenu( void );

void DdsDispSetupMenu( void );

void EqRxDispSetupMenu( void );

void EqTxDispSetupMenu( void );

void EchoDispSetupMenu( void );

void DnrDispSetupMenu( void );

void MoniVolDispSetupMenu( void );

void TxSetupDispSetupMenu( void );

void TempDispSetupMenu( void );

#endif
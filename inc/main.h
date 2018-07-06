#ifndef __MAIN_H_
#define __MAIN_H_



#include "pcb.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "stm32f4xx.h"
#include "lcd.h"
#include "SysTime.h"
#include "i2c.h"
#include "save.h"
#include "spi.h"
#include "CS4272dig.h"
#include "valcoder.h"
#include "ad985x.h"
#include "dds.h"
#include "Si5351.h"
#include "Smetr.h"
#include "key.h"
#include "menu_sel.h"
#include "menu_setup.h"
#include "Clock.h"
#include "adc.h"
#include "gpio.h"
#include "bpf.h"


#include "sdr.h"



#define ROTATE_LCD
#define F24

extern char text[64];
//============================================================================
//                              INTERRUPT
#define VALCODER_Pre            0
#define VALCODER_Sub            0

#define ADC_Pre                 0
#define ADC_Sub                 1

#define Timer1ms_Key_Pre        0
#define Timer1ms_Key_Sub        2


#define USB_Pre        1
#define USB_Sub        0


#define Timer1ms_SDR_Pre        2
#define Timer1ms_SDR_Sub        0

#define TXDMACodac_Pre          2
#define TXDMACodac_Sub          1

#define RXDMACodac_Pre          2
#define RXDMACodac_Sub          2

//============================================================================

//============================================================================
#define   TotalBend   25
#define   TotalFilter  8
#define   TotalMode   7

#define   LSB         0
#define   USB         1
#define   CW          2
#define   CWR         3
#define   AM          4
#define   FM          5
#define   FM2          6
//============================================================================

//============================================================================
#define MIN(a,b)        (((a) < (b)) ? (a) : (b))
#define MAX(a,b)        (((a) > (b)) ? (a) : (b))
#define min(a,b)        (((a) < (b)) ? (a) : (b))
#define max(a,b)        (((a) > (b)) ? (a) : (b))
//============================================================================


//============================================================================


#pragma pack (1)
typedef struct _SDR_MODE{
    
  unsigned char         Band;
  
  unsigned long         Freq[TotalBend];
  unsigned char         Mode[TotalBend];
  unsigned char         Filter_SSB[TotalBend];
  unsigned char         Filter_AMFM[TotalBend];
  
  
  unsigned char         ANF;
  unsigned char         DNR;
  unsigned char         NB;
  unsigned char         NB_LEVEL;
  unsigned char         ATT[TotalBend];
  unsigned char         PRE[TotalBend];
  unsigned char         BYPS;
  unsigned char         Moni;
  unsigned char         Moni_vol;

  unsigned char         AGC_RX[TotalBend];
  unsigned char         AGC_TX;

  unsigned char         TX_PWR;
  unsigned char         TX_ECHO;
  signed long           Echo_del;
  float                 Echo_vol;
  float                 Echo_rpt;
  
  unsigned char         TX_EQ;
  unsigned char         RX_EQ;
  signed char           RX_SEQ[5];
  signed char           TX_SEQ[5];

  unsigned char         RTX_Mode;//0-RX 1-TX

  unsigned char         Menu_Sel;
  /*unsigned char         Tmp;// for debug
  float                 fTmp;// for debug
  unsigned char         ValMode;//0-freq 1-coint
  signed long           ValCount;*/
  
  unsigned char crc_static;
  unsigned char crc_sum;
  
}SDR_MODE;
//============================================================================

//============================================================================
#pragma pack (1)
typedef struct _SDR_SETUP{
    
  unsigned short        FFT_Size;
  float                 FFT_Spect_beta_up;
  float                 FFT_Spect_beta_dwn;
  unsigned char         FFT_Spect_ofset;
  unsigned long         Sample_Rate;
  unsigned char         SpecScale;
  
  unsigned char         DDS_type;//0-AD9851 1-AD9852 2-AD9951
  unsigned char         DDS_mul[4];
  unsigned long         DDS_mck[4];
  float                 koeff_DDS;

  unsigned short        RX_GAIN;
  unsigned short        MIC_GAIN;

  unsigned short        AMFM_Centr;
  unsigned short        FILTER_SSB[TotalFilter][2];
  unsigned short        FILTER_AMFM[TotalFilter];
  
  unsigned char         DNR_Ord;
  unsigned char         DNR_Del;
  float                 DNR_Rate;
  float                 DNR_Leak;
    
  float                 rx_mag[TotalBend];
  float                 rx_ph[TotalBend];

  float                 tx_mag[TotalBend];
  float                 tx_ph[TotalBend];
 
  unsigned char         tx_mix;//100%
  unsigned char         tx_fm_dev;//
  unsigned char         tx_am_mod;//

  unsigned char crc_static;
  unsigned char crc_sum;
}SDR_SETUP;
//============================================================================

//============================================================================
#pragma pack (1)
typedef struct _SDRSTR{
 
  SDR_MODE              SdrMode;
  SDR_SETUP             SdrSetup;
  
  unsigned char         Spectr[256];
  unsigned char         Spectr_avg[256];
  
  unsigned char         TX_Tone;
  float                 Smetr;

  unsigned char         FmPll_Init;
  
  unsigned char         ValMode;//0-freq 1-coint
  signed long           ValCount;

  unsigned char         Tmp;// for debug
  float                 fTmp;// for debug
  
}SDRSTR;
//============================================================================

//============================================================================
extern __IO uint8_t KEY_Done[ TotalKey ];
extern __IO uint8_t KEY_Current[ TotalKey ];
//============================================================================

//============================================================================
extern const unsigned long BandTable[TotalBend][2];
//============================================================================

//============================================================================
extern unsigned long adc_REV , adc_FWD, adc_VLT;
extern unsigned long  PTT_SW1, PTT_SW2;
extern float adc_delta, adc_delta_swr;
extern COMPLEX_short adc_buf[100];
//============================================================================

extern volatile unsigned long time_tic;

extern unsigned char SaveFreq;
extern unsigned char SaveSdrMode;
//extern SDR_MODE SdrMode;
extern unsigned char SaveSdrSetup;
//extern SDR_SETUP SdrSetup;
extern SDRSTR SdrStr;
extern unsigned char MenuSetup;
extern unsigned char WaterfallON;

extern unsigned short Waterfall_gamma[256];





//SDR
#include "arm_math.h"
#include "arm_const_structs.h"
#include "arm_common_tables.h"
#include "CS4272_i2s.h"
#include "PrepareBuf.h"
#include "bandpass.h"
#include "dspAgc.h"
#include "rx_sdr.h"
#include "tx_sdr.h"
#include "fm_demod.h"
#include "dnr.h"
#include "anf.h"
#include "nb.h"
#include "echo.h"
#include "equalizer.h"

#include "lmadf.h"

extern float dspGain;

extern unsigned char need_next;


extern arm_cfft_radix4_instance_f32  cfft_instance;
extern arm_cfft_instance_f32*        CFFT_init_struct;


extern __IO COMPLEX_long *pADC;
extern __IO COMPLEX_long *pDAC;
extern __IO COMPLEX_long *pIN;
extern __IO COMPLEX_long *pOUT;

extern COMPLEX_long ADC_buff[FFT_FILTER_SIZE];
extern COMPLEX_long DAC_buff[FFT_FILTER_SIZE];

extern COMPLEX_float cfft_buffer[FFT_FILTER_SIZE]@"CCM_RAM";       // внутренни буффер для БПФ
extern COMPLEX_float cfft_buffer_old[FFT_FILTER_SIZE/2]@"CCM_RAM"; // внутренни буффер для БПФ старые данные
extern COMPLEX_float buffer_old[40]@"CCM_RAM";                     // для смещения спектра SHIFT (48k/2048 = 23.4375Hz*40 = 937.5Hz)
extern COMPLEX_float FIR_coef[FFT_FILTER_SIZE]@"CCM_RAM";          // Filter
extern float Buf_float_tmp[FFT_FILTER_SIZE/2];                       // Аудио данные для AGC  
#define Buf_float_tmp2_size     FFT_FILTER_SIZE*4
extern float Buf_float_tmp2[Buf_float_tmp2_size];                    // RX - spectr_waterfall / TX - echo


#define SpectrWS

#ifdef SpectrWS
  extern COMPLEX_float Tmp_Pwr[2048];
  extern float WindowCoefs_for_Spectrum[2048];
#endif
  
  

#endif

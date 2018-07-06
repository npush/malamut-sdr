#ifndef __ADC_H_
#define __ADC_H_

void ADC_init( void );

typedef enum {
  PTT_DWN = 0, PTT_UP,

  PTT_0,  PTT_1,  PTT_2,  PTT_3,  PTT_4,  PTT_6,  PTT_7,  PTT_8,  PTT_9,
  
  PTT_Z, PTT_R, PTT_A, PTT_B, PTT_C, PTT_D,
  
  PTT_P1, PTT_P2, PTT_P3, PTT_P4,
  
  PTT_NO = 255,
}PTT_SW;
void PTT_SW_Scan( void );
#endif
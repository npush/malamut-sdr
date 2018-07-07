#include "main.h"

//============================================================================
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) //!< IAR Compiler 
    #pragma data_alignment=4   
  #endif
#endif //USB_OTG_HS_INTERNAL_DMA_ENABLED 
   
__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;
//============================================================================

#include "tjpgd.h"



static const  unsigned char g_myData[] = {
  #include "../boot1.pic" //http://tools.garry.tv/bin2c/
};

typedef struct {
	const BYTE * jpic;
	WORD jsize;
	WORD joffset;
} IODEV;
static  JDEC jd;		// TJDEC decompression object
static  IODEV iodev;	// Identifier of the decompression session (depends on application)
static  JRESULT rc;		// Result

UINT input_func (JDEC * jd, BYTE * buff, UINT ndata) {
	//IODEV * dev = (IODEV *) jd->device;
IODEV * dev = &iodev;

	ndata = dev->jsize - dev->joffset  > ndata ? ndata : dev->jsize - dev->joffset;

	if(buff) memcpy(buff, dev->jpic + dev->joffset, ndata);

	dev->joffset += ndata;

	return ndata;
}


UINT output_func(JDEC * jd, void * bitmap, JRECT * rect) {

	WORD * bmp = (WORD *) bitmap;

	WORD x;
	WORD y;
	WORD d;

#if 1
	WORD h = rect->right - rect->left + 1;
	WORD w = rect->bottom - rect->top + 1;

        
                  TM_ILI9341_SendCommand(ILI9341_MAC);
        #ifdef ROTATE_LCD
          TM_ILI9341_SendData(0x8C);
        #else
          TM_ILI9341_SendData(0x4C);
        #endif
        TM_ILI9341_SetCursorPosition(rect->top, rect->left,rect->top+w-1, rect->left + h-1);
        //TM_ILI9341_SetCursorPosition( rect->left,rect->top, rect->left + h,rect->top+w);
        ILI9341_WRX_RESET;
        ILI9341_CS_RESET;
        TM_SPI_Send(ILI9341_SPI, ILI9341_GRAM);
        ILI9341_WRX_SET;

         for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++){ 
              d = bmp[x * h + y];
              TM_SPI_Send(ILI9341_SPI, *(((unsigned char*)&d)+1));
              TM_SPI_Send(ILI9341_SPI, *(((unsigned char*)&d)+0));
            }
         }
        ILI9341_CS_SET;	
        TM_ILI9341_SendCommand(ILI9341_MAC);
      #ifdef ROTATE_LCD
        TM_ILI9341_SendData(0xA8);
      #else
        TM_ILI9341_SendData(0x68);
      #endif    
        
        
    /*for(y = 0; y < h; y++) {
		//LCD_SetLine(rect->top, rect->left + y);
		//for(x = 0; x < w; x++) LCD_WriteLine(bmp[x * h + y]);

		for(x = 0; x < w; x++){ 
                  TM_ILI9341_DrawPixel(rect->left + y, rect->top+x,  bmp[x * h + y] );
                }
	}*/


#else

	WORD i = 0;

	for(y = rect->top; y <= rect->bottom; y++) {
		for(x = rect->left; x <= rect->right; x++) {
			LCD_SetPoint(y, x, bmp[i++]);
		}
	}

#endif

	return 1;	// Continue to decompress
}


#define SCALE	0	/* Output scaling 0:1/1, 1:1/2, 2:1/4 or 3:1/8 */

BYTE jdwork[3100];


//============================================================================

const unsigned long BandTable[TotalBend][2] = {
  {1810000,    2000000},// 10
  {3500000,    3800000},
  {7000000,    7200000},
  {10100000,  10150000},
  {14000000,  14350000},
  {18068000,  18168000},
  {21000000,  21450000},
  {24890000,  24990000},
  {26100000,  28000000},
  {28000000,  29700000},

  {2300000,  2495000},//13
  {3200000,  3400000},
  {3900000,  4000000},
  {4750000,  5060000},
  {5730000,  6295000},
  {7200000,  7600000},
  {9400000,  9990000},
  {11600000, 12100000},
  {13500000, 13870000},
  {15100000, 15900000},
  {17480000, 17900000},
  {18900000, 19020000},
  {21450000, 21850000},
  {25600000, 26100000},
  {0, 100000000}

};
//============================================================================

//============================================================================
unsigned char SaveFreq;
unsigned long Freq_temp;

unsigned char SaveSdrMode;
unsigned char SaveSdrSetup;

SDRSTR SdrStr;

unsigned char MenuSetup;
unsigned char WaterfallON;

//============================================================================



float dspGain_tmp;
unsigned char rxtx_tmp=0xff;
char text[64];


int main(void){

  //===========================================================
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  //IWDG counter clock: 40KHz(LSI) / x = x KHz
  IWDG_SetPrescaler(IWDG_Prescaler_16);//IWDG_Prescaler_32
  // Set counter reload value to 349
  IWDG_SetReload(1000);//10ms
  IWDG_ReloadCounter();
  //IWDG_Enable();
  //===========================================================
  MenuSetup = 0;
  WaterfallON = 1;
  
  
  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );

  SysTime_init();
  Key_init();
  Gpio_init();
  Lcd_init();
   //=======================================================       
  iodev.jpic = g_myData;//PIC88;
  iodev.jsize = sizeof(g_myData);
  iodev.joffset = 0;

  rc = jd_prepare(&jd, input_func, jdwork, sizeof(jdwork), &iodev);
  if(rc) {
          TM_ILI9341_Fill(GREEN);
          for(;;);
  }

  rc = jd_decomp(&jd, output_func, SCALE);
  if(rc) {
          TM_ILI9341_Fill(RED);
          for(;;);
  }
  delay_ms(500);
  TM_ILI9341_Fill(BLACK);
  //=======================================================
  
  Lcd_Spectr_init( 0 );
  for( int i=20;i<220; ++i ){
    TM_ILI9341_DrawLine( 258, i ,259,i,Waterfall_gamma[i+20]);
    TM_ILI9341_DrawLine( 318, i ,319,i,Waterfall_gamma[i+20]);
  }
  TM_ILI9341_DrawLine( 0, 19 ,319,19, WHITE);
  TM_ILI9341_DrawLine( 0, 219 ,319,219, WHITE);
  
  i2c_ee_init();
  EEP_Load();
  if( SdrStr.Tmp )
    SdrStr.Tmp = 0;
  SdrStr.SdrMode.BYPS = 0;

  //=======================================================
  CS4272dig_init();
  CodecTXPwrSet();
  Valcoder_init();
  BPF_init();
  DDS_init();
  DDS_freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ]*2 );
  ADC_init();
  LcdMode();
  LcdFilters();
  MenuChoiseLcd();
  Lcd_Pre();
  Lcd_Att();
  Lcd_Anf();
  Lcd_Dnr();
  Lcd_Nb();
  Lcd_Agc();
  Lcd_Moni();
  Lcd_Echo();
  Lcd_Eq();
  Lcd_RfPwr();
  S_meter_line();
  Clock_init();


  SDR_init();

  //AF_MUTE_OFF(); 
  
  PreSet();
  AttSet();  
  
  
  

    USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS 
            USB_OTG_HS_CORE_ID,
#else            
            USB_OTG_FS_CORE_ID,
#endif  
            &USR_desc, 
            &USBD_CDC_cb, 
            &USR_cb);

  
  
  while(1){
   
    IWDG_ReloadCounter();
    

    
    if( rxtx_tmp != SdrStr.SdrMode.RTX_Mode ){
      if(SdrStr.SdrMode.RTX_Mode){
        
        CodecTXPwrSet();
        
        if( SdrStr.SdrMode.Moni )
         Spi_wr ( DAC_VOLUME_A,  100-SdrStr.SdrMode.Moni_vol,  CODEC_CS_RX );//moni dB
        else
         Spi_wr ( DAC_VOLUME_A,  0x80,  CODEC_CS_RX );//mute
        
        Lcd_Agc();
        TM_ILI9341_Puts(190, 0, "TX" , &TM_Font_11x18, RED, BLACK);
        if(MenuSetup == 0){
          TM_ILI9341_DrawFilledRectangleFast( 0,20, 256, 80, BLACK );
        }
        SWR_meter_draw();
      }else{
        
        Spi_wr ( DAC_VOLUME_A,  0,  CODEC_CS_RX );//0dB
        
        Lcd_Agc();
        
        TM_ILI9341_Puts(190, 0, "RX" , &TM_Font_11x18, YELLOW, BLACK);
        if(MenuSetup == 0){
          TM_ILI9341_DrawFilledRectangleFast( 0,20, 256, 80, BLACK );
        }
        
      }
      rxtx_tmp = SdrStr.SdrMode.RTX_Mode;
    }
    
    
    
    if( dspGain_tmp!=dspGain ){
      sprintf(text,  "%6.1f",dspGain);
      TM_ILI9341_DrawFilledRectangleFast( 200,200, 255, 218, BLACK );
      TM_ILI9341_Puts(200, 189, text, &TM_Font_11x18, BLUE, BLACK);
      dspGain_tmp = dspGain;
    }
      /*sprintf(text,  "%4d",PTT_SW1);
      TM_ILI9341_Puts(200, 189, text, &TM_Font_11x18, BLUE, BLACK);
      sprintf(text,  "%4d",PTT_SW2);
      TM_ILI9341_Puts(200-18, 189, text, &TM_Font_11x18, BLUE, BLACK);*/
    
    if(MenuSetup)
      MenuSetupScan();
    else
      MenuSelScan();
    BandKey();
    
    PTT_SW_Scan();

    

    //GPIO_WriteBit(LedCpu_PORT, LedCpu_PIN, Bit_RESET  );
    Lcd_Spectr( 0, 20, 60,  WaterfallON );//28ms@216MHz 
    //GPIO_WriteBit(LedCpu_PORT, LedCpu_PIN, Bit_SET  );

    S_meter();
    if( (SdrStr.SdrMode.RTX_Mode==1)&&(MenuSetup == 0) )
      SWR_meter();
        
      
    if(SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] !=Freq_temp){
      Lcd_Freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] );
      if( SdrStr.SdrSetup.DDS_type == 3 ){
        if( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] < 56000000ul )
          Si5351_freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] * 2 );
        else{
          MIX_RX_OFF();
          Si5351_freq( SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ] * 2 );
          delay_ms(5);
          MIX_RX_ON();
        }
      }
      Freq_temp = SdrStr.SdrMode.Freq[ SdrStr.SdrMode.Band ];
    }

    Clock();
    Lcd_DCVolt();
    EEP_Save();
  }
  
}
//------------------------------------------------------------------------------

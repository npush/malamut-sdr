/**
 *  Defines for your entire project at one place
 * 
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@version 	v1.0
 *	@ide		Keil uVision 5
 *	@license	GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#ifndef TM_DEFINES_H
#define TM_DEFINES_H

#include "pcb.h"   
   
/* Put your global defines for all libraries here used in your project */
#define ILI9341_SPI                 SPI1 //OR something similar
#define ILI9341_SPI_PINS            TM_SPI_PinsPack_1
 
#define ILI9341_SPI_NOMOSI          1
 
//TM_SPI1_INT_InitPins 

#ifdef  pcbASDR_V1
  /* Change custom CS, DC and RESET pins */
  /* CS pin */
  #define ILI9341_CS_CLK                RCC_AHB1Periph_GPIOB
  #define ILI9341_CS_PORT               GPIOB
  #define ILI9341_CS_PIN                GPIO_Pin_0
  /* WRX or DC pin */
  #define ILI9341_WRX_CLK                RCC_AHB1Periph_GPIOC
  #define ILI9341_WRX_PORT                GPIOC
  #define ILI9341_WRX_PIN                GPIO_Pin_4
  /* RST pin */
  #define ILI9341_RST_CLK                RCC_AHB1Periph_GPIOC
  #define ILI9341_RST_PORT                GPIOC
  #define ILI9341_RST_PIN                GPIO_Pin_5
#else
   
  /* Change custom CS, DC and RESET pins */
  /* CS pin */
  #define ILI9341_CS_CLK                RCC_AHB1Periph_GPIOA
  #define ILI9341_CS_PORT               GPIOA
  #define ILI9341_CS_PIN                GPIO_Pin_4
  /* WRX or DC pin */
  #define ILI9341_WRX_CLK                RCC_AHB1Periph_GPIOA
  #define ILI9341_WRX_PORT                GPIOA
  #define ILI9341_WRX_PIN                GPIO_Pin_6
  /* RST pin */
  #define ILI9341_RST_CLK                RCC_AHB1Periph_GPIOC
  #define ILI9341_RST_PORT                GPIOC
  #define ILI9341_RST_PIN                GPIO_Pin_5
#endif   
   
#endif

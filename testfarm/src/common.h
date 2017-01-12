//////////////////////////////////////////////////////////////////////////////////
// Company:		T.S.Labolatory Coporation
// Engineer:	
// 
// Create Date:   	Jul/18/2013
// Project Name: 	Checker4
// Module Name:   	common.h
// Tool versions: 	MPLAB v8.91
//				Microchip XC16
// Description: 
//
// Dependencies: 
//
//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _COMMON_H
#define _COMMON_H

#include <p24Exxxx.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <delay.h>
#include <math.h>
#include "typedefs.h"

#define FCY 40000000UL

#define Firm_Version	"0000"						//!< @brief ファームバージョン

#define BUSY		(_RG8)

#define ABORT		(_LATG9)
#define SLT			(_LATG7)

#define SLC			(LATC)
#define SLC0		(_LATC1)
#define SLC1		(_LATC2)
#define SLC2		(_LATC3)
#define SLC3		(_LATC4)

#define SER_A		(_LATC12)
#define SER_B		(_LATC13)
#define SER_C		(_LATC14)
#define SER_D		(_LATC15)

#define SDT0G		(LATB)
#define SDT1G		(LATD)


#define SW10		(_RE0)
#define SW11		(_RE1)
#define SW12		(_RE2)
#define SW13		(_RE3)
#define SW20		(_RE4)
#define SW21		(_RE5)
#define SW22		(_RE6)
#define SW23		(_RE7)

#define Start		(1<<1)
#define memory0set	(2<<1)
#define memory1set	(3<<1)
#define Control		(4<<1)
#define Set_adrs	(5<<1)
#define Set_data	(6<<1)
#define TWEasign	(7<<1)
#define Serialmode	(8<<1)

#define DEFINE_MAX	(10240)	//EEPROMから持ってくる最大BYTE数
//#define DEFINE_MAX	(100)		//for debug 注意 

#define FPGA_MEMORY (2048)	//FPGA メモリー長 RAMが足りなくなったら減らす

/////////////


// cpu related parameters
#define XTAL_FREQ					(8000000L)		/**< source osc frequency */
#define dFoscExt					XTAL_FREQ			/**< External Crystal or Clock Frequency (Hz) */
#define dPLL						(10)				/**< PLL ratio */

// Derived
#define dFosc						(dFoscExt*dPLL) /**< Clock frequency (Hz) */
#define dFcy						(dFosc/2)       /**< Instruction cycle frequency (Hz) */
#define dTcy						(1.0/dFcy)      /**< Instruction cycle period (sec) */

#define TIMER_INTERVAL				(156)			/**< 1ms duration(CPU動作クロック[80.0Mhz/2]/256/144) */

#define BYTE_MAX					(255)

#define INTERVAL_KEY_SCAN			(100)				//!< @brief SCAN信号切替インターバル

#define BUFMAX_STRBUF				(128)			//!< @brief strbufの最大文字数
#define BUFMAX_COMMAND				(128)
#define BUFMAX_LENGTH				(16)				//!< @brief コマンド文字数


#define EEP_USED					(0x1fff0)
#define EEP_SERIAL					(0x1fff8)

#define UDT_SERIAL					(99)

#endif /*_COMMON_H*/

//////////////////////////////////////////////////////////////////////////////////
// Company:		T.S.Labolatory Coporation
// Engineer:	
// 
// Create Date:   	Jul/18/2013
// Project Name: 	Checker4
// Module Name:   	ck4_gpio.c
// Tool versions: 	MPLAB v8.91
//				Microchip XC16
// Description: 
//
// Dependencies: 
//
// Revision: 		0.0.0.0 - Initial
// Additional Comments: 
//
//
//////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "ck4_gpio.h"
#include "ck4_timer.h"

byte rec_vsync;			//!< @brief VSYNC�M����M�t���O

BYTE now_input_1;			//!< @brief �\���L�[�̌��݂̒l
BYTE now_input_2;			//!< @brief ����L�[�̌��݂̒l
BYTE pre_input_1;			//!< @brief �\���L�[�̑O��̒l
BYTE pre_input_2;			//!< @brief ����L�[�̑O��̒l
BYTE pre_f_switch_1;		//!< @brief �\���L�[�t���O�i�X�V�O�j
BYTE pre_f_switch_2;		//!< @brief ����L�[�t���O�i�X�V�O�j

byte SwCount_Scan1;		//!< @brief SCAN1����SW�A����������
byte SwCount_Scan2;		//!< @brief SCAN2����SW�A����������
byte SwCount_Scan3;		//!< @brief SCAN3����SW�A����������
byte SwCount_ScanPw;		//!< @brief �d���X�C�b�`�̘A����������

/******************
 * GPIO�������֐�
 ******************/
void gpio_init(void)
{
	// all-ADCs not used
	ANSELA=0;
	ANSELB=0;
	ANSELC=0;
	ANSELD=0;
	ANSELE=0;
//	ANSELF=0;
	ANSELG=0;

	// GPIO���o�͕����ݒ�
	TRISA = 0xffff;
	TRISB = 0x0000;		// 
	TRISC = 0;			// 
	TRISD = 0xe000;		// 
	TRISE = 0x00ff;
	TRISF = 0xffdf;		// 
	TRISG = 0xf002;		// 

	// GPIO������
	LATB = 0;
	LATC = 0;
	LATD = 0x0;		//
	LATE = 0x0;
	LATG = 0;

	CNPUE=0xff;

//�e��s���A�T�C��    U1RX:RP113 RPINR18   U1TX:RP112 U1TX:1      �N���b�N�o��:RP118 REFCLK:110001
	RPINR18=113;
	RPOR12=0x0100;
	RPOR13=0x3100;

//REFCLK
	REFOCON=0x0200;				//20MHz for FPGA
	REFOCON=0x8200;				//20MHz for FPGA
}


/******************
 * �O�����荞�݊֐�
 ******************/
void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void)
{

}


// gpio.c END

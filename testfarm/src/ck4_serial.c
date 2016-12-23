////////////////////////////////////////////////////////////////////////////////////////////////
// Company          	: T.S.Labolatory Coporation
//
// Create Date			: 2013.07.18
// Project Name			: Checker4
// Module Name			: ck4_serial.c
// Tool versions		: MPLAB v8.91
//						  MPLAB C XC16
// Description			:
//
// Dependencies			:
//
// Additional Comments	:STA2対応からほとんどデッドコピー
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include "common.h"
#include "ck4_gvar.h"
#include "ck4_uart.h"

//元々 C0:TWE C1:SI C2:RSTB C3:START

unsigned int Ascii2Uint(char *);
void send_UDT_SerialComb (void);
void SendFpgaCommand(void);

#define	S_TWE	(SER_A)
#define	S_SI		(SER_B)
#define	S_RSTB	(SER_C)
#define	S_START	(SER_D)

#define S_LATC	SLC

enum {
	CMD_NOP = 0,			//  0 NOP
	CMD_START_COM_1,		//  1 Start1
	CMD_START_COM_2,		//  2 Start2
	CMD_START_COM_3,		//  3 Start3
	CMD_STOP_COM_1,		//  4 Stop1
	CMD_STOP_COM_2,		//  5 Stop2
	CMD_STOP_COM_3,		//  6 Stop3
	CMD_RESET_ALL,		//  7 Reset All
	CMD_SIGNAL_SEL,		//  8 Select Signal
	CMD_DUT_GROUP,		//  9 Select DUT Groups
	CMD_D_DUT_NUM,		// 10 Disable DUT #
	CMD_DIS_DUT_GRP128,	// 11 Disable 128 DUT group   1 -  128
	CMD_DIS_DUT_GRP256,	// 12 Disable 128 DUT group 129 -  256
	CMD_DIS_DUT_GRP384,	// 13 Disable 128 DUT group 257 -  384
	CMD_DIS_DUT_GRP512,	// 14 Disable 128 DUT group 385 -  512
	CMD_DIS_DUT_GRP640,	// 15 Disable 128 DUT group 513 -  640
	CMD_DIS_DUT_GRP768,	// 16 Disable 128 DUT group 641 -  768
	CMD_DIS_DUT_GRP896,	// 17 Disable 128 DUT group 769 -  896
	CMD_DIS_DUT_GRP1024,	// 18 Disable 128 DUT group 897 - 1024
	CMD_ACT_SW,			// 19 Activate SW
	
	CMD_MAX
};
#define DUMMY_MID					1
#define DUMMY_END					2
#define DUMMY_STOP3				4

// Send FPGA Command
word m_waCmdFPGA[CMD_MAX] = {
				0x0000,		// NOP
				0x7D3B,		// Start1
				0x7D1D,		// Start2
				0x7D4E,		// Start3
				0x7E44,		// Stop1
				0x7E22,		// Stop2
				0x7E51,		// Stop3
				0x0100,		// Reset All
				0x0208,		// Select Signal
				0x030C,		// Select Duts
				0x0400,		// Disable DUT
				0x1B00,		// Disable 128DUT group 128
				0x1B01,		// Disable 128DUT group 256
				0x1B02,		// Disable 128DUT group 384
				0x1B03,		// Disable 128DUT group 512
				0x1B04,		// Disable 128DUT group 640
				0x1B05,		// Disable 128DUT group 768
				0x1B06,		// Disable 128DUT group 896
				0x1B07,		// Disable 128DUT group 1024
				0x1F00,		// Activate SW //endo 1FF0->1F00
};
word nDUT;
word DUT_m1, bDUT, nGrpNo, wCmd, wSIG ;
word tgDut;
byte tgLoop;
WORD wDATA;
int nEndCnt;
int nStDummy;
#define wData		wDATA._word
#define wData_L		wDATA.byte0
#define wData_H		wDATA.byte1

WORD wBYTE;
#define wByte		wBYTE._word
#define wByte_L	wBYTE.byte0
#define wByte_H	wBYTE.byte1

int nEndCnt;
int nStDummy;

int serial_proc(void){

	nDUT = atoi(com_data[0]);			// DUT(Dec)
	wSIG = Ascii2Uint(com_data[1]);		// Signal(Hex)
	tgDut=0x0001;						// endo
	if(nDUT<1 || nDUT>1152 || wSIG>0x00ff ) { usart_puts("NG\r\n");	return FALSE;}
	else {
		send_UDT_SerialComb();
		usart_puts("DONE\r\n");
	}
	return 0;
}
void send_UDT_SerialComb (void){
	int i, j;
				DUT_m1 = nDUT-1;			// Dec
				nGrpNo = DUT_m1/128;		// グループNo
				bDUT   = DUT_m1%128;		// 余り
				tgLoop = bDUT/16;			// 転送ループ
				tgDut <<= (bDUT%16);		// ビットシフト回数

				// Send FPGA Command
				wData	= m_waCmdFPGA[CMD_NOP];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// NOR(NOP)

				wData	= m_waCmdFPGA[CMD_START_COM_1];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// Start1

				wData	= m_waCmdFPGA[CMD_START_COM_2];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// Start2

				wData	= m_waCmdFPGA[CMD_START_COM_3];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// Start3

				wData	= m_waCmdFPGA[CMD_RESET_ALL];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// Reset All

				wData	= m_waCmdFPGA[CMD_STOP_COM_1];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// Stop1

				wData	= m_waCmdFPGA[CMD_STOP_COM_2];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// Stop2

				wData	= m_waCmdFPGA[CMD_STOP_COM_3];
				nEndCnt	= DUMMY_STOP3;
				nStDummy	= 3;
				SendFpgaCommand();			// Stop3

				for(i = 0; i < 8; i++) {
					wCmd = m_waCmdFPGA[CMD_DIS_DUT_GRP128 + i];

					wData	= m_waCmdFPGA[CMD_NOP];
					nEndCnt	= DUMMY_END;
					nStDummy	= 3;
					SendFpgaCommand();		// NOR(NOP)

					wData	= m_waCmdFPGA[CMD_START_COM_1];
					nEndCnt	= DUMMY_END;
					nStDummy	= 3;
					SendFpgaCommand();		// Start1

					wData	= m_waCmdFPGA[CMD_START_COM_2];
					nEndCnt	= DUMMY_END;
					nStDummy	= 3;
					SendFpgaCommand();		// Start2

					wData	= m_waCmdFPGA[CMD_START_COM_3];
					nEndCnt	= DUMMY_END;
					nStDummy	= 3;
					SendFpgaCommand();		// Start3

					wData	= wCmd;
					nEndCnt	= DUMMY_MID;
					nStDummy	= 3;
					SendFpgaCommand();		// Disable 128 DUT group(command)

					// Disable 128 DUT group(Data word(1~8))
					for(j = 0; j < 8; j++){
						if(i==nGrpNo){
							if(j==tgLoop){	wData = tgDut;
							}else{			wData = 0x0000;
							}
						}else{				wData = 0x0000;
						}

						if(j < 7){	nEndCnt	= DUMMY_MID;
						}else{		nEndCnt	= DUMMY_END;
						}
						nStDummy	= 0;
						SendFpgaCommand();
					}

					wData	= m_waCmdFPGA[CMD_NOP];
					nEndCnt	= DUMMY_END;
					nStDummy	= 3;
					SendFpgaCommand();		// NOR(NOP)	

					wData	= m_waCmdFPGA[CMD_ACT_SW];
					nEndCnt	= DUMMY_END;
					nStDummy	= 3;
					SendFpgaCommand();		// Activate SW

					wData	= m_waCmdFPGA[CMD_STOP_COM_1];
					nEndCnt	= DUMMY_END;
					nStDummy	= 3;
					SendFpgaCommand();		// Stop1

					wData	= m_waCmdFPGA[CMD_STOP_COM_2];
					nEndCnt	= DUMMY_END;
					nStDummy	= 3;
					SendFpgaCommand();		// Stop2

					wData	= m_waCmdFPGA[CMD_STOP_COM_3];
					nEndCnt	= DUMMY_STOP3;
					nStDummy	= 3;
					SendFpgaCommand();		// Stop3
				}

				wData	= m_waCmdFPGA[CMD_NOP];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// NOR(NOP)

				wData	= m_waCmdFPGA[CMD_START_COM_1];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// Start1

				wData	= m_waCmdFPGA[CMD_START_COM_2];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// Start2

				wData	= m_waCmdFPGA[CMD_START_COM_3];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// Start3

				wData	= m_waCmdFPGA[CMD_SIGNAL_SEL];
				nEndCnt	= DUMMY_MID;
				nStDummy	= 3;
				SendFpgaCommand();			// Select Signals(Command)

				wData	= wSIG;
				nEndCnt	= DUMMY_END;
				nStDummy	= 0;
				SendFpgaCommand();			// Select Signals(Data word)

				wData	= m_waCmdFPGA[CMD_NOP];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// NOR(NOP)

				wData	= m_waCmdFPGA[CMD_DUT_GROUP];
				nEndCnt	= DUMMY_MID;
				nStDummy	= 3;
				SendFpgaCommand();			// Select Duts(Command)

				wData	= 0x0fff;
				nEndCnt	= DUMMY_END;
				nStDummy	= 0;
				SendFpgaCommand();			// Select Duts(Data word)

				wData	= m_waCmdFPGA[CMD_NOP];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// NOR(NOP)	

				wData	= m_waCmdFPGA[CMD_ACT_SW];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// Activate SW

				wData	= m_waCmdFPGA[CMD_STOP_COM_1];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// Stop1

				wData	= m_waCmdFPGA[CMD_STOP_COM_2];
				nEndCnt	= DUMMY_END;
				nStDummy	= 3;
				SendFpgaCommand();			// Stop2

				wData	= m_waCmdFPGA[CMD_STOP_COM_3];
				nEndCnt	= DUMMY_STOP3;
				nStDummy	= 3;
				SendFpgaCommand();			// Stop3
}

//*---------------------------------------------------------------------------
//* Function  : Send FPGA Command
//* --------------------------------------------------------------------------
//* Input		: None
//* Output	: None
//* Note		: None
//*---------------------------------------------------------------------------
void SendFpgaCommand(void)
{
//	int i;
	wByte	 = 0x8080;		// 16Bit

	S_TWE = 0;
	S_START = 0;
	S_SI = 0;

	S_TWE = 0;
	if(nStDummy){
		do{
			S_TWE = 1;
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();

			Nop();	//Nop 15
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();

			S_TWE = 0;

			Nop();//NOP 15
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
		}while(--nStDummy);
	}

	// MSB
	if(wData_H&wByte_H)	S_LATC = 0xe000;	//0x0e; START+RSTB+SI
	else				S_LATC = 0xc000;	//0x0c; START+RSTB

	wByte_H >>= 1;

	Nop();
	Nop();
	Nop();

	S_TWE = 1;		// Latch

	Nop();//NOP 8
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();

	while(1){
		if(wData_H&wByte_H)	S_LATC = 0x06000;	//0x06;RSTB+SI
		else				S_LATC = 0x04000;	//0x04;RSTB
			Nop();	//Nop 8
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();

		if(!wByte_H)
			break;

		wByte_H >>= 1;

		S_TWE = 1;		// Latch

			Nop();	//Nop3
			Nop();
			Nop();
	}
	while(1){
		if(wData_L&wByte_L)	S_LATC = 0x06000;	//0x06;RSTB+SI
		else				S_LATC = 0x04000;	//0x04;RSTB
			Nop();	//Nop 8
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();

		if(!wByte_L)
			break;

		wByte_L >>= 1;

		S_TWE = 1;		// Latch
			Nop();	//Nop3
			Nop();
			Nop();
	}

	S_TWE = 0;
	S_SI = 0;
	Nop();

	S_TWE = 0;
	if(nEndCnt){
		do{
			S_TWE = 1;
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();

			Nop();	//Nop 11
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();


			S_TWE = 0;

			Nop();	//Nop 11
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();

		}while(--nEndCnt);
	}

//	return TRUE;	
}

unsigned int Ascii2Uint(char *s)
{
	byte	i=0;
	word	temp=0;

	do
	{
		temp *= 16;

		if(s[i]>='0' && s[i]<='9'){
			temp += (s[i] - '0');
		}else if(s[i]>='A' && s[i]<='F'){
			temp += (s[i] - 55);
		}else if(s[i]>='a' && s[i]<='f'){
			temp += (s[i] - 87);
		}else{
			return 65535;	// 引数チェックでNG
		}

		++i;
	}while(s[i]!='\0');

	return temp;
}

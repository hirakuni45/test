//////////////////////////////////////////////////////////////////////////////////
// Company:		T.S.Labolatory Coporation
// Engineer:		
// 
// Create Date:   	Jul/18/2013
// Project Name: 	CK4
// Module Name:   	main.c
// Tool versions: 	MPLAB v8.91
//				Microchip XC16
// Description: 
//
// Dependencies: 
//
// Revision: 		0.0.0.0 - Initial
//                  0.0.0.1 - 品種切り替え時の初期値送出 追加
// Additional Comments: 
//
//
//////////////////////////////////////////////////////////////////////////////////
#include "ports.h"
#include "common.h"
#include "ck4_gpio.h"
#include "ck4_timer.h"
#include "ck4_uart.h"
#include "ck4_gvar.h"
#include "ck4_i2c.h"
#include "ck4_fpga.h"
#include "libpic30.h"

// Set Configuration Bit
#include "Config_24E.h"

/* 静的変数宣言 */
byte CmdFlag = 0;
byte Key_Access;		//!< @brief キー受付許可/禁止フラグ

	int stat_1;
/**
 *	@brief シリアル番号
 */


/**
 *	@brief ファームバージョン
 */
const char SOURCE_VERSION[] = Firm_Version;



/** @brief
 *	ステートマシン
 */
enum _status {
	STAT_PWUP = 0,			//!< 起動前処理
	STAT_WAIT,				//!< FPGA起動待機
	STAT_IDLE,				//!< アイドル状態
	STAT_MARK_1,				//!< スイッチ入力状態
	STAT_MARK_2,				//!< スイッチ入力状態
	STAT_MARK_3,				//!< スイッチ入力状態
	STAT_MODE_1,				//!< スイッチ入力状態
	STAT_MODE_2,				//!< スイッチ入力状態
	STAT_MODE_3,				//!< スイッチ入力状態
	STAT_MODE_4,				//!< スイッチ入力状態
	STAT_MODE_5,				//!< スイッチ入力状態
	STAT_MODE_6,				//!< スイッチ入力状態
	STAT_MODE_7,				//!< スイッチ入力状態
	STAT_FPGA,				//!< FPGAへデータ転送状態
	STAT_DEBUG,				//!< DEBUG用
	STAT_TRACK,				//!< トラッキング実行
	STAT_WAIT_SW_RELEASE,		//!< スイッチリリース待ち
	STAT_WAIT_SW_RELEASE_2,	//!< スイッチリリース待ち_2
	STAT_WAIT_TIME,			//!< 自動ポジショニングの表示待機
	STAT_FINISH,				//!< 終了処理
};
enum _CommandFlag {
	CMD_NON = 0,		//!< 遷移なし
	CMD_WRITE,		//!< 設定コマンド
	CMD_READ,			//!< 読み出しコマンド
	CMD_TRK_STA,		//!< トラッキングコマンド
	CMD_TRK_STP,		//!< トラッキングコマンド
};

/** @brief
 *	サブステートマシン
 */

/** @brief
 *	FPGAコマンド
 */

// Parameter -----------------------------------------------------------

// 擬似I2C受信用
	byte dbg_i2c = FALSE;
	byte i2c_1st = 0;
	byte i2c_2nd = 0;
// 擬似I2C受信用



byte dbg_flg = FALSE;		// for Debug



// プロトタイプ宣言
int main(void);
void loop(void);
void init(void);
byte usart_command_handler(void);
byte exec_command(char *com);
void exec_i2c_command(void);
void delay_us(int uSec);
void delay_ms(int mSec);
void var_init(void);



// グローバル変数宣言

/*********************************
 * 初期化関数
 *********************************/
void init(void)
{
	gpio_init();		// GPIO初期化
	var_init();		// 割り込み前変数初期化
	uart_init();		// uart初期化
	timer_init();		// タイマー初期化
	i2c_init();		// I2C初期化(Master Mode)

	int i;
	for(i = 0; i < 24; ++i) {
		pin_copy_tbl[i] = -1;
	}
}

void var_init(void){
	device_no=~PORTE;
	device_no=pre_device_no=((device_no>>4)&0xf)*10+(device_no&0xf);
	define_reading=0;
	eeprom_stat=0;
	eeprom_used=0;
	serial_pin[0]=0;
	serial_pin[1]=1;
	serial_pin[2]=2;
	serial_pin[3]=3;

	ID_name[0]=NULL;

	tgDut=0x0001;

}

/*********************************
 * メイン関数
 *********************************/
int main(void)
{
	byte b[4];
	byte a;

	// Configure Oscillator
	// Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
	// Fosc= 8*40/(2*2)= 80Mhz
	PLLFBD = 38;				// PLLDIV=1/40 to 160MHz
	CLKDIVbits.PLLPRE = 0;		// N1= 2
	CLKDIVbits.PLLPOST = 0;	// N2= 2

	OSCTUNbits.TUN = 23;				// FRCの周波数調整(約8MHz)

	while(OSCCONbits.LOCK != 1) {};		// Wait for PLL to lock

	// IEC all clear
	IEC0 = 0x0000;
	IEC1 = 0x0000;
	IEC2 = 0x0000;
	IEC3 = 0x0000;
	IEC4 = 0x0000;
	IEC5 = 0x0000;
//	IEC6 = 0x0000;
	IEC7 = 0x0000;
	IEC8 = 0x0000;

	_NSTDIS = 1;	// 割り込みのネスティング(0:許可，1:禁止)

	init();		// 各種初期化


//	I2C1ADD = 0x0;				// 外部から設定されたI2Cスレーブアドレス(とりあえず０)


	// マイコン起動後の待機
	__delay_ms(2500);				// 2.5s wait（FPGA起動待機）

	fpga_init();
	

//debug
//				write_eeprom(0,0xa5);
//				write_eeprom(EEP_USED,0xff);
//				write_eeprom(EEP_USED+1,0xff);
//				write_eeprom(EEP_USED+2,0xff);
//				write_eeprom(EEP_USED+3,0xff);
////
	__delay_ms(5);
	a=read_eeprom(0);
	b[0]=read_eeprom(EEP_USED);
	b[1]=read_eeprom(EEP_USED+1);
	b[2]=read_eeprom(EEP_USED+2);
	b[3]=read_eeprom(EEP_USED+3);

	if((b[3]==0xff)&&(b[2]==0xff)&&(b[1]==0xff)&&(b[0]==0xff)){
		//eeprom 無し
		eepsize=0;
		eeprom_used=0;
	}
	else{
		eepsize=(unsigned long)b[0]+((unsigned long)b[1]<<8)+((unsigned long)b[2]<<16)+((unsigned long)b[3]<<24);
		eeprom_used=1;
		get_device_info();
	}	
	b[0]=read_eeprom(EEP_SERIAL);
	b[1]=read_eeprom(EEP_SERIAL+1);
	b[2]=read_eeprom(EEP_SERIAL+2);
	b[3]=read_eeprom(EEP_SERIAL+3);
	if(b[0]>=24) serial_pin[0]=0;
	else serial_pin[0]=(int)b[0];
	if(b[1]>=24) serial_pin[1]=1;
	else serial_pin[1]=(int)b[1];
	if(b[2]>=24) serial_pin[2]=2;
	else serial_pin[2]=(int)b[2];
	if(b[3]>=24) serial_pin[3]=3;
	else serial_pin[3]=(int)b[3];


	loop();	// エンドレスループ

	return 0;
}

//*********************************
//* ループ関数
//*********************************
void loop(void)
{

	// ステートマシン
	while(1){
		usart_command_handler();
		__delay_ms(5);
//		fpga_debug();

		//DBG_1 ^= 1;

		// ウォッチドッグタイマをクリア
//		ClrWdt();


		// ステートマシン 未使用
		switch(stat_1){

			//*****************
			// 起動処理
			//*****************
			case STAT_PWUP:
				break;

			//*****************
			// 待機
			//*****************
			case STAT_WAIT:
				break;

			//*****************
			// アイドル状態
			//*****************
			case STAT_IDLE:
				break;



			//*****************
			// その他のステート
			//*****************
			default:
				stat_1 = STAT_IDLE;		// アイドルへ遷移
				break;

		}// end switch




	}// end while
}



//------------------------------------------------------------------------------------------------------------
// main.c END


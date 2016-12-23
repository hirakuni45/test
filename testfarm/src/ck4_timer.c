//////////////////////////////////////////////////////////////////////////////////
// Company:		T.S.Labolatory Coporation
// Engineer:		
// 
// Create Date:   	Jul/18/2013
// Project Name: 	Checker4
// Module Name:   	ck4_imer.c
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
#include "ck4_timer.h"
#include "ck4_gpio.h"
#include "ck4_gvar.h"
#include "ck4_i2c.h"


typedef struct {
	unsigned int counter;
	unsigned int enable;
} USERTIMER;

volatile USERTIMER usertimer[USERTIMER_LAST];

int c_device_no=0;

#define RSW_CNT	(5)


/** タイマ初期化.
 */
void timer_init(void)
{
	// タイマ1をオープン
	OpenTimer1(	T1_ON				/* Timer1 ON */
		 		& T1_IDLE_CON			/* operate during sleep */
		 		& T1_GATE_OFF			/* Timer Gate time accumulation disabled */
		 		& T1_PS_1_256			/* Prescaler 1:256 */
		 		& T1_SYNC_EXT_OFF		/* Do not synch external clk input */
		 		& T1_SOURCE_INT,		/* Internal clock source */
		 		TIMER_INTERVAL);		// 1ms interval

	// タイマ1割り込み設定
	ConfigIntTimer1(	T1_INT_PRIOR_4	/* 100 = Interrupt is priority 4 */
		 			& T1_INT_ON);		/* Interrupt Enable */
}

/** ユーザタイマ開始.
 * @param id ユーザタイマID
 * @param ms 時間(ms単位)
 */
void UserTimerStart(USERTIMER_ID id, int ms)
{
	usertimer[id].enable = 0;	// 以降の処理中にタイマ割り込みが入っても処理をしないように
	usertimer[id].counter = ms;	// 時間(ms単位)を設定
	usertimer[id].enable = 1;	// 動作開始
}

/** ユーザタイマ値取得
 * @param id ユーザタイマID
 */
word UserTimerValue(USERTIMER_ID id)
{
	return usertimer[id].counter;
}

/** ユーザタイマ停止.
 * @param id ユーザタイマID
 */
void UserTimerStop(USERTIMER_ID id)
{
	usertimer[id].enable = 0;	// カウンタを停止
}

/** ユーザタイマ１状態.
 * @param id ユーザタイマID
 * @return カウンタ状態 1:動作中,0:停止中
 */
int UserTimerStatus(USERTIMER_ID id)
{
	return usertimer[id].enable;
}

/** ユーザタイマ処理.
 *  タイマ割込みルーチンからコールされる(1msおき)
 */
void UserTimer_isr(void)
{
	int i;

	for (i = 0; i < USERTIMER_LAST; i++) {
		if (usertimer[i].enable) {
			if (usertimer[i].counter)
				usertimer[i].counter--;
			else
				usertimer[i].enable = 0;
		}
	}
}

///** ウェイト.
// * @param ms 時間(ms単位)
// */
//void wait_ms(int ms)
//{
//	 UserTimerStart(USERTIMER1, ms);
//	 while (UserTimerStatus(USERTIMER1));
//}

/** タイマ割込み処理ルーチン.
 *  周期：1ms
 */
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
	static int interval_key_scan	= INTERVAL_KEY_SCAN;	// キーセンスマトリクス周期カウンタ
	int dn;

	TMR1 = 0;			// タイマ初期化
	_T1IF = 0;		// 割り込みフラグクリア

	// 100ms周期
	if (--interval_key_scan <= 0) 
	{
		dn=~PORTE;
		dn=((dn>>4)&0xf)*10+(dn&0xf);

		if(pre_device_no!=dn){
			c_device_no++;
//			if(pre_device_no!=dn) c_device_no=0;
//			pre_device_no=dn;
			if (c_device_no>=RSW_CNT){
				if(define_reading==0){
					device_no=dn;
					get_device_info();
					pre_device_no=dn;
				}
				c_device_no=RSW_CNT;
			}
		}
		else {
			c_device_no=0;
			pre_device_no=dn;
		}

		interval_key_scan = INTERVAL_KEY_SCAN;
	}

	// ユーザタイマのカウント処理
	UserTimer_isr();
}









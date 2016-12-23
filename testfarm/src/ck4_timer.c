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


/** �^�C�}������.
 */
void timer_init(void)
{
	// �^�C�}1���I�[�v��
	OpenTimer1(	T1_ON				/* Timer1 ON */
		 		& T1_IDLE_CON			/* operate during sleep */
		 		& T1_GATE_OFF			/* Timer Gate time accumulation disabled */
		 		& T1_PS_1_256			/* Prescaler 1:256 */
		 		& T1_SYNC_EXT_OFF		/* Do not synch external clk input */
		 		& T1_SOURCE_INT,		/* Internal clock source */
		 		TIMER_INTERVAL);		// 1ms interval

	// �^�C�}1���荞�ݐݒ�
	ConfigIntTimer1(	T1_INT_PRIOR_4	/* 100 = Interrupt is priority 4 */
		 			& T1_INT_ON);		/* Interrupt Enable */
}

/** ���[�U�^�C�}�J�n.
 * @param id ���[�U�^�C�}ID
 * @param ms ����(ms�P��)
 */
void UserTimerStart(USERTIMER_ID id, int ms)
{
	usertimer[id].enable = 0;	// �ȍ~�̏������Ƀ^�C�}���荞�݂������Ă����������Ȃ��悤��
	usertimer[id].counter = ms;	// ����(ms�P��)��ݒ�
	usertimer[id].enable = 1;	// ����J�n
}

/** ���[�U�^�C�}�l�擾
 * @param id ���[�U�^�C�}ID
 */
word UserTimerValue(USERTIMER_ID id)
{
	return usertimer[id].counter;
}

/** ���[�U�^�C�}��~.
 * @param id ���[�U�^�C�}ID
 */
void UserTimerStop(USERTIMER_ID id)
{
	usertimer[id].enable = 0;	// �J�E���^���~
}

/** ���[�U�^�C�}�P���.
 * @param id ���[�U�^�C�}ID
 * @return �J�E���^��� 1:���쒆,0:��~��
 */
int UserTimerStatus(USERTIMER_ID id)
{
	return usertimer[id].enable;
}

/** ���[�U�^�C�}����.
 *  �^�C�}�����݃��[�`������R�[�������(1ms����)
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

///** �E�F�C�g.
// * @param ms ����(ms�P��)
// */
//void wait_ms(int ms)
//{
//	 UserTimerStart(USERTIMER1, ms);
//	 while (UserTimerStatus(USERTIMER1));
//}

/** �^�C�}�����ݏ������[�`��.
 *  �����F1ms
 */
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
	static int interval_key_scan	= INTERVAL_KEY_SCAN;	// �L�[�Z���X�}�g���N�X�����J�E���^
	int dn;

	TMR1 = 0;			// �^�C�}������
	_T1IF = 0;		// ���荞�݃t���O�N���A

	// 100ms����
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

	// ���[�U�^�C�}�̃J�E���g����
	UserTimer_isr();
}









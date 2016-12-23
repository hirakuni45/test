#ifndef _CK4_TIMER_H
#define _CK4_TIMER_H

#include "timer.h"

typedef enum {
	USERTIMER0 = 0,		//!< USART�ʐM�̃^�C���A�E�g
	USERTIMER1,			//!< ���g�p
	USERTIMER2,			//!< ���g�p
	USERTIMER3,			//!< ���g�p
	USERTIMER4,			//!< ���g�p
	USERTIMER5,			//!< ���g�p
	USERTIMER6,			//!< ���g�p
	USERTIMER7,			//!< ���g�p
	USERTIMER_LAST,
} USERTIMER_ID;

void timer_init(void);

void UserTimerStart(USERTIMER_ID id, int  ms);
void UserTimerStop(USERTIMER_ID id);
word UserTimerValue(USERTIMER_ID id);
int UserTimerStatus(USERTIMER_ID id);

void UserTimer_isr(void);

//void wait_ms(int ms);

#endif /*_MY_TIMER_H*/

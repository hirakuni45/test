#ifndef _CK4_TIMER_H
#define _CK4_TIMER_H

#include "timer.h"

typedef enum {
	USERTIMER0 = 0,		//!< USART通信のタイムアウト
	USERTIMER1,			//!< 未使用
	USERTIMER2,			//!< 未使用
	USERTIMER3,			//!< 未使用
	USERTIMER4,			//!< 未使用
	USERTIMER5,			//!< 未使用
	USERTIMER6,			//!< 未使用
	USERTIMER7,			//!< 未使用
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

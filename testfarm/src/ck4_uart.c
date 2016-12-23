//////////////////////////////////////////////////////////////////////////////////
// Company:		T.S.Labolatory Coporation
// Engineer:	
// 
// Create Date:   	Jul/18/2013
// Project Name: 	Checker4
// Module Name:   	ck4_uart.c
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
#include "uart.h"
#include "common.h"
#include "ck4_uart.h"

#define BAUD			(115200L)			/**< UART2�{�[���[�g */
#define BUFMAX_UART	(256)			/**< UART�����O�o�b�t�@�T�C�Y */

volatile char uart_buf[BUFMAX_UART];	/**< UART�����O�o�b�t�@ */
int ptrWR_uart_buf = 0;				/**< UART�����O�o�b�t�@�����݃|�C���^ */
int ptrRD_uart_buf = 0;				/**< UART�����O�o�b�t�@�Ǐo���|�C���^ */


void uart_init(void)
{
	// UART1�I�[�v��
	OpenUART1(	UART_EN					/* Module enable */
				& UART_IrDA_DISABLE		/* IrDA encoder and decoder disabled */
				& UART_MODE_SIMPLEX		/* UxRTS pin in Simplex mode */
				& UART_UEN_00				/* UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLK pins controlled by port latches*/
				& UART_DIS_WAKE			/* Disable Wake-up on START bit Detect during SLEEP Mode bit*/
				& UART_DIS_LOOPBACK		/* Loop back disabled*/
				& UART_DIS_ABAUD			/* Baud rate measurement disabled or completed*/
				& UART_UXRX_IDLE_ONE		/* UxRx Idle state is one */
				& UART_BRGH_SIXTEEN		/* BRG generates 16 clocks per bit period */
				& UART_NO_PAR_8BIT			/*no parity 8 bit*/
				& UART_1STOPBIT,			/*1 stop bit*/

				UART_IrDA_POL_INV_ZERO		/* IrDA encoded, UxTX Idel state is '0' */
				& UART_SYNC_BREAK_DISABLED	/* Sync break transmission disabled or completed */
				& UART_TX_ENABLE			/* Transmit enable */
				& UART_TX_BUF_NOT_FUL		/* Transmit buffer is not full */
				& UART_INT_RX_CHAR			/* Interrupt on every char received */
				& UART_ADR_DETECT_DIS		/* address detect disable */
				& UART_RX_OVERRUN_CLEAR,	/* Rx buffer Over run status bit clear */

				((dFcy/BAUD) / 16) -1);

	// UART2�����ݐݒ�
	ConfigIntUART1(
					UART_RX_INT_EN		/*Receive interrupt enabled*/
				 	& UART_RX_INT_PR6		/*Priority RX interrupt 6*/
				 	& UART_TX_INT_DIS		/*transmit interrupt disabled*/
				 	& UART_TX_INT_PR6		/*Priority TX interrupt 6*/
				);				
}

/*
 * UART2��M������
 *  �����O�o�b�t�@�Ɏ�M�f�[�^���i�[����
 */
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
	_U1RXIF = 0;		// ���荞�݃t���O�N���A

	uart_buf[ptrWR_uart_buf] = ReadUART1();

	// �����݃|�C���^����
	if (++ptrWR_uart_buf >= BUFMAX_UART)
		ptrWR_uart_buf = 0;
}

/*
 * UART�����O�o�b�t�@����1��������
 *  @return ��M�o�b�t�@��̏ꍇ��-1�A��łȂ��ꍇ�͕�����Ԃ�
 */
int getc_from_usart_buf(void)
{
	int ret_c = -1;

	if (ptrWR_uart_buf != ptrRD_uart_buf) {
		ret_c = uart_buf[ptrRD_uart_buf];
		if (++ptrRD_uart_buf >= BUFMAX_UART)
			ptrRD_uart_buf = 0;
	}

	return ret_c;
}

/** UART��M.
 * @return ��M����
 */
int usart_read(void)
{
	return ReadUART1();
}

/** UART��M.
 * @return ��M����
 */
int usart_getc(void)
{
	return getcUART1();
}

/** UART��M.
 * @param c ���M����
 */
void usart_write(int c)
{
	WriteUART1(c);
}

/** UART��M�f�[�^�L��.
 * @return 1:�f�[�^��M��,0:����M
 */
int usart_drdy(void)
{
	return DataRdyUART1();
}

/** UART�����񑗐M.
 * @param s ���M������
 */
void usart_puts(char *s)
{
	for (; *s; s++) {
		while (BusyUART1());	// ���M�����܂�wait
		usart_write(*s);
	}
}

void usart_put(char *s)
{
	for (; *s; s++) {
		while (BusyUART1());	// ���M�����܂�wait
		usart_write(*s);
	}
}



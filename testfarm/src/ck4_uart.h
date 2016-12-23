#ifndef _CK4_UART_H
#define _CK4_UART_H


void uart_init(void);
int getc_from_usart_buf(void);
int usart_read(void);
void usart_write(int c);
int usart_getc(void);
int usart_drdy(void);
void usart_puts(char *s);
void usart_put(char *s);



#endif /*_CK4_UART_H*/

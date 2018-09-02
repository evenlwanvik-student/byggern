#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

void UART_init(unsigned long clockspeed);
int UART_transmit(unsigned char c);
int UART_receive(void);

#endif

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

typedef enum {
    NODE1,
    NODE2
} node_t;

void UART_init(unsigned long clockspeed, node_t node);
void UART_transmit(unsigned char data);
unsigned char UART_receive();

#endif

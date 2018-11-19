#ifndef F_CPU
#define F_CPU 16000000UL	//This is just a macro, it has no data type.
#endif

#define BAUD 9600

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "UART_driver.h"

void UART_init(unsigned long clockspeed)
{
    printf("UART_init()...\n");
    _delay_ms(1000);

    unsigned long baud = BAUD;
    unsigned char ubrr = (clockspeed/(16*baud))-1;
    /* Set the baud rate */
    UBRR0H = (unsigned char) ubrr >> 8;
    UBRR0L = (unsigned char) ubrr;
    /* Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<USBS0) | (3<<UCSZ00);

    fdevopen(UART_transmit, UART_receive);
}

void UART_transmit(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

unsigned char UART_receive()
{
    /* Wait for data to be received */
    while ( !(UCSR0A & (1<<RXC0)) );
    /* Get and return received data from buffer */
    return UDR0;
}

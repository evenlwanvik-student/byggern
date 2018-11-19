#ifndef F_CPU
#define F_CPU 4915200UL	//This is just a macro, it has no data type.
#endif

#define BAUD 9600

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "UART_driver.h"

void UART_init(unsigned long clockspeed)
{
    unsigned long baud = BAUD;
    unsigned char ubrr = (clockspeed/(16*baud))-1;
    /* Set the baud rate */
    UBRR0H = (unsigned char)ubrr >> 8;
    UBRR0L = (unsigned char)ubrr;
    /* Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);

    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00);

    // Enable i/o stream via UART
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

unsigned char * const ext_iot_uart = 0x1900;

uint8_t UART_receive_IoT_test()
{
    //unsigned char *ext_mem_iot_data =  0x1823;
    /* Wait for data to be received */
    while ( !(UCSR0A & (1<<RXC0)) );
    /* Get and return received data from buffer */

    if (UDR0 == 0x02)  // if STX (start of text) = 0x02, we have a message
    {
        ext_iot_uart[0] = UART_receive();               // MSB length
        ext_iot_uart[1]  = UART_receive();              // LSB length
        ext_iot_uart[2] = UART_receive();               // msg type
        uint16_t length = (uint16_t)((ext_iot_uart[0] << 8) | ext_iot_uart[1]);
        if (length == 3)
        {
            for (uint8_t i = 0; i < length-1; i++)  // get payload based on length
            {
                ext_iot_uart[i+3] = UART_receive();
                printf("data = %d, ", ext_iot_uart[i+3]);
            }
            printf("\n\r");
        }

        if (UART_receive() == 0x03) // last bit.. just receive it
        {
            return 1;
        }
        return 0;
     }
}

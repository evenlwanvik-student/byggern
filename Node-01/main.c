
#ifndef F_CPU
#define F_CPU 4915200UL	//This is just a macro, it has no data type.
#endif

#include <stdio.h>
#include <avr/io.h>

#include "UART_driver.h"

void exercise1(void) {
	//UART
	UART_transmit('a');		//Returns the character next in the alphabet.
    char whatever = UART_receive();
    UART_transmit(whatever+1);
}

int main()
{
    unsigned long clockspeed = F_CPU;
    UART_init(clockspeed);
    while (1)
    {
        exercise1();
    }

    return 0;

}

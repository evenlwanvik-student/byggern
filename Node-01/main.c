
#ifndef F_CPU
#define F_CPU 4915200UL	//This is just a macro, it has no data type.
#endif

#include <stdio.h>
#include <avr/io.h>

#include "UART_driver.h"

void exercise1(unsigned char c) {
    //UART
    UART_transmit(abc); 
    // bchar whatever = UART_receive(); // this halts, bits indicating data is sent low?
    // UART_transmit(whatever+1);
}

int main()
{
    unsigned long clockspeed = F_CPU;
    UART_init(clockspeed);
	
    unsigned int charCtr = 65; // A in asci
    while (1)
    {
        exercise1(char(charCtr));
	asciCtr++;
	if (asciCtr == 90) asciCtr = 65;
    }

    return 0;

}

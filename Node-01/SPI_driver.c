#ifndef F_CPU
#define F_CPU 4915200UL	//This is just a macro, it has no data type.
#endif

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

// send/read serial data over SPI (have to send to read)
char SPI_transmission(char data){
    // Start transmission
    SPDR = data;
    // Wait for transmission to complete
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}

void SPI_init(void){
    // set SS, MOSI and SCK output, all others input
    DDRB = (1<<PB4) | (1<<PB5) | (1<<PB7);
    // Enable SPI, Master, set clock rate fck/16
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
}

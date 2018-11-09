#include <stdio.h>
#include <avr/io.h>

char SPI_transmission(char data){ //HUSK Å SKRIV TRANSMISSION PÅ ALLE WRITE/READ
    /*Start transmission*/
    SPDR = data;
    /*Wait for transmission to complete */
    while(!(SPSR & (1<<SPIF))){
        ;
    }

    return SPDR;
}

void SPI_init(void){
    /*set SS, MOSI and SCK output, all others input */
    DDRB = (1<<PB7) | (1<<PB2) | (1<<PB1) | (1<<PB0);
    /*Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);

    //PORTB |= 1<<PB4;

    //MCP2515_reset();
}

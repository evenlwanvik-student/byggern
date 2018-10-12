#include <stdio.h>
#include <avr/io.h>

char SPI_transmission (char data){ //HUSK Å SKRIV TRANSMISSION PÅ ALLE WRITE/READ
    /*Start transmission*/
    SPDR = data;
    /*Wait for transmission to complete */
    while(!(SPSR & (1<<SPIF))){
        ;
    }

    return SPDR;
}


//void SPI_SlaveInit(void)
//{
    /* Set MISO output, all others input */
//    DDR_SPI = (1<<DD_MISO);
    /* Enable SPI */
//    SPCR = (1<<SPE);
//}


char SPI_read(void)
{
    SPDR = 0x00;
    /* Wait for reception complete */
    while(!(SPSR & (1<<SPIF))){
        ;
    }
    /* Return data register */
    return SPDR;
}

void SPI_init(void){
    /*set SS, MOSI and SCK output, all others input */
    DDRB = (1<<PB4) | (1<<PB5) | (1<<PB7);
    /*Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);

    //PORTB |= 1<<PB4;

    //MCP2515_reset();
}

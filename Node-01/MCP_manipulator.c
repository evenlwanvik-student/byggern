#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "bit_macros.h"
#include "SPI_driver.h"
#include "MCP2515.h"

char MCP2515_read(ad_byte)
{
    clear_bit(PORTB, PB4);

    SPI_write(MCP_READ);
    SPI_write(ad_byte);
    char data = SPI_write(0x00);

    set_bit(PORTB,PB4);

    return data;
}

void MCP2515_write(char ad_byte, char cData)
{
    clear_bit(PORTB,PB4);

    SPI_write(MCP_WRITE);
    SPI_write(ad_byte);
    SPI_write(cData);


    set_bit(PORTB , PB4);
}

void MCP2515_request_to_send(char instr_data){
    clear_bit(PORTB, PB4);

    SPI_write(instr_data);

    set_bit(PORTB , PB4);

}

char MCP2515_read_status(){
    clear_bit(PORTB , PB4);

    SPI_write(MCP_READ_STATUS);

    char status = SPI_read();

    set_bit(PORTB , PB4);

    return status;
}

void MCP2515_bit_modify (char address, char mask, char data){
    clear_bit(PORTB,PB4);

    SPI_write(MCP_BITMOD);
    SPI_write(address);
    SPI_write(mask);
    SPI_write(data);

    set_bit(PORTB , PB4);
}

void MCP2515_reset(void){
    clear_bit(PORTB , PB4);

    SPI_write(MCP_RESET);

    set_bit(PORTB , PB4);

    _delay_ms(50);
}

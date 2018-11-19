#ifndef F_CPU
#define F_CPU 4915200UL	//This is just a macro, it has no data type.
#endif

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "bit_macros.h"
#include "SPI_driver.h"
#include "MCP2515.h"

// Read received CAN data and send to MCU via SPI
char MCP2515_read(char addr_byte)
{
    clear_bit(PORTB, PB4);

    SPI_transmission(MCP_READ);
    SPI_transmission(addr_byte);
    char data = SPI_transmission(0x00);

    set_bit(PORTB,PB4);

    return data;
}

// write CAN data from MCU to MCP2515 via SPI
void MCP2515_write(char addr_byte, char cData)
{
    clear_bit(PORTB,PB4);

    SPI_transmission(MCP_WRITE);
    SPI_transmission(addr_byte);
    SPI_transmission(cData);


    set_bit(PORTB , PB4);
}

// send command to check if we can send
void MCP2515_request_to_send(char instr_data){
    clear_bit(PORTB, PB4);

    SPI_transmission(instr_data);

    set_bit(PORTB , PB4);

}

char MCP2515_read_status(){
    clear_bit(PORTB , PB4);

    SPI_transmission(MCP_READ_STATUS);

    char status = SPI_transmission(0x00);

    set_bit(PORTB , PB4);

    return status;
}

void MCP2515_bit_modify (char address, char mask, char data){
    clear_bit(PORTB,PB4);

    SPI_transmission(MCP_BITMOD);
    SPI_transmission(address);
    SPI_transmission(mask);
    SPI_transmission(data);

    set_bit(PORTB , PB4);
}

void MCP2515_reset(void){
    clear_bit(PORTB , PB4);

    SPI_transmission(MCP_RESET);

    set_bit(PORTB , PB4);

    _delay_ms(50);
}

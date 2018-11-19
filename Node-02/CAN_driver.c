#ifndef F_CPU
#define F_CPU 16000000UL	//This is just a macro, it has no data type.
#endif

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "MCP2515.h"
#include "MCP_driver.h"
#include "SPI_driver.h"
#include "CAN_driver.h"
#include "bit_macros.h"

volatile uint8_t receive_flag = 0;

// INT2_vect is interrupt on PD0 (INT0_vect is on PD2?)
ISR(INT2_vect) {
    receive_flag = 1;
}

void CAN_init(void){
    MCP2515_reset();
    MCP2515_write(MCP_CANCTRL , MODE_NORMAL);

    MCP2515_write(MCP_CANINTE, MCP_RX_INT);

	// Disable global interrupts
	cli();
	// Interrupt on PB0 falling edge
	set_bit(EICRA, ISC21);
	clear_bit(EICRA, ISC20);
	// Enable interrupt
	set_bit(EIMSK,INT2);
	// Enable global interrupts
	sei();
}

// check if package in buffers
uint8_t CAN_int_vect(){
    if (receive_flag) {
        receive_flag = 0;
        return 1;
    }
    else return 0;
}

void CAN_send(can_msg_t* msg){
    /* Arbitration field; set ID and length */
    char id_high = msg->id >> 3;
    char id_low = msg->id << 5;
    uint8_t length = msg->length;

    MCP2515_write(MCP_TXB0SIDH, id_high);
    MCP2515_write(MCP_TXB0SIDL, id_low);
    MCP2515_write(MCP_TXB0DLC, length);

    // Set data
    for (uint8_t i = 0; i<length; i++) {
        MCP2515_write(MCP_TXB0Dm, msg->data[i]);
    }

    MCP2515_request_to_send(MCP_RTS_TX0); //+buffer*16);
}

void CAN_read(can_msg_t* msg_read){
    uint8_t id_high = MCP2515_read(MCP_RXB0SIDH);
    uint8_t id_low = MCP2515_read(MCP_RXB0SIDL);

    msg_read->length = MCP2515_read(MCP_RXB0DLC);

    id_high = id_high << 3;
    id_low = id_low >> 5;
    msg_read->id =  id_high + id_low;

    for (uint8_t i = 0; i < msg_read->length ; i++){
        msg_read->data[i] = MCP2515_read(MCP_RXB0DM + i);
    }

	MCP2515_bit_modify(MCP_CANINTF, 1, 0); // set interupt vector 1 to 0
	MCP2515_bit_modify(MCP_CANINTF, 2, 0); // set interupt vector 2 to 0
}

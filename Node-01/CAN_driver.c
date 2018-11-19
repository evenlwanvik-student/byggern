#ifndef F_CPU
#define F_CPU 4915200UL	//This is just a macro, it has no data type.
#endif

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "MCP2515.h"
#include "MCP_driver.h"
#include "SPI_driver.h"
#include "CAN_driver.h"
#include "bit_macros.h"

volatile uint8_t flag = 0;

ISR(INT0_vect) {
    flag = 1;
}

void CAN_init(void){

    // Initialize MCP2515
    MCP2515_reset();
    MCP2515_write(MCP_CANCTRL , MODE_NORMAL); // normal mode
    MCP2515_write(MCP_CANINTE, MCP_RX_INT);   // declare an interrupt for every message

  	// Disable global interrupts
  	cli();
  	// Interrupt on falling edge PD2
  	set_bit(MCUCR, ISC01);
  	clear_bit(MCUCR, ISC00);
  	// Enable interrupt on PD2
  	set_bit(GICR,INT0);
  	// Enable global interrupts
    sei();
}

// check if package in buffers
uint8_t CAN_int_vect(){
    if (flag) {
        flag = 0;
        return 1;
    }
    else return 0;
}

// // check if MCP received msg
// uint8_t CAN_transmit_complete(){
//     uint8_t flag = MCP2515_read(MCP_CANINTF);
//     if ((flag & (MCP_TX0IF)) == MCP_TX0IF){
//         return 0;
//     }
//
//     return 1;
// }

// send msg to MCP2515
void CAN_send(can_msg_t* msg){

    // set ID and length
    char id_high = msg->id >> 3;
    char id_low = msg->id << 5;
    uint8_t length = msg->length;

    MCP2515_write(MCP_TXB0SIDH, id_high);
    MCP2515_write(MCP_TXB0SIDL, id_low);
    MCP2515_write(MCP_TXB0DLC, length);

    // Set data bytes
    for (uint8_t i = 0; i<length; i++) {
        MCP2515_write(MCP_TXB0Dm + i , msg->data[i]);
    }

    MCP2515_request_to_send(MCP_RTS_TX0);
}

// read msg from MCP2515
void CAN_read(can_msg_t* msg_read){

    // read ID and length
    uint8_t id_high = MCP2515_read(MCP_RXB0SIDH);
    uint8_t id_low = MCP2515_read(MCP_RXB0SIDL);
    msg_read->length = MCP2515_read(MCP_RXB0DLC);

    id_high = id_high << 3;
    id_low = id_low >> 5;
    msg_read->id =  id_high + id_low;

    // read data
    for (uint8_t i = 0; i < msg_read->length ; i++){
        msg_read->data[i] = MCP2515_read(MCP_RXB0DM + i);
    }

    MCP2515_bit_modify(MCP_CANINTF, 1, 0); // set interupt vector 1 to 0
    MCP2515_bit_modify(MCP_CANINTF, 2, 0); // set interupt vector 2 to 0
}

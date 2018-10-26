#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "MCP2515.h"
#include "MCP_driver.h"
#include "SPI_driver.h"
#include "CAN_driver.h"
#include "bit_macros.h"

volatile uint8_t flag = 0;

ISR(INT2_vect){
	flag = 1;
	printf("flaggg");
}

void CAN_error(){

}

uint8_t CAN_transmit_complete(){
    uint8_t flag = MCP2515_read(MCP_CANINTF);
    if ((flag & (MCP_TX0IF)) == MCP_TX0IF){
        return 0;
    }

    return 1;
}

void CAN_int_vect(){

}

void CAN_send(can_msg_t* msg){

    volatile uint8_t buffer = 0;

    buffer += 1;
    if (buffer > 2)
    {
        buffer = 0;
    }

    // while (!(CAN_transmit_complete())){
    //     ;
    // }

    /* Arbitration field; set ID and length */
    char id_high = msg->id >> 3;
    char id_low = msg->id << 5;
    uint8_t length = msg->length;

    printf("id send %d\n\r", msg->id);
    //printf("length1 %d\n\r", msg->length);

    MCP2515_write(MCP_TXB0SIDH, id_high);
    MCP2515_write(MCP_TXB0SIDL, id_low);
    MCP2515_write(MCP_TXB0DLC, length);

    // Set data
    for (uint8_t i = 0; i<length; i++) {
        MCP2515_write(MCP_TXB0Dm + i , msg->data[i]);
    }

    MCP2515_request_to_send(MCP_RTS_TX0);

}

void CAN_read(can_msg_t* msg_read){
    uint8_t id_high = MCP2515_read(MCP_RXB0SIDH);
    uint8_t id_low = MCP2515_read(MCP_RXB0SIDL);

    msg_read->length = MCP2515_read(MCP_RXB0DLC);

    id_high = id_high << 3;
    id_low = id_low >> 5;
    msg_read->id =  id_high + id_low;

    printf("low read %d\n\r", id_low);
    printf("read length %d\n\r", msg_read->length);
    //printf("high %d\n\r", id_high);


    for (uint8_t i = 0; i < msg_read->length ; i++){
        msg_read->data[i] = MCP2515_read(MCP_RXB0DM + i);
    }
}


void CAN_init(void){
    MCP2515_reset();
    MCP2515_write(MCP_CANCTRL , MODE_NORMAL);

    MCP2515_write(MCP_CANINTE, MCP_RX_INT);

	// Disable global interrupts
	cli();
	// Interrupt on falling edge
	set_bit(MCUCR, ISC21);
	clear_bit(MCUCR, ISC20);
	// Enable interrupt
	set_bit(EIMSK,INT2);
	// Enable global interrupts
	sei();
}

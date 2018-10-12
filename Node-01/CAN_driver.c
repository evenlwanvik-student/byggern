
#include "MCP2515.h"
#include "MCP_manipulator.h"
#include "SPI_driver.h"
#include "CAN_driver.h"


void CAN_send(can_msg* msg){
    /* Arbitration field */
    char id_high = msg->id >> 8;
    char id_low = msg->id & (0x00FF);

    MCP2515_write(,id_high);
    MCP2515_write(,id_low);
}

can_msg_t CAN_receive(){
    set_bit(PORTB, PB4);

    can_msg_t msg = MCP2515_read();

    clear_bit(PORTB, PB4);

    return msg;
}


void CAN_init(void){
    set_bit(PORTB, PB4);

    SPI_write(MODE_LOOPBACK);

    clear_bit(PORTB, PB4);
}

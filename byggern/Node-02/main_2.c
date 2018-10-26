#define F_CPU 16000000UL	//This is just a macro, it has no data type.

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#include "UART_driver.h"
//#include "ADC_driver.h"
#include "bit_macros.h"
#include "SPI_driver.h"
#include "MCP_driver.h"
#include "MCP2515.h"
#include "CAN_driver.h"

void exercise6(){
    can_msg_t *msg1 = malloc(sizeof(can_msg_t));
    can_msg_t *msg_read1 = malloc(sizeof(can_msg_t));


    msg1->length = 8;

    for (uint8_t i = 0; i < msg1->length ; i++){
        msg1->data[i] = i;
        printf("%d\r\n",msg1->data[i]);
    }
    msg1->id = 3;    //ID skrives om til 0 av seg selv. WHY?!

    printf("ID: %d\n\r", msg1->id);

    while(1) {
        //_delay_ms(1);

        CAN_send(msg1);
        _delay_ms(1000);
        printf("hellu %d\n\r", msg1->id);
        //CAN_read(msg_read1);

        // for (uint8_t i = 0; i < msg_read1->length ; i++){
        //     printf("%d\r\n",msg_read1->data[i]);
        // }

        //char c = MCP2515_read(MCP_CANSTAT);
        //printf("CAN status = %x\n\r",c);
        //_delay_ms(1);
    }
    free(msg1);
    free(msg_read1);
}


void main(){
    unsigned long clockspeed = F_CPU;
    UART_init(clockspeed);
    SPI_init();
    CAN_init();
    exercise6();
}

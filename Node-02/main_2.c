#define F_CPU 16000000UL	//This is just a macro, it has no data type.

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#include "UART_driver.h"
#include "bit_macros.h"

// can libraries
#include "SPI_driver.h"
#include "MCP_driver.h"
#include "CAN_driver.h"
#include "MCP2515.h"

// servo include
#include "SERVO_driver.h"

// IR include
#include "ADC_driver.h"
#include "IR_driver.h"

// motor
#include "DAC_driver.h"
#include "MOTOR_driver.h"

void exercise6(){
    can_msg_t *msg_send = malloc(sizeof(can_msg_t));
    can_msg_t *msg_read = malloc(sizeof(can_msg_t));

    while(1) {

        if (CAN_int_vect()) {
            CAN_read(msg_read);
            printf("received ID: %d, pos x,y = (%d,%d)\n\n\r", msg_read->id, msg_read->data[0], msg_read->data[1]);
        }

        //CAN_read(msg_read1);

        // for (uint8_t i = 0; i < msg_read1->length ; i++){
        //     printf("%d\r\n",msg_read1->data[i]);
        // }

        //char c = MCP2515_read(MCP_CANSTAT);
        //printf("CAN status = %x\n\r",c);
        //_delay_ms(1);
    }
    free(msg_send);
    free(msg_read);
}

void exercise7(){
    can_msg_t *msg_read = malloc(sizeof(can_msg_t));

    uint8_t score = 0;

    while(1) {

        if (CAN_int_vect()) {
            CAN_read(msg_read);
            printf("\nID: %d, data: %d\n\r", msg_read->id, msg_read->data[0]);
            SERVO_set(msg_read->data[0]);

        }
        if (IR_blocked()) {
            score ++;

            printf("GOAL?! score = %d\n\r", score);
            if (score > 9) {
                printf("Game over... restarting!\n\r");
                score = 0;
            }
            _delay_ms(500);
        }

    }



    free(msg_read);
}

void exercise8() {
    can_msg_t *msg_read = malloc(sizeof(can_msg_t));
    uint8_t print_counter = 0;
    while (1) {
        if (CAN_int_vect()) {
            CAN_read(msg_read);

            SERVO_set(msg_read->data[0]);
            MOTOR_move(msg_read->data[0]);

            if (print_counter == 0) {
                //printf("\nID: %d, data: %d\n\r", msg_read->id, msg_read->data[0]);
            }
            print_counter++;
            print_counter %= 4;

        }
    }
    free(msg_read);
}

void main(){
    unsigned long clk_speed = F_CPU;
    UART_init(clk_speed);
    SPI_init();
    CAN_init();
    SERVO_init(clk_speed);
    ADC_init();
    DAC_init();
    MOTOR_init();

    //exercise6();
    //exercise7();
    exercise8();
}

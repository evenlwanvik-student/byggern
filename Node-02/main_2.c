#ifndef F_CPU
#define F_CPU 16000000UL	//This is just a macro, it has no data type.
#endif

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
#include "PID_controller.h"

#include "pingis_game.h"
#include "BUZZER_driver.h"

// void exercise6(){
//     can_msg_t *msg_send = malloc(sizeof(can_msg_t));
//     can_msg_t *msg_read = malloc(sizeof(can_msg_t));
//
//     while(1) {
//
//         if (CAN_int_vect()) {
//             CAN_read(msg_read);
//             printf("received ID: %d, pos x,y = (%d,%d)\n\n\r", msg_read->id, msg_read->data[0], msg_read->data[1]);
//         }
//
//         //CAN_read(msg_read1);
//
//         // for (uint8_t i = 0; i < msg_read1->length ; i++){
//         //     printf("%d\r\n",msg_read1->data[i]);
//         // }
//
//         //char c = MCP2515_read(MCP_CANSTAT);
//         //printf("CAN status = %x\n\r",c);
//         //_delay_ms(1);
//     }
//     free(msg_send);
//     free(msg_read);
// }
//
// void exercise7(){
//     can_msg_t *msg_read = malloc(sizeof(can_msg_t));
//
//     uint8_t score = 0;
//
//     while(1) {
//
//         if (CAN_int_vect()) {
//             CAN_read(msg_read);
//             printf("\nID: %d, data: %d\n\r", msg_read->id, msg_read->data[0]);
//             SERVO_set(msg_read->data[0]);
//
//         }
//         if (IR_blocked()) {
//             score ++;
//
//             printf("GOAL?! score = %d\n\r", score);
//             if (score > 9) {
//                 printf("Game over... restarting!\n\r");
//                 score = 0;
//             }
//             _delay_ms(500);
//         }
//
//     }
//     free(msg_read);
// }
//
// void exercise8() {
//     can_msg_t *msg_read = malloc(sizeof(can_msg_t));
//     uint8_t can_counter = 0;
//     uint16_t test_counter = 0;
//
//
//     while (1) {
//
//         if (test_counter == 0) {
//             //printf("here\n\r");
//         }
//
//         if (CAN_int_vect()) {
//             //printf("encoder w/o reset = %d\n\r", (uint16_t)MOTOR_encoder_read(0));
//             CAN_read(msg_read);
//
//             SERVO_set(msg_read->data[0]);
//
//             if (can_counter == 0) {
//
//                 if (msg_read->data[4] && 0x04)
//                 {
//                     SOLENOID_activate();
//                     //printf("activate solenoid\n\r");
//                 }
//
//                 //printf("encoder w/o reset = %d\n\r", MOTOR_encoder_read(0));
//                 //printf("\nID: %d, joystick = %d slider = %d\n\r", msg_read->id, msg_read->data[0], msg_read->data[2]);
//                 //int16_t encoder_data = MOTOR_encoder_read(0);
//             }
//             can_counter++;
//             can_counter %= 4;
//
//             PID_update_ref(msg_read->data[2]);
//         }
//     }
//     free(msg_read);
//     test_counter = (test_counter + 1)%10000;
// }

void collective_init(){
    unsigned long clk_freq = F_CPU;
    UART_init(clk_freq);
    SPI_init();
    CAN_init();
    TWI_Master_Initialise();
    SERVO_init(clk_freq);
    ADC_init();
    DAC_init();
    MOTOR_init();
    PID_init();
    SOLENOID_init();
    pingis_init();
    BUZZER_init();
}

void main(){
    collective_init();
    //exercise6();
    //exercise7();
    //exercise8();

    pingis_play();
}

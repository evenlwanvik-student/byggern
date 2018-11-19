#include <stdint.h>
#include <avr/interrupt.h>

#include "TWI_Master.h"

void DAC_init(){
    cli(); // disable interrupts
    TWI_Master_Initialise();
    sei(); // enable interupts
}

void DAC_data_transmit(uint8_t data){
    uint8_t msg[3];

    // set address
    msg[0] = 0b01011110;
    // command
    msg[1] = 0;
    // data
    msg[2] = data;

    TWI_Start_Transceiver_With_Data(msg, 3);
}

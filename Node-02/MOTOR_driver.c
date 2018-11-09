#include <math.h>
#include <avr/io.h>
#include <util/delay.h>

#include "bit_macros.h"
#include "DAC_driver.h"
#include "MOTOR_driver.h"

void motor_init(){

	// Enable motor pin + set enable
	set_bit(DDRH, PH4);
	set_bit(PORTH, PH4);

	// Direction pin to output
	set_bit(DDRH, PH1);

	// Output Enable pin (active low, !OE)
	set_bit(DDRH, PH5);

	//Selection pin: SEL
	set_bit(DDRH, PH3);

	// Set Reset pin: RST
	set_bit(DDRH, PH6);

	// Reset encoder
	MOTOR_encoder_reset();

	// // Set data bits to off (inputs by default)
	// clear_bit(DDRK, PK0);
	// clear_bit(DDRK, PK1);
	// clear_bit(DDRK, PK2);
	// clear_bit(DDRK, PK3);
	// clear_bit(DDRK, PK4);
	// clear_bit(DDRK, PK5);
	// clear_bit(DDRK, PK6);
	// clear_bit(DDRK, PK7);

}

void MOTOR_speed_set(uint8_t speed){
	DAC_data_transmit(speed);
}

void MOTOR_encoder_reset() {
	clear_bit(PORTH, PH6);
	_delay_us(200);
	set_bit(PORTH, PH6);
}

motor_dir_t MOTOR_set_direction(uint8_t joystick_pos){

    if (joystick_pos < 48) {
        clear_bit(PORTH, PH1);
        return LEFT;
    }
    else if (joystick_pos >= 51){
        set_bit(PORTH, PH1);
        return RIGHT;
    }
    else {
        return NEUTRAL;
    }
}

void MOTOR_move(uint8_t joystick_pos) {

    if (MOTOR_set_direction(joystick_pos) == LEFT){
        MOTOR_speed_set((int)(round((50-joystick_pos)*5.1)));
    }
    else if (MOTOR_set_direction(joystick_pos) == RIGHT){
        MOTOR_speed_set((int)(round((joystick_pos-50)*5.1)));

    }
    else {
        MOTOR_speed_set(0);
    }
}

#ifndef F_CPU
#define F_CPU 16000000UL	//This is just a macro, it has no data type.
#endif

#include <math.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "bit_macros.h"
#include "DAC_driver.h"
#include "MOTOR_driver.h"
#include "PID_controller.h"

void MOTOR_init(){
		// Enable motor pin + set enable
		set_bit(DDRH, PH4);
		set_bit(PORTH, PH4);

		// Direction pin to output
		set_bit(DDRH, PH1);

		// Decoder output enable pin (active low, !OE)
		set_bit(DDRH, PH5);
	  set_bit(PORTH, PH5);

		// decoder selection pin: SEL
		set_bit(DDRH, PH3);

		// decoder reset pin: !RST (set high)
		set_bit(DDRH, PH6);
		set_bit(PORTH, PH6);

		// reset encoder
		MOTOR_encoder_reset();
		// set data bits to off (inputs by default)
		clear_bit(DDRK, PK0);
		clear_bit(DDRK, PK1);
		clear_bit(DDRK, PK2);
		clear_bit(DDRK, PK3);
		clear_bit(DDRK, PK4);
		clear_bit(DDRK, PK5);
		clear_bit(DDRK, PK6);
		clear_bit(DDRK, PK7);

		MOTOR_calibrate();
}

void MOTOR_speed_set(uint8_t speed){
		DAC_data_transmit(speed);
}

// sets the direction of motor with joystick (Excercise 8 only)
motor_dir_t MOTOR_set_direction(uint8_t joystick_pos){
    if (joystick_pos < 48) {
        return LEFT;
    }
    else if (joystick_pos >= 51){
        return RIGHT;
    }
    else {
        return NEUTRAL;
    }
}
// also only used in exercise 8
void MOTOR_move(uint8_t joystick_pos) {

    if (MOTOR_set_direction(joystick_pos) == LEFT){
        //printf("Going left, Output = %d\r\n", (int)(round((50-joystick_pos)*5.1)));
        clear_bit(PORTH, PH1);
        MOTOR_speed_set((int)(round((50-joystick_pos)*5.1)));
    }
    else if (MOTOR_set_direction(joystick_pos) == RIGHT){
        //printf("Going right, Output = %d\r\n", (int)(round((joystick_pos-50)*5.1)));
        set_bit(PORTH, PH1);
        MOTOR_speed_set((int)(round((joystick_pos-50)*5.1)));
    }
    else {
        MOTOR_speed_set(0);
    }
}

void MOTOR_direction_set(motor_dir_t dir){
		if (dir == LEFT) clear_bit(PORTH, PH1);
		else if (dir == RIGHT) set_bit(PORTH, PH1);
}


void MOTOR_encoder_reset(){
		clear_bit(PORTH, PH6);
		_delay_us(100);
		set_bit(PORTH, PH6);
}

uint16_t MOTOR_encoder_read(uint8_t reset_flag){
		clear_bit(PORTH, PH5);     // !OE, enable encoder output
		clear_bit(PORTH, PH3);     // select high byte
		_delay_us(250);
		uint8_t high = PINK;      // read high byte
		set_bit(PORTH, PH3);     // select low byte
		_delay_us(250);
		uint8_t low = PINK;      // read low byte

		if (reset_flag) {
				MOTOR_encoder_reset();  // if not calibrating; reset encoder
		}
	  set_bit(PORTH, PH5);      // disable encoder output

		uint16_t encoder_data = (((uint16_t)(high << 8)) | low);

		return encoder_data;
}

// Calibrate to far left corner, loop with constant speed untill change in
// rotation = 0
void MOTOR_calibrate() {
		MOTOR_direction_set(LEFT);
		MOTOR_speed_set(70);
		int16_t cur_rot = MOTOR_encoder_read(0);
		int16_t prev_rot = cur_rot+100;
		while(prev_rot != cur_rot) {
				prev_rot = cur_rot;
				_delay_ms(60);
				cur_rot = MOTOR_encoder_read(0);
		}
		MOTOR_encoder_reset();
		MOTOR_encoder_read(0);
		PID_update_ref(0);
}

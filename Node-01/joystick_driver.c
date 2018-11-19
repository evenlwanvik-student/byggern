#ifndef F_CPU
#define F_CPU 4915200UL	//This is just a macro, it has no data type.
#endif

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>

#include "avr/io.h"
#include "joystick_driver.h"
#include "ADC_driver.h"
#include "bit_macros.h"

volatile int8_t x_pos;
volatile int8_t y_pos;
volatile joystick_position_t joystick_pos;
volatile sliders_position_t sliders;
volatile state_t control_state;

volatile int8_t x_offset;
volatile int8_t y_offset;

// counter interrupt, 256 prescaler
ISR(TIMER0_OVF_vect){
	channel_t channel;
	char data_char = get_ADC_data();
    uint8_t data = data_char;

    switch (control_state){
        case JOYSTICK_X:
            joystick_pos.x = abs(round((data-x_offset)*100/255)); // get data and scale
            control_state = JOYSTICK_Y; // set next control state
            channel = CHANNEL2;         // set next channel
            break;
        case JOYSTICK_Y:
            joystick_pos.y = abs(round((data-y_offset)*100/255));
            control_state = RIGHT_SLIDER;
            channel = CHANNEL3;
            break;
        case RIGHT_SLIDER:
            sliders.right = round(data*200/255)-100;
            control_state = LEFT_SLIDER;
            channel = CHANNEL4;
            break;
        case LEFT_SLIDER:
						sliders.left = round(data*200/255)-100;
            control_state = JOYSTICK_X;
            channel = CHANNEL1;
            break;
        default:
            printf("Not valid channel");
            return EXIT_FAILURE;
    }
    ADC_start_read(channel);

}

// calibrate when joystick in center position
void auto_calibrate(){
    ADC_start_read(CHANNEL1);
    _delay_ms(5);
    x_offset = get_ADC_data()- 127;

    ADC_start_read(CHANNEL2);
    _delay_ms(5);
    y_offset = get_ADC_data() - 127;
    //printf("x_offset = %d, y_offset = %d", x_offset, y_offset);
    _delay_ms(1000);
}

void joystick_init(int prescaler){

	  // Initialize button input ports
		clear_bit(DDRB, PB0);	//Joystick button
		set_bit(PORTB, PB0);	//Set pull-up resistor
		clear_bit(DDRB, PB1);	//Right button
		clear_bit(DDRB, PB2);	//Left button

		// calibrate joystick
    auto_calibrate();

    //control_state = JOYSTICK_X;
    ADC_start_read(CHANNEL1);

    //-------------INITIALIZE TIMER INPUT-----------------

    // Disable global interrupts
    cli();
    // enable timer overflow interrupt for Timer0
    TIMSK=(1<<TOIE0);
    // start timer0 with /256 prescaler
    if (prescaler == 256){
        TCCR0 = (1<<CS02);
    }
    else if (prescaler == 1024){
        TCCR0 = (1<<CS02) | (1<<CS00);
    }
    // Enable global interrupts again
    sei();
    //---------------------------------------------------
}

// return boolean if the button being tested is activated
int joystick_button(usb_button_t button){
	switch (button) {
			case JOYSTICKBUTTON :
					return !test_bit(PINB, PINB0);
					break;
			case LBUTTON :
					return test_bit(PINB, PINB2);
					break;
			case RBUTTON :
					return test_bit(PINB, PINB1);
					break;
			default:
					printf("Not valid button");
					return EXIT_FAILURE;
		}

}

// get joystick x and y values
joystick_position_t joystick_position_get(){
    return joystick_pos;
}

// calculate and return the direction of the joystick
joystick_direction_t joystick_direction_get() {

    uint8_t x = joystick_pos.x;
    uint8_t y = joystick_pos.y;

		if (abs(x-50) >= abs(y-50)){  // check if x > y
			if (x < 45){                // check if x "negative"
				return LEFT;
			}
			else if (x > 55){
				return RIGHT;
			}
	        else {
	            return NEUTRAL;
	        }
		}
		else{
	        if (y < 30){
				return DOWN;
			}
			else if (y > 70){
				return UP;
			}
	        else {
	            return NEUTRAL;
	        }
		}
}

// get left slider position
int slider_left_get(void){
	return sliders.left;
}
// get right slider position
int slider_right_get(void){
	return sliders.right;
}

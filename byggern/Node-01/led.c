#include <avr/io.h>
#include "led.h"
#include <avr/delay.h>

void led_init(){
	DDRC |= (1 << PC0);
	PORTC |= (1 << PC0);
}

void led_turn_on(){
	PORTC &= ~(1 << PC0);
}

void led_turn_off(){
	PORTC |= (1 << PC0);
}

void led_on_off() {
	led_turn_on();
	_delay_ms(10);

	led_turn_off();
	_delay_ms(1000);
}

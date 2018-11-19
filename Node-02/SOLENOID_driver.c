#ifndef F_CPU
#define F_CPU 16000000UL	//This is just a macro, it has no data type.
#endif

#include "bit_macros.h"
#include "solenoid_driver.h"

#include <avr/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void SOLENOID_init(){
  	// Set pin A1 to output
  	set_bit(DDRL, PL0);
  	set_bit(PORTL, PL0);
}

void SOLENOID_activate(){
  	// Set a pulse on pin A1
  	clear_bit(PORTL, PL0);
  	_delay_ms(100);
  	set_bit(PORTL, PL0);
}

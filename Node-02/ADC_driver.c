#ifndef F_CPU
#define F_CPU 16000000UL	//This is just a macro, it has no data type.
#endif

#include <stdlib.h>
#include <avr/io.h>

#include "ADC_driver.h"
#include "bit_macros.h"

static channel = 0;

void ADC_init(void){

		// Set pin A2 to input
		clear_bit(DDRF, PF2);

		// Enable ADC
		set_bit(ADCSRA, ADEN);

		// Prescaler
		set_bit(ADCSRA, ADPS0); // prescaler = 128
		set_bit(ADCSRA, ADPS1); // prescaler = 64
		// = 196 clk cycles

		// set voltage reference AVCC with external capacitor at AREF pin
		set_bit(ADMUX,REFS0);
}

uint16_t ADC_read() {
		//Read from ADC2
		set_bit(ADMUX, MUX1);

		// Start conversion
		set_bit(ADCSRA,ADSC);

		// Wait for conversion complete
		loop_until_bit_is_clear(ADCSRA,ADSC);

		uint8_t data_low = ADCL;
		uint8_t data_high = ADCH & 0x3;
		return (((uint16_t)data_low) | ((uint16_t)data_high) << 8	);
}

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
	//set_bit(ADCSRA, ADPS0); // prescaler = 128
	//set_bit(ADCSRA, ADPS1); // prescaler = 64
	set_bit(ADCSRA, ADPS0);
	set_bit(ADCSRA, ADPS1);
	//set_bit(ADCSRA, ADPS2); // prescaler = 32
	// = 224 clk cycles

	// Set voltage reference to 2.56V
	//set_bit(ADMUX,REFS1);
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


// burde vi bruke intern eller ekstern spennings referanse?
//ADCH
//ADCL

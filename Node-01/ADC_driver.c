#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "ADC_driver.h"
//#include "memory_layout.h"
#include "bit_macros.h"

volatile char* ext_ADC_ptr = 0x1400; // pointer to adress we want to write to, starting at 0x1400
volatile char ADC_data; //

ISR(INT1_vect){
	ADC_data = ext_ADC_ptr[0x00];
}

void ADC_init(void){
    printf("Initializing ADC driver...\n\r");
    _delay_ms(1000);

	//INITIALIZE INTERRUPT ON PIN PD3
	// Button input
	clear_bit(DDRD, PD3);
	set_bit(PORTD, PD3);	//Set pull-up resistor
	// Disable global interrupts
	cli();
	// Interrupt on falling edge PD3
	set_bit(MCUCR, ISC11);
	clear_bit(MCUCR, ISC10);
	// Enable interrupt on PD3
	set_bit(GICR,INT1);
	// Enable global interrupts
	sei();
}

char get_ADC_data(void){
	return ADC_data;
}

void ADC_start_read(channel_t channel){

	char data = 0x00;

	switch (channel) {
		case CHANNEL1 :
			data = 0x04;
			break;
		case CHANNEL2 :
			data = 0x05;
			break;
		case CHANNEL3 :
			data = 0x06;
			break;
		case CHANNEL4 :
			data = 0x07;
			break;
		default:
			printf("Not valid channel");
			return EXIT_FAILURE;
	}

	ext_ADC_ptr[0x00] = data;

}

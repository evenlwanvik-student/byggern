#include <avr/io.h>

#include "PWM_driver.h"
#include "bit_macros.h"

static uint32_t pwm_timer_freq;

void PWM_init(float period, unsigned long clk_freq){

	uint16_t prescaler = 8;

	// Set Fast PWM (mode 14 from table 17-2)
    TCCR1A  |=  (1 << WGM11);
    TCCR1B  |=  (1 << WGM12)
            |   (1 << WGM13);

    // Set compare output on PB6 (OC1B), set at BOTTOM (table 17-4)
    TCCR1A  |=  (1 << COM1A1);

	// set period, will receive 20ms from caller
	pwm_timer_freq = clk_freq/prescaler;
	ICR1 = 	pwm_timer_freq*period + 0.5; // -0.5 since it floors to int

	// Set prescaler top to 256
	clear_bit(TCCR1B, CS12);
	set_bit(TCCR1B, CS11);
	clear_bit(TCCR1B, CS10);

	// Set PB5 to output mode
	set_bit(DDRB, PB5);

	// uint16_t prescaler = 8;
    //
	// // Set Fast PWM (mode 14 from table 17-2)
    // TCCR4A  |=  (1 << WGM41);
    // TCCR4B  |=  (1 << WGM42)
    //         |   (1 << WGM43);
    //
    // // Set compare output on PB5 (OC1A), set at BOTTOM (table 17-4)
    // TCCR4A  |=  (1 << COM4A1);
    //
	// // set period, will receive 20ms from caller
	// pwm_timer_freq = clk_freq/prescaler;
	// ICR4 = 	pwm_timer_freq*period + 0.5; // -0.5 since it floors to int
    //
	// // Set prescaler top to 256
	// clear_bit(TCCR4B, CS42);
	// set_bit(TCCR4B, CS41);
	// clear_bit(TCCR4B, CS40);
    //
	// // Set PH4 to output mode
	// set_bit(DDRH, DDH3);
}

void PWM_width_set(float pulseswidth_sec) {
	uint16_t n_pulses = pwm_timer_freq*pulseswidth_sec + 0.5; // -0.5 since it floors to int
	OCR1A = n_pulses;

	// uint16_t n_pulses = pwm_timer_freq*pulseswidth_sec + 0.5; // -0.5 since it floors to int
	// OCR4A = n_pulses;
}

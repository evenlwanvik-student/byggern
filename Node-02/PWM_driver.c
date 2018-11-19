#include <avr/io.h>

#include "PWM_driver.h"
#include "bit_macros.h"
#include "math.h"

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
		ICR1 = 	round(pwm_timer_freq*period); 

		// Set prescaler top to 8
		clear_bit(TCCR1B, CS12);
		set_bit(TCCR1B, CS11);
		clear_bit(TCCR1B, CS10);

		// Set PB5 to output mode
		set_bit(DDRB, PB5);
}

void PWM_width_set(float pulseswidth_sec) {
		uint16_t n_pulses = round(pwm_timer_freq*pulseswidth_sec);
		OCR1A = n_pulses;
}

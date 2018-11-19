#include "SERVO_driver.h"
#include "PWM_driver.h"
#include <avr/io.h>

void SERVO_init(unsigned long clk_freq){

	float period_sec = 0.02;
	PWM_init(period_sec, clk_freq);
	PWM_width_set(0.0015); // set to center

}

// servo_val = 0-100
void SERVO_set(int servo_val){
	//servo_val = 100-servo_val;

	float min_pwm = 0.001;
	float max_pwm = 0.002;

	float servo_val_ms = ((float)servo_val)/100000.0;

	float servo_pwm = servo_val_ms + min_pwm;

	if (servo_pwm < min_pwm) {
		servo_pwm = min_pwm;
	}
	else if (servo_pwm > max_pwm) {
		servo_pwm = max_pwm;
	}

	PWM_width_set(servo_pwm);
}

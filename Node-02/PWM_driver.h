#ifndef PWM_DRIVER_H_
#define PWM_DRIVER_H_

void PWM_init(float period, unsigned long clock_freq);

void PWM_period_set(float sec);

void PWM_pulse_set(float sec);

#endif /* JOYSTICK_DRIVER_H_ */

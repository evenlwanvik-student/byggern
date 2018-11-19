#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

typedef enum {
    LEFT,
    RIGHT,
    NEUTRAL
} motor_dir_t;

void MOTOR_speed_set(uint8_t speed);
void MOTOR_init();
void MOTOR_move(uint8_t speed);
motor_dir_t MOTOR_set_direction(uint8_t joystick_pos);
void MOTOR_encoder_reset();
uint16_t MOTOR_encoder_read(uint8_t reset_flag);

#endif /* ADC_DRIVER_H_ */

#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

typedef enum{
    DEFAULT,
    HARD,
    BEASTMODE
} difficulty_t;

difficulty_t difficulty;

void PID_init();
void PID_update_ref(int16_t);
void PID_difficulty(difficulty_t difficulty);

volatile uint8_t calib_flag;

#endif /* PID_CONTROLLER_H_ _ */

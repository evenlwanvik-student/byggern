#include <stdint.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "MOTOR_driver.h"
#include "PID_controller.h"

// scaled by 100
int16_t Kp = 300;                    // 2.5 proportional constant
int16_t Ki = 500;                    // 3.0 integration constant
int16_t Kd = 40;                     // 0.5 keep derivative constant low, noise sensitive.
// scaled by 1000
int16_t dt = 0.016;                     // 0.016 execution time of loop, figure it out...
// would've thought that T = 64us, look below for explanation.. dt = 0.016 worked though..

volatile int16_t ref = 0.0;          // reference point
volatile int16_t integrator = 0.0;   // for summation of errors
volatile int16_t prev_error = 0;     // for calculating the errors rate of change

//int16_t rot_max = -8800;
//int16_t rot_min = 100;

volatile int16_t rotation = 0;       // keep track of rotation

int16_t center = 0;                  // FIND MAX AND MIN VALUE OF MOTORBOX AND DIVIDE..

int cycle_counter = 0;
int cycles = 0;

/* interrupt activated by timer 2 for a PID iteration, i.e. if we have
a 1024 prescaler, the sampling frequency of the system will be
Fs = 16000000[cycles/sec]/1024[cycles] = 15625, with period T = 64us */
ISR(TIMER2_OVF_vect)
{
    if (!calib_flag)
    {
        cycle_counter++;
        // GET MEASUREMENTS

        rotation = (int16_t)MOTOR_encoder_read(0);  // integrate the rotation
        int16_t measured = (rotation + center)/(-center/100);
        int16_t error = ref - measured;
        prev_error = error;

        integrator += error;                            // integrating error
        if (abs(integrator) > 5000) integrator = 5000;  // saturation threshold for intergrator
        int16_t derivative = error - prev_error;        // change in error between measurements

        int16_t out = round((Kp/100)*error + ((Ki/100)*integrator)*(dt/1000) - ((Kd/100)*derivative*(1000/dt)));
        int16_t abs_out = abs(out);

        // if error is negative, move left, if positive move right
        if (out < 0) MOTOR_direction_set(LEFT);
        else           MOTOR_direction_set(RIGHT);
        if (abs_out>200) abs_out = 200;            // set speed limit to 200
        MOTOR_speed_set(abs_out);                  // set the speed
    }
}

void PID_init()
{
    calib_flag = 0;
    rotation = 0;
    center = 4450;
    ref = 0;

    difficulty = DEFAULT;
    PID_update_difficulty();

    cli();                                       // Disable global interrupts
    TIMSK2=(1<<TOIE2);                           // enable timer overflow interrupt for Timer2
    TCCR2B = (1<<CS20) | (1<<CS21) | (1<<CS22);  // start timer2 with /1024 prescaler
    sei();                                       // Enable global interrupts
}

void PID_update_ref(int16_t val)
{
    ref = val;
    //printf("ref = %d\n\r", ref);
}

// choose the difficulty of the game based on PID values
void PID_update_difficulty()
{
    switch (difficulty) // values are scaled by 100
    {
        case DEFAULT:
            Kp = 100;
            Ki = 700;
            Kd = 50;
            break;
        case HARD:
            Kp = 700;
            Ki = 1000;
            Kd = 40;
            break;
        case BEASTMODE:
            Kp = 1000;
            Ki = 2000;
            Kd = 40;
            break;
    }
}

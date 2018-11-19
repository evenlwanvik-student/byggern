#ifndef F_CPU
#define F_CPU 16000000UL	//This is just a macro, it has no data type.
#endif

#include <stdint.h>
#include <util/delay.h>

#include "bit_macros.h"
#include "CAN_driver.h"
#include "SOLENOID_driver.h"
#include "IR_driver.h"
#include "PID_controller.h"
#include "MOTOR_driver.h"

can_msg_t *msg_send;
can_msg_t *msg_read;

void pingis_strike_msg()
{
    msg_send->id        = 2;
    msg_send->length    = 1;
    msg_send->data[0]   = 1; // tell node 1 that IR is blocked (strike)
}

void pingis_destructor()
{
	  free(msg_send);
    free(msg_read);
}

void pingis_init()
{
    difficulty = DEFAULT;
    msg_send = malloc(sizeof(can_msg_t));
    msg_read = malloc(sizeof(can_msg_t));
}

void pingis_play()
{
    uint8_t solenoid_counter = 0;
    while(1)
    {
        if (CAN_int_vect()) // if received CAN msg
        {
            CAN_read(msg_read);
            if (msg_read->id == 1)  // id 1 indicates a standard data message
            {
                SERVO_set(msg_read->data[0]);       // joystick data for servo
                PID_update_ref(msg_read->data[3]);  // get ref from right slider
                if ((msg_read->data[4] == 8) && solenoid_counter == 0) // if right button activated
                {
                    SOLENOID_activate();
                }
                solenoid_counter = (solenoid_counter+1)%10; // slow down the solenoid activation frequency
                if (difficulty != msg_read->data[5]) // if difficulty changed
                {
                    difficulty = msg_read->data[5];
                    PID_update_difficulty();         // update difficulty
                }
            }
            else if (msg_read->id == 2) // id 2 is a calibration CAN message
            {
                calib_flag = 1;
                printf("calibrate\r\n");
                MOTOR_calibrate();
                calib_flag = 0;
            }
        }

        // If IR is blocked, send a strike message to Node 1
        if (IR_blocked())
        {
            pingis_strike_msg();
            CAN_send(msg_send);
            //BUZZER_game_over();
            _delay_ms(100);
        }
		    _delay_ms(5);
    }

    pingis_destructor();
}

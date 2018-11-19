#ifndef F_CPU
#define F_CPU 4915200UL	//This is just a macro, it has no data type.
#endif

#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

#include "bit_macros.h"
#include "CAN_driver.h"
#include "OLED_driver.h"
#include "OLED_menu.h"
#include "joystick_driver.h"
#include "UART_driver.h"

can_msg_t *msg_send;
can_msg_t *msg_read;
unsigned char * const ext_iot_pingis = 0x1900;

void pingis_create_message()
{
		joystick_position_t joy_pos = joystick_position_get();
    msg_send->id = 1;
    msg_send->length = 6;
		msg_send->data[4] = 0; // init to 0, problems with
		msg_send->data[0] = joy_pos.x;
		msg_send->data[1] = joy_pos.y;


		// Different messages if we want to play from USB multifunction card or NET solution
		if (game_mode == USB_CARD)
		{
				msg_send->data[2] = slider_left_get();
				msg_send->data[3] = slider_right_get();
				msg_send->data[4] |= (joystick_button(LBUTTON)<<1);   // left button on bit 2
				msg_send->data[4] |= (joystick_button(RBUTTON)<<2);   // right button on bit 3
		}
		else if (game_mode == IOT)
		{
			if (UART_receive_IoT_test())
			{
					uint16_t pos = (uint16_t)( ext_iot_pingis[3]<<8 |  ext_iot_pingis[4]);
					msg_send->data[3] = (int8_t)(pos/5.0-100);
			}
		}
		_delay_ms(10);
		msg_send->data[5] = difficulty;                       // send difficulty enum
}

void pingis_destructor()
{
	  free(msg_send);
    free(msg_read);
}

void pingis_init()
{
		msg_send = malloc(sizeof(can_msg_t));
    msg_read = malloc(sizeof(can_msg_t));
}

void pingis_play()
{
		uint8_t OLED_counter = 0;
		uint8_t point_counter = 0;
		uint8_t calibrate = 1;
    while(1)
    {
				if (OLED_counter == 0) {
						OLED_navigate_menu();
				}
				OLED_counter = (OLED_counter+1)%60; // update OLED every 60 loopcycle

				// node communication only operative if in game mode
				if (game_type == PINGIS)
				{

						OLED_update_points(points);
						if (point_counter == 0) points++;      // add a point
						point_counter = (point_counter+1)%80; // give a point ever 100 loopcycle

						if (CAN_int_vect())                   //Game over when message is received from CAN
						{
								CAN_read(msg_read);
								OLED_game_over();
								point_counter = 0;
								calibrate = 1;
								msg_send->data[4] &= ~(1 << 4);        //Disable motors and solenoid
								while(CAN_int_vect()) CAN_read(msg_read);  //Uncheck CAN interrupt in case still active
						}
						pingis_create_message();                  // Create CAN message based on MF-card inputs
						CAN_send(msg_send);                       // Send created message
				}
				if (CAN_int_vect())                           // Safety check for CAN interrupt.
						CAN_read(msg_read);
				_delay_ms(5);
		}
		pingis_destructor();        //End Pingis game.
}

void pingis_calibrate_motor()   //Calibrate motor encoder msg
{
		printf("calibrate\r\n");
		msg_send->id = 2;
		msg_send->length = 1;
		msg_send->data[0] = 1;
		CAN_send(msg_send);
}


#ifndef F_CPU
#define F_CPU 4915200UL	//This is just a macro, it has no data type.
#endif

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

// generic libraries for all nodes
#include "UART_driver.h"

#include "SRAM_driver.h"
//#include "memory_layout.h"
#include "ADC_driver.h"
#include "joystick_driver.h"
#include "bit_macros.h"
#include "OLED_driver.h"
#include "OLED_menu.h"

// can libraries
#include "SPI_driver.h"
#include "MCP_driver.h"
#include "CAN_driver.h"
#include "MCP2515.h"

// snake lib
//#include "snake_game.h"

#define OLED_CMD 0x000 // 0001 0000 000 000
#define OLED_DATA 0x200 // 0001 0001 000 000
#define ADC 0x400 // 0001 0010 000 000
#define SRAM 0x800 // 0001 1000 000 000

// OLEDCMD  from 0x1000-0x11FF : 0001 0000 0000 0000 - 0001 0001 1111 1111
// OLEDDATA from 0x1200-0x13FF : 0001 0010 0000 0000 - 0001 0011 1111 1111
// ADC      from 0x1400-0x17FF : 0001 0100 0000 0000 - 0001 0111 1111 1111
// SRAM     from 0x1800-0x1FFF : 0001 1000 0000 0000 - 0001 1111 1111 1111

void memory_layout_init() {
    //printf("Initializing memory layout...\n");
    _delay_ms(1000);
    set_bit(MCUCR,SRE);		//Enable External Memory Interface. PE1 is automatically controlled as this command takes control.
    set_bit(SFIOR,XMM2);	//Remove 4 Most Significant Bits from address so that JTAG interface doesn't crash
}

// void exercise1() {
//     printf("\nhello world!\n");
// }
//
// void exercise2() {
//   	char data = 'c';
//   	volatile char* ext_ram = 0x1000;
//
//   	SRAM_test();
//     _delay_ms(1000);
//   	uint8_t i = 0;
//   	while(1) {
//     		printf("OLED_CMD %d\n\r", i++);
//     		ext_ram[OLED_CMD] = data;
//     		_delay_us(50);
//     		printf("OLED_DATA %d\n\r", i++);
//     		ext_ram[OLED_DATA] = data; //
//     		_delay_us(100);
//     		printf("ADC %d\n\r", i++);
//     		ext_ram[ADC] = data;
//     		_delay_us(200);
//     		printf("SRAM %d\n\r", i++);
//     		ext_ram[SRAM] = data;
//     		_delay_us(250);
//   	}
// }
// void exercise3(void) {
//   	joystick_position_t position;
//   	joystick_direction_t direction;
//   	while(1){
//
//     		/*------------- get/print position -------------*/
//     		position = joystick_position_get();
//     		printf("\rJoystick: \tx: %d \t\tSlider:\tleft: %d \n", position.x, slider_left_get());
//     		printf("\r\t\ty: %d \t\t\tright: %d \n\n", position.y, slider_right_get());
//
//     		/*------------- test buttons -------------*/
//     		if (joystick_button(JOYSTICKBUTTON)){
//     			printf("\rJOYSTICK BUTTON\n");
//     		}
//     		if (joystick_button(LBUTTON)){
//     			printf("\rLEFT BUTTON\n");
//     		}
//     		if (joystick_button(RBUTTON)){
//     			printf("\rRIGHT BUTTON\n");
//     		}
//
//     		/*------------- get/print position -------------*/
//     		direction = joystick_direction_get();
//     		printf("\n\rDirection: %d\n\n", direction);
//     		printf("\r- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n\n");
//     		_delay_ms(500);
//   	}
//   	//CUTOFF FREQUENCY OF FILTER = 1/(100*(10⁻9)*2000) = 5000
//   	//1. order filter -> slope = -20db
// }
//
// void exercise4(void){
//     while (1) {
//         OLED_navigate_menu();
//         _delay_ms(150);
//     }
// }
//
// void exercise5(void){
//     can_msg_t *msg1 = malloc(sizeof(can_msg_t));
//     can_msg_t *msg_read1 = malloc(sizeof(can_msg_t));
//     msg1->length = 8;
//
//     for (uint8_t i = 0; i < msg1->length ; i++){
//         msg1->data[i] = i;
//         //printf("%d\r\n",msg1->data[i]);
//     }
//     msg1->id = 3;    //ID skrives om til 0 av seg selv. WHY?!
//     printf("ID: %d\n\r", msg1->id);
//     while(1) {
//         //_delay_ms(1);
//         CAN_send(msg1);
//         _delay_ms(1000);
//         CAN_read(msg_read1);
//         for (uint8_t i = 0; i < msg_read1->length ; i++){
//             printf("%d\r\n",msg_read1->data[i]);
//         }
//     }
//     free(msg1);
//     free(msg_read1);
// }
//
// void exercise_6_7_8(void){
//
//     uint8_t can_counter = 0;
//     joystick_position_t joy_pos;
//     joystick_direction_t direction;
//     sliders_position_t slider_pos;
//
//     can_msg_t *msg_send = malloc(sizeof(can_msg_t));
//     can_msg_t *msg_read = malloc(sizeof(can_msg_t));
//
//     uint8_t id = 1;
//
//     while(1) {
//         msg_send->id = id;
//         joy_pos = joystick_position_get();
//         msg_send->length = 6;
//         msg_send->data[0] = joy_pos.x;
//         msg_send->data[1] = joy_pos.y;
//         msg_send->data[2] = slider_right_get();
//
//         // buttons on byte 3
//         msg_send->data[3] = 0;
//         msg_send->data[3] |= joystick_button(JOYSTICKBUTTON);
//         msg_send->data[3] |= joystick_button(LBUTTON) << 1;
//         msg_send->data[3] |= joystick_button(RBUTTON) << 2;
//         printf("msg_send->data[3] = %d\n\r", msg_send->data[3]);
//
//         if (CAN_int_vect()) {
//             printf("received msg");
//             CAN_read(msg_read);
//             for (uint8_t i = 0; i < msg_read->length ; i++){
//                 printf("%d\r\n",msg_read->data[i]);
//             }
//         }
//
//         //_delay_ms(1);
//         CAN_send(msg_send);
//         id = id++%10;
//         _delay_ms(5);
//
//         if (can_counter == 0)
//             //printf("sending ID: %d, pos x,y = (%d,%d), slider right = %d\n\n\r", msg_send->id, joy_pos.x, joy_pos.y, msg_send->data[2]);
//
//         can_counter++;
//         can_counter %= 5;
//
//     }
//     free(msg_send);
//     free(msg_read);
// }

void collective_init() {

    unsigned long clockspeed = F_CPU;
    int prescaler_joystick_timer = 1024;

    UART_init(clockspeed);
    memory_layout_init();
    ADC_init();
    joystick_init(prescaler_joystick_timer);
    SPI_init();
    CAN_init();
    OLED_init();
    OLED_menu_init();
    pingis_init();
}

void main()
{
    collective_init();

    //exercise1();
  	//exercise2();
  	//exercise3();
    //exercise4();
    //exercise5();
    //exercise_6_7_8();
    //main_snake();

    pingis_play();
}

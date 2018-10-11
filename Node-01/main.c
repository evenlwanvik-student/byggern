
#ifndef F_CPU
#define F_CPU 4915200UL	//This is just a macro, it has no data type.
#endif

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#include "UART_driver.h"
#include "led.h"
#include "SRAM_driver.h"
//#include "memory_layout.h"
#include "ADC_driver.h"
#include "joystick_driver.h"
#include "bit_macros.h"
#include "OLED_driver.h"
#include "OLED_menu.h"

#define OLED_CMD 0x000 // 0001 0000 000 000
#define OLED_DATA 0x200 // 0001 0001 000 000
#define ADC 0x400 // 0001 0010 000 000
#define SRAM 0x800 // 0001 1000 000 000

// OLEDCMD  from 0x1000-0x11FF : 0001 0000 0000 0000 - 0001 0001 1111 1111
// OLEDDATA from 0x1200-0x13FF : 0001 0010 0000 0000 - 0001 0011 1111 1111
// ADC      from 0x1400-0x17FF : 0001 0100 0000 0000 - 0001 0111 1111 1111
// SRAM     from 0x1800-0x1FFF : 0001 1000 0000 0000 - 0001 1111 1111 1111

void memory_layout_init() {
    printf("memory_layout_init()...\n");
    //_delay_ms(500);
    set_bit(MCUCR,SRE);		//Enable External Memory Interface. PE1 is automatically controlled as this command takes control.
    set_bit(SFIOR,XMM2);	//Remove 4 Most Significant Bits from address so that JTAG interface doesn't crash
}

/*
void exercise1(unsigned char c) {
	UART_transmit(UART_receive() + 1);		//Returns the character next in the alphabet.
	printf("\nhello world!\n");

}

void exercise2() {
	//SRAM
	char data = 'c';
	volatile char* ext_ram = 0x1000;

	SRAM_test();
	uint8_t i = 0;
	while(1) {
	//	printf("OLED_CMD %d\n", i++);
		ext_ram[OLED_CMD] = data;
		_delay_us(50);
	//	printf("OLED_DATA %d\n", i++);
		ext_ram[OLED_DATA] = data; //
		_delay_us(100);
//		printf("ADC %d\n", i++);
		ext_ram[ADC] = data;
		_delay_us(200);
//		printf("SRAM %d\n", i++);
		ext_ram[SRAM] = data;
		_delay_us(250);
	}
}
*/
void exercise3(void) {
	//Joystick
	joystick_position_t position;
	joystick_direction_t direction;
	while(1){

		/*------------- get/print position -------------*/
		position = joystick_position_get();
		printf("Joystick: \tx: %d \t\tSlider:\tleft: %d \n", position.x, slider_left_get());
		printf("\t\ty: %d \t\t\tright: %d \n\n", position.y, slider_right_get());

		/*------------- test buttons -------------*/
		if (joystick_button(JOYSTICKBUTTON)){
			printf("JOYSTICK BUTTON\n");
		}
		if (joystick_button(LBUTTON)){
			printf("LEFT BUTTON\n");
		}
		if (joystick_button(RBUTTON)){
			printf("RIGHT BUTTON\n");
		}

		/*------------- get/print position -------------*/
		direction = joystick_direction_get();
		printf("\nDirection: %d\n\n", direction);
		printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n\n");
		_delay_ms(1000);

	}
	// CUTOFF FREQUENCY OF FILTER = 1/(100*(10⁻9)*2000) = 5000
	// 1. order filter -> slope = -20db
}

void exercise4(void){
    while (1) {
        OLED_navigate_menu();
        _delay_ms(200);
    }
}

int main()
{
    unsigned long clockspeed = F_CPU;
	int prescaler_joystick_timer = 1024;

    UART_init(clockspeed);
	memory_layout_init();
	ADC_init();
	joystick_init(prescaler_joystick_timer);
    OLED_init();
    OLED_menu_init();

	//exercise1();
	//exercise2();
	//exercise3();
    exercise4();
    // while(1){
    //     OLED_clear();
    //     uint8_t i = 0;
    //     char* data = "Menu Demo\0";
    //     //printf(data);
    //     while (data[i] != '\0') {
    //         OLED_printf(data,0,0);
    //         i++;
    //     }
    //     _delay_ms(1000);
    // }

    return 0;
}

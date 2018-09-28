#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "memory_layout.h"
#include "fonts.h"
#include "OLED_driver.h"

volatile char* ext_oled_data = EXT_OLED_DATA;
volatile char* ext_oled_cmd = EXT_OLED_CMD;


volatile oled_position_t pos = {0,0};

volatile uint8_t font_size = 8;


void alt_font_size(font_enum size){
    switch (size){
        case (FONT4):
            font_size = 4;
            break;
        case (FONT5):
            font_size = 5;
            break;
        case (FONT8):
            font_size = 8;
            break;
    }
}

void write_cmd(uint8_t cmd){
    ext_oled_cmd[0] = cmd;
}

void write_data(uint8_t data){
    //ext_oled_data[0] = data;
    char * pointer = 0x1200;
    pointer[0] = data;
}

void OLED_set_adressing_mode(adressing_mode mode){
    write_cmd(0x20);
    write_cmd(mode);
}

void init_OLED()
{
    printf("OLED init()...\n");
    //_delay_ms(1000);
     write_cmd(0xae); // display off
     write_cmd(0xa1); //segment remap
     write_cmd(0xda); //common pads hardware: alternative
     write_cmd(0x12);
     write_cmd(0xc8); //common output scan direction:com63~com0
     write_cmd(0xa8); //multiplex ration mode:63
     write_cmd(0x3f);
     write_cmd(0xd5); //display divide ratio/osc. freq. mode
     write_cmd(0x80);
     write_cmd(0x81); //contrast control
     write_cmd(0x50);
     write_cmd(0xd9); //set pre-charge period
     write_cmd(0x21);
     write_cmd(0x20); //Set Memory Addressing Mode
     write_cmd(0x02);
     write_cmd(0xdb); //VCOM deselect level mode
     write_cmd(0x30);
     write_cmd(0xad); //master configuration
     write_cmd(0x00);
     write_cmd(0xa4); //out follows RAM content
     write_cmd(0xa6); //set normal display
     write_cmd(0xaf); // display on
 }

 void OLED_reset(){

 }

 void OLED_home(){

 }

// YOU ARE AT THIS FUNCTION EVEN!!!!

 void OLED_goto_line(uint8_t line){
     if (line < 7 || line < 0) {
         return 0;
     }
     else {
         pos.row = line; // update current position row
         oled_set_adressing_mode(PAGE_MODE); //
 		 write_cmd(0xB0 + line); // B0 is the page start address + line is the wanted line
 		 oled_set_adressing_mode(HORIZONTAL_MODE);
     }
 }

void OLED_goto_column(uint8_t column){

}

void OLED_clear_line(uint8_t line){

}

void OLED_pos(uint8_t row,uint8_t column){
    OLED_goto_line(row);
}

void OLED_print_char(char print_c){
    uint8_t print_char = print_c - 32;

    for (uint8_t i = 0 ; i < font_size; i++)
    {
        write_data(pgm_read_byte(&font8[print_char][i]));
        pos.col += font_size;
    }
}

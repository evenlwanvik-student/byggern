#ifndef F_CPU
#define F_CPU 4915200UL	//This is just a macro, it has no data type.
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "fonts.h"
#include "OLED_driver.h"
#include "memory_layout.h"

volatile char* ext_oled_data = EXT_OLED_DATA;
volatile char* ext_oled_cmd = EXT_OLED_CMD;

volatile uint8_t font_size = 8;
volatile oled_position_t pos = {0,0};

/* ----------------- functions not included in header ----------------- */

void write_cmd(uint8_t cmd){
    ext_oled_cmd[0] = cmd;
}

void write_data(uint8_t data){
    ext_oled_data[0] = data;
}

void OLED_set_adressing_mode(adressing_mode_t mode){
    write_cmd(0x20);
    write_cmd(mode);
}

void OLED_goto_line(uint8_t line){
    if (line < 8) {
        pos.line = line;                      // update current position
        OLED_set_adressing_mode(PAGE_MODE);
        write_cmd(0xB0 + line);               // B0 is the page start address + line is the wanted line
 		    OLED_set_adressing_mode(HORIZONTAL_MODE);
    }
}

void OLED_goto_column(uint8_t column){
    if (column < 128) {
        OLED_set_adressing_mode(PAGE_MODE);
        pos.col = column; // update current position
        /* We need to set 0-127 bit, but can only use 0-16. We'll combine it into
        two 0-16 hexadecimals to be able to create a 127bit address*/
        write_cmd(0x00 + (column & 0x0F));       // bitmask to set lower bits
        write_cmd(0x10 + ((column & 0xF0)>>4));  // bitmask to set higher bits
        OLED_set_adressing_mode(HORIZONTAL_MODE);
    }
}

// E.T. go home
void OLED_home(){
    OLED_goto_line(0);
    OLED_goto_column(0);
}

// iterate over columns in page(line), set all pixels to 0
void OLED_clear_line(uint8_t line){
    OLED_goto_pos(line, 0);
    for (uint8_t i = 0; i<128; i++) {
        write_data(0b00000000);        // clear pixels in every column
    }
    OLED_goto_line(line);              // go back to start of line
}

/* ----------------- functions included in header ----------------- */

// clear
void OLED_clear(){
    for (uint8_t i = 0; i < 8; i++) {
        OLED_clear_line(i);
    }
    OLED_home();
}

void OLED_init()
{
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
    OLED_set_adressing_mode(HORIZONTAL_MODE); //Set Memory Addressing Mode
    write_cmd(0x02);
    write_cmd(0xdb); //VCOM deselect level mode
    write_cmd(0x30);
    write_cmd(0xad); //master configuration
    write_cmd(0x00);
    write_cmd(0xa4); //out follows RAM content
    write_cmd(0xa6); //set normal display
    write_cmd(0xaf); // display on

    write_cmd(0x40); //set start line address

    OLED_clear();
}

void OLED_goto_pos(uint8_t line, uint8_t column){
    OLED_goto_line(line);
    OLED_goto_column(column);
}

// print single char
void OLED_print_char(char data){
    uint8_t print_c = data - 32;

    switch (font_size) {
        case 4:
            for (uint8_t i = 0 ; i < font_size; i++)
            {
                write_data(pgm_read_byte(&font4[print_c][i]));
                pos.col += font_size;
            }
            break;
        case 5:
            for (uint8_t i = 0 ; i < font_size; i++)
            {
                write_data(pgm_read_byte(&font5[print_c][i]));
                pos.col += font_size;
            }
            break;
        case 8:
            for (uint8_t i = 0 ; i < font_size; i++)
            {
                write_data(pgm_read_byte(&font8[print_c][i]));
                pos.col += font_size;
            }
            break;
    }
}

void OLED_printf(char* data, uint8_t line, uint8_t col) {
    int i = 0;
    OLED_goto_pos(line, col);
    while (data[i] != '\0') {
        OLED_print_char(data[i]);
        i++;
    }
}

void OLED_print_value(uint8_t val, uint8_t line, uint8_t col) {
    char out_string[3];
    sprintf(out_string, "%d", val);
    OLED_printf(&out_string, line, col);
}

uint8_t OLED_fontsize_get() {
    return font_size;
}

void OLED_alt_font_size(font_enum_t size){
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

void OLED_print_countdown(uint8_t line)
{
    OLED_clear();
    for (int8_t k = 2; k>=0;k--)
    {
        for (uint8_t i = 0; i < 2; i++)
        {
            OLED_goto_pos(line+i, 50);
            for (uint8_t j = 0 ; j < 16; j++)
            {
                write_data(pgm_read_byte(&font16[i+2*k][j]));
            }
        }
        _delay_ms(1000);
    }
}

/* ---------Functions that utilizes flash memory --------- */

// print function for reading program memory
void OLED_printf_P(const char * data, uint8_t line, uint8_t col)  {
    OLED_goto_pos(line, col);
    while ((pgm_read_byte(data) != 0x00)) {
        OLED_print_char(pgm_read_byte(data++)); //increment pointer and print char
    }
}

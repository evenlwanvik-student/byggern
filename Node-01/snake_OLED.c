#include <stdlib.h>
#include <stdint.h>

#include "OLED_driver.h"

// matrix (array) holding the screen pixel encoding
unsigned char OLED_buf[1024];

/* Iterate over the whole display and encode everything from
the buffer onto the display */
void update_OLED()
{
    uint8_t line, col;
    unsigned char *p = OLED_buf;
    for (line=0,col=0; line<8; line++)
    {
        OLED_goto_pos(line,col);
        for (col = 0; col<128; col++)
        {
            write_data(*p++); // *p will increment for all 8*128 iterations
        }
    }
    OLED_goto_pos(0,0);
}

void snake_OLED_init()
{
    printf("initializing OLED for snake ...");
    OLED_clear();
    // clear the OLED buffer
    for (uint16_t i = 0; i<1024; i++) OLED_buf[i] = 0;
    update_OLED();
    OLED_goto_pos(0,0);
}

// encode a pixel, set the bits in the matrix (array) buffer
void one_pixel(uint8_t x, uint8_t y, uint8_t val)
{
  	unsigned char *p = &OLED_buf[((y>>3)<<7) + x];

  	if (val) *p |=  (1<<(y%8));
  	else     *p &= ~(1<<(y%8));
}

uint8_t read_pixel(uint8_t x,uint8_t y)
{
	unsigned char *p = &OLED_buf[((y>>3)<<7) + x];//ÕÒµ½¶ÔÓ¦×Ö½Ú
	if (*p & (1<<(y%8))) return 1;
	else return 0;
}

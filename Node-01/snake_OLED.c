#ifndef F_CPU
#define F_CPU 4915200UL	//This is just a macro, it has no data type.
#endif

#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>

#include "OLED_driver.h"

typedef unsigned char uchar_t;

// matrix (array) holding the screen pixel encoding
uchar_t *OLED_buf = (char *) 1800;

/* Iterate over the whole display and encode everything from
the buffer onto the display */
void update_OLED()
{
    uint8_t line, col;
    uchar_t *p = OLED_buf;
    for (line=0,col=0; line<8; line++)
    {
        printf("\nLine %d: \n\r", line);
        OLED_goto_pos(line,col);
        for (col = 0; col<128; col++)
        {
            printf("%d ", *p);
            write_data(*p++); // *p will increment for all 8*128 iterations
        }
    }
    OLED_goto_pos(0,0);
    _delay_ms(5000);
}

void clear_buffer()
{
    for (uint16_t i = 0; i<1024; i++) OLED_buf[i] = 0;
}

void snake_OLED_init()
{
    //printf("initializing OLED for snake ...");
    OLED_clear();
    update_OLED();
    clear_buffer();      // clear OLED buffer matrix
    OLED_goto_pos(0,0);
}

// encode a pixel, set the bits in the matrix (array) buffer
void one_pixel(uchar_t x, uchar_t y, uchar_t val)
{
  	uchar_t *p = &OLED_buf[((y>>3)<<7) + x];

  	if (val) *p |=  (1<<(y%8));
  	else     *p &= ~(1<<(y%8));
}

uchar_t read_pixel(uchar_t x,uchar_t y)
{
    //printf("\n\nchecking pixel (%d,%d),\n\r", x, y);
  	uchar_t *p = &OLED_buf[((y>>3)<<7) + x];
    //printf("%d\n\r", *p);
    //printf("%d\n\n\r", (1<<(y%8)));
    if(*p & (1<<(y%8)))return 1;
  	//if (*p & (1<<(y%8))) return 1;
  	else return 0;
}

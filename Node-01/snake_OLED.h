#ifndef SNAKE_OLED_H_
#define SNAKE_OLED_H_

typedef unsigned char uchar_t;

void snake_OLED_init();
void update_OLED();
void one_pixel(uchar_t x, uchar_t y, uchar_t val);
uchar_t read_pixel(uchar_t x,uchar_t y);

#endif /* SNAKE_OLED_H_ */

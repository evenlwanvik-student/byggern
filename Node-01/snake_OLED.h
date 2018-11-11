#ifndef SNAKE_OLED_H_
#define SNAKE_OLED_H_

void snake_OLED_init();
void update_OLED();
void one_pixel(uint8_t x, uint8_t y, uint8_t val);
uint8_t read_pixel(uint8_t x,uint8_t y);

#endif /* SNAKE_OLED_H_ */

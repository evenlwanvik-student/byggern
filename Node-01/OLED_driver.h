#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_

void OLED_clear();
void OLED_init();
void OLED_print_char(char print_c);
void OLED_printf(char *data, uint8_t line, uint8_t col);
void OLED_printf_P(const char * data, uint8_t line, uint8_t col);
void OLED_align_centre(char* input_str);
void OLED_goto_pos(uint8_t line, uint8_t column);
void OLED_print_welcome_P(const char* data);
uint8_t OLED_fontsize_get();

typedef enum {
    FONT8,
    FONT5,
    FONT4
} font_enum_t;

void OLED_alt_font_size(font_enum_t size);

typedef enum{
    HORIZONTAL_MODE,
    VERTICAL_MODE,
    PAGE_MODE
} adressing_mode_t;

typedef struct {
	int line, col;
} oled_position_t;

#endif /* OLED_DRIVER_H_ */

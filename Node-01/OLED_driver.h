

void init_OLED();

void OLED_print_char(char print_c);

typedef enum {
    FONT8,
    FONT5,
    FONT4
} font_enum;

typedef enum{
    HORIZONTAL_MODE,
    VERTICAL_MODE,
    PAGE_MODE
} adressing_mode;

typedef struct {
	int row, col;
} oled_position_t;

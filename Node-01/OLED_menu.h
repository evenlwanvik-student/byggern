#ifndef OLED_MENU_H_
#define OLED_MENU_H_

#include <avr/pgmspace.h>

#include "OLED_driver.h"

void OLED_menu_init(void);
uint8_t OLED_navigate_menu(void);
void OLED_update_points(int16_t val);
void OLED_game_over(void);

volatile uint16_t points;

typedef enum {
    MAIN_MENU,
    CREDITS_MENU,
    PINGIS_MENU,
    HIGHSCORE_MENU,
    GAME_PAGE,
    POST_PAGE,
    DIFF_PAGE,
    MODE_PAGE
} menues_t;

typedef enum {
    MENU_ITEM_1,
    MENU_ITEM_2,
    MENU_ITEM_3,
    MENU_ITEM_4,
} menu_options_t;

typedef enum {
    PINGIS,
    SNAKE,
    NORMAL
} game_type_t;

typedef enum{
    DEFAULT,
    HARD,
    BEASTMODE
} difficulty_t;

typedef enum{
    USB_CARD,
    IOT
} mode_t;

volatile difficulty_t difficulty;
volatile game_type_t game_type;
volatile mode_t game_mode;

#endif /* OLED_MENU_H_ */

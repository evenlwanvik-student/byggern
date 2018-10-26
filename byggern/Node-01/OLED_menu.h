#ifndef OLED_MENU_H_
#define OLED_MENU_H_

#include <avr/pgmspace.h>

#include "OLED_driver.h"

void OLED_menu_init(void);
void OLED_navigate_menu(void);

typedef enum {
    MAIN_MENU,
    CREDITS_MENU
} menues_t;

typedef enum {
    MENU_ITEM_1,
    MENU_ITEM_2,
    MENU_ITEM_3,
    MENU_ITEM_4,
    MENU_ITEM_5,
    MENU_ITEM_6
} menu_options_t;

#endif /* OLED_MENU_H_ */

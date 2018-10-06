#include <util/delay.h>

#include "OLED_menu.h"
#include "OLED_driver.h"
#include "joystick_driver.h"

volatile uint8_t current_menu_size;

volatile uint8_t menuNo;
volatile uint8_t col_offset_main = 10;
volatile uint8_t col_offset_sub = 20;
volatile uint8_t line_offset = 1;
volatile menu_options_t current_menu;

/*------------------- flash memory stuff -----------------*/

const char welcome_P[] PROGMEM = "Welcome";

const char mainM100[] PROGMEM = "Main menu";
// choices in main menu
const char mainM101[] PROGMEM = "Menu Item 1";
const char mainM102[] PROGMEM = "Menu Item 2";
const char mainM103[] PROGMEM = "Menu Item 3";
const char * const main_menu_pointer[] PROGMEM = {mainM101, mainM102, mainM103};

const char creditsM200[] PROGMEM = "Credits";
// choices in main menu
const char creditsM201[] PROGMEM = "Even Wanvik";
const char creditsM202[] PROGMEM = "Michael Mullins";
const char creditsM203[] PROGMEM = "Unicorns";
const char * const credits_menu_pointer[] PROGMEM = {creditsM201, creditsM202, creditsM203};

//Menu structure
//[0] -Number of level 0 menu items
//[1]...[n] number of second level menu items
//Eg. MSTR2[1] shows that menu item 1 has 3 submenus
const uint8_t MSIZE[] PROGMEM = {
    3,    //number of menu items
    3    //Number of submenu items of menu item 1
    //add more later
};

/*------------------- flash memory stuff -----------------*/

menu_options_t OLED_menu_selection()
{

    joystick_direction_t dir = joystick_direction_get();

    if (dir==UP) {
        OLED_goto_credits();
        printf("going up");
        // pos.line--;
        // if (pos.line < 2) {
        //     pos.line = pos.line + MSIZE[current_menu];
        // }
        // OLED_print_menu();
    }
    else if (dir==DOWN) {
        OLED_goto_main();
        printf("going down");
    }
    else {
        if (joystick_button(JOYSTICKBUTTON)) {
            if (current_menu == MAIN_MENU) {
                OLED_goto_credits();
            }
            else {
                OLED_goto_main();
            }
            printf("button clicked");
        }
    }
}

void OLED_print_menu() {

}

// current menu = 0, M100
void OLED_goto_main() {
    OLED_clear();
    current_menu = MAIN_MENU;
    uint8_t line = line_offset;
    OLED_alt_font_size(FONT8);
    OLED_printf_P(mainM100, line++, col_offset_main);
    OLED_alt_font_size(FONT5);
    for (uint8_t i = 0; i<pgm_read_byte(&MSIZE[current_menu]); i++) {
        OLED_printf_P((char*)pgm_read_word(&main_menu_pointer[i]), line++, col_offset_sub);
    }
}

// current menu = 0, M200
void OLED_goto_credits() {
    OLED_clear();
    current_menu = CREDITS_MENU;
    uint8_t line = line_offset;
    OLED_alt_font_size(FONT8);
    OLED_printf_P(creditsM200, line++, col_offset_main);
    OLED_alt_font_size(FONT5);
    for (uint8_t i = 0; i<pgm_read_byte(&MSIZE[current_menu]); i++) {
        OLED_printf_P((char*)pgm_read_word(&credits_menu_pointer[i]), line++, col_offset_sub);
    }
}

void OLED_menu_init(void) {
    menuNo = 0;
    current_menu = MAIN_MENU;
    uint8_t ctr_line = (uint8_t)(64-7*OLED_fontsize_get()/2);
    OLED_printf_P(welcome_P,4,ctr_line); // write in centre
    _delay_ms(10000);
    OLED_clear();
    OLED_goto_main();
}

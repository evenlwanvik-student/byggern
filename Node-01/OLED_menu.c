#include <util/delay.h>
#include <stdio.h>

#include "OLED_menu.h"
#include "OLED_driver.h"
#include "joystick_driver.h"

volatile uint8_t current_menu_size;

volatile uint8_t menuNo;
volatile uint8_t col_offset_main;
volatile uint8_t col_offset_sub;
volatile uint8_t line_offset;
volatile uint8_t option_line;         // arrow that indicates the current coice, initialize to 1st option (skips header)
volatile menues_t current_menu;                         // always start program at main menu
volatile menu_options_t current_option;   // Always start program at first item

/*------------------- flash memory stuff -----------------*/

const char welcome_P[] PROGMEM = "Welcome";

const char mainM100[] PROGMEM = "Main menu";
// choices in main menu
const char mainM101[] PROGMEM = "Menu Item 1";
const char mainM102[] PROGMEM = "Menu Item 2";
const char mainM103[] PROGMEM = "Credits";
const char * const main_menu_pointer[] PROGMEM = {mainM101, mainM102, mainM103};

const char creditsM200[] PROGMEM = "Credits";
// Credits page names
const char creditsM201[] PROGMEM = "Even Wanvik";
const char creditsM202[] PROGMEM = "Michael Mullins";
const char creditsM203[] PROGMEM = "Back to Main Menu";
const char * const credits_menu_pointer[] PROGMEM = {creditsM201, creditsM202, creditsM203};

//Menu structure
//Eg. MSIZE[1] shows that menu item 1 has 3 submenus
const uint8_t MSIZE[] PROGMEM = {
    3,    //number of main menu items
    3     //Number of credits menu items
};

/*------------------- flash memory stuff -----------------*/

void OLED_print_menu(void) {
    uint8_t line = line_offset;                             // line iterator for printing all strings
    OLED_clear();
    switch (current_menu) {
        case MAIN_MENU:
            OLED_alt_font_size(FONT8);                                       // big letters for menu name
            OLED_printf_P(mainM100, line++, col_offset_main);                // print menu name
            OLED_alt_font_size(FONT5);                                       // small letters for menu options
            for (uint8_t i = 0; i<pgm_read_byte(&MSIZE[current_menu]); i++)  // print menu options
                OLED_printf_P((char*)pgm_read_word(&main_menu_pointer[i]), line++, col_offset_sub);
            OLED_alt_font_size(FONT4);
            OLED_goto_pos(option_line, col_offset_sub - 6);                 // go to column before option to print the arrow
            OLED_print_char('>');                                            // print arrow for current option
            break;
        case CREDITS_MENU:
            OLED_alt_font_size(FONT8);                                       // big letters for menu name
            OLED_printf_P(creditsM200, line++, col_offset_main);             // print menu name
            OLED_alt_font_size(FONT5);                                       // small letters for menu options
            for (uint8_t i = 0; i<pgm_read_byte(&MSIZE[current_menu]); i++)  // print menu options
                OLED_printf_P((char*)pgm_read_word(&credits_menu_pointer[i]), line++, col_offset_sub);
            OLED_goto_pos(option_line, col_offset_sub - 6);                  // go to column before option to print the arrow
            OLED_alt_font_size(FONT4);
            OLED_print_char('>');                                            // print arrow for current option
            break;
    }
}

void OLED_choose_option(void) {
    switch (current_option) {
        case MENU_ITEM_1:
            switch (current_menu) {
                case MAIN_MENU:
                    printf("Choosing menu Item 1 in main menu\n");
                    break;
                case CREDITS_MENU:
                    printf("Choosing menu Item 1 in credits menu\n");
                    break;
            }
        case MENU_ITEM_2:
            switch (current_menu) {
                case MAIN_MENU:
                    printf("Choosing menu Item 2 in main menu\n");
                    break;
                case CREDITS_MENU:
                    printf("Choosing menu Item 2 in credits menu\n");
                    break;
            }
        case MENU_ITEM_3:
            switch (current_menu) {
                case MAIN_MENU:
                    printf("Going to credits menu\n");
                    current_menu = CREDITS_MENU;
                    break;
                case CREDITS_MENU:
                    printf("Going back to main menu\n");
                    current_menu = MAIN_MENU;
                    break;
            }
        default:
            printf("%d is not an available choise yet\n", current_option);
    }
    current_option = MENU_ITEM_1;       // reset to menu_item 1 for new menu choice
    option_line = line_offset + 1;      // reset to option line 1 for new menu choice
}

void OLED_navigate_menu(void) // call this function from main
{
    uint8_t menusize = pgm_read_byte(&MSIZE[current_menu]);
    joystick_direction_t dir = joystick_direction_get();
    if (dir==UP) {                                  // If joystick is 'UP'
        // OLED_goto_credits();
        printf("going up\n");
        option_line--;                              // current option line goes upwards to next option
        current_option--;                           // also iterate the enum to MENU_ITEM_xx
        if (option_line < (line_offset + 1)) {      // if option line is smaller than offset + title line (1)
            option_line = option_line + menusize;   // go to bottom option (cyclic menu)
            current_option = menusize;              // go to last MENU_ITEM
        }
        OLED_print_menu();
    }
    else if (dir==DOWN) {
        //OLED_goto_main();
        printf("going down\n");
        option_line++;
        current_option++;
        if (current_option >= menusize) {
            option_line = line_offset + 1;
            current_option = 0;
        }
        OLED_print_menu();
    }
    else if (joystick_button(JOYSTICKBUTTON) ||
               joystick_button(RIGHT_SLIDER) ||
               joystick_button(LEFT_SLIDER)){
        printf("choosing option\n");
        OLED_choose_option();
        OLED_print_menu();
    }
}

void OLED_menu_init(void) {
    menuNo = 0;
    current_menu = MAIN_MENU;
    current_option = MENU_ITEM_1;
    col_offset_main = 10;
    col_offset_sub = 20;
    line_offset = 1;
    option_line = line_offset + 1;

    uint8_t ctr_line = (uint8_t)(64-7*OLED_fontsize_get()/2);
    OLED_printf_P(welcome_P,4,ctr_line); // write in centre
    _delay_ms(10000);
    OLED_clear();
    OLED_print_menu();
}


#ifndef F_CPU
#define F_CPU 4915200UL	//This is just a macro, it has no data type.
#endif

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
volatile uint8_t option_line;             // arrow that indicates the current coice, initialize to 1st option (skips header)
volatile menues_t current_menu;           // always start program at main menu
volatile menu_options_t current_option;   // Always start program at first item

volatile uint8_t game_selected;

// for game
volatile uint8_t survival_time;
volatile uint8_t strikes;

/*------------------- flash memory stuff -----------------*/

const char welcome_P[] PROGMEM = "Welcome";

const char mainM100[] PROGMEM = "Main menu";
// choices in main menu
const char mainM101[] PROGMEM = "Play Game";
const char mainM102[] PROGMEM = "Highscore";
const char mainM103[] PROGMEM = "Credits";
const char * const main_menu_pointer[] PROGMEM = {mainM101, mainM102, mainM103};

const char creditsM200[] PROGMEM = "Credits";
// Credits page names
const char creditsM201[] PROGMEM = "Back to Main Menu";
const char creditsM202[] PROGMEM = "Even Wanvik";
const char creditsM203[] PROGMEM = "Michael Mullins";
const char creditsM204[] PROGMEM = "Unicorns";
const char * const credits_menu_pointer[] PROGMEM = {creditsM201, creditsM202, creditsM203, creditsM204};

const char pingisM200[] PROGMEM = "Pingis";
// Credits page names
const char pingisM201[] PROGMEM = "Start/Pause Game";
const char pingisM202[] PROGMEM = "New Game";
const char pingisM203[] PROGMEM = "Back To Main";
const char pingisM204[] PROGMEM = "Survival Time:";
const char pingisM205[] PROGMEM = "Strikes:";
const char * const PINGIS_MENU_pointer[] PROGMEM = {pingisM201, pingisM202, pingisM203, pingisM204, pingisM205};

const char highscoreM200[] PROGMEM = "Highscore";
// Credits page names
const char highscoreM201[] PROGMEM = "Back To Main Menu";
const char highscoreM202[] PROGMEM = "1. ";
const char highscoreM203[] PROGMEM = "2. ";
const char highscoreM204[] PROGMEM = "3. ";
const char highscoreM205[] PROGMEM = "4. ";
const char highscoreM206[] PROGMEM = "5. ";
const char * const highscore_menu_pointer[] PROGMEM = {highscoreM201, highscoreM202, highscoreM203, highscoreM204, highscoreM205, highscoreM205};



//Menu structure
//Eg. MSIZE[1] shows that menu item 1 has 3 submenus
// of which is dynamic, and has more functionality than just printing stuff
const uint8_t MSIZE[] PROGMEM = {
    3,    //number of main menu items
    1,    //Number of credits menu items
    2,    //Number of game menu items
    1,    //Number of highscore menu items
};

/*------------------- flash memory stuff -----------------*/

void OLED_print_menu(void) {
    uint8_t line = line_offset;                             // line iterator for printing all strings
    OLED_clear();
    switch (current_menu) {
        case MAIN_MENU: {
            OLED_alt_font_size(FONT8);                                       // big letters for menu name
            OLED_printf_P(mainM100, line++, col_offset_main);                // print menu name
            OLED_alt_font_size(FONT5);                                       // small letters for menu options
            for (uint8_t i = 0; i<pgm_read_byte(&MSIZE[current_menu]); i++)  // print menu options
                OLED_printf_P((char*)pgm_read_word(&main_menu_pointer[i]), line++, col_offset_sub);
            OLED_alt_font_size(FONT4);
            OLED_goto_pos(option_line, col_offset_sub - 6);                 // go to column before option to print the arrow
            OLED_print_char('>');                                            // print arrow for current option
            break;
        }
        case CREDITS_MENU: {
            OLED_alt_font_size(FONT8);                                       // big letters for menu name
            OLED_printf_P(creditsM200, line++, col_offset_main);             // print menu name
            OLED_alt_font_size(FONT5);                                       // small letters for menu options
            OLED_printf_P((char*)pgm_read_word(&credits_menu_pointer[0]), line++, col_offset_sub);
            OLED_goto_pos(option_line, col_offset_sub - 6);                  // go to column before option to print the arrow
            OLED_alt_font_size(FONT4);
            OLED_print_char('>');                                            // print arrow for current option
            line++;
            for (uint8_t i = 1; i<4; i++)
                OLED_printf_P((char*)pgm_read_word(&credits_menu_pointer[i]), line++, col_offset_sub);
            break;
        }
        case PINGIS_MENU: {
            uint8_t i = 0;
            OLED_alt_font_size(FONT8);                                       // big letters for menu name
            OLED_printf_P(pingisM200, line++, col_offset_main);               // print menu name
            OLED_alt_font_size(FONT5);                                       // small letters for menu options
            for (i; i<pgm_read_byte(&MSIZE[current_menu]); i++)  // print menu options
                OLED_printf_P((char*)pgm_read_word(&PINGIS_MENU_pointer[i]), line++, col_offset_sub);
            OLED_goto_pos(option_line, col_offset_sub - 6);                  // go to column before option to print the arrow
            OLED_alt_font_size(FONT4);
            OLED_print_char('>');                                            // print arrow for current option
            OLED_printf_P((char*)pgm_read_word(&PINGIS_MENU_pointer[i++]), line+=2, col_offset_sub); // print survival time
            OLED_print_value(survival_time, line, col_offset_sub + 20);
            OLED_printf_P((char*)pgm_read_word(&PINGIS_MENU_pointer[i++]), line+=2, col_offset_sub); // print strikes
            OLED_print_value(strikes, line, col_offset_sub + 20);
            break;
        }
        case HIGHSCORE_MENU: {
            OLED_alt_font_size(FONT8);                                       // big letters for menu name
            OLED_printf_P(highscoreM200, line++, col_offset_main);             // print menu name
            OLED_alt_font_size(FONT5);                                       // small letters for menu options
            OLED_printf_P((char*)pgm_read_word(&highscore_menu_pointer[0]), line++, col_offset_sub);
            OLED_goto_pos(option_line, col_offset_sub - 6);                  // go to column before option to print the arrow
            OLED_alt_font_size(FONT4);
            OLED_print_char('>');                                            // print arrow for current option
            for (uint8_t i = 1; i<6; i++)
                OLED_printf_P((char*)pgm_read_word(&highscore_menu_pointer[i]), line++, col_offset_sub);
            break;
        }
    }
}

void OLED_choose_option(void) {
    switch (current_option) {
        case MENU_ITEM_1:
            switch (current_menu) {
                case MAIN_MENU:
                    printf("Go to pingis menu\n");
                    current_menu = PINGIS_MENU;
                    break;
                case CREDITS_MENU:
                    printf("Back to main menu\n");
                    current_menu = MAIN_MENU;
                    break;
                case PINGIS_MENU:
                    printf("Start/Pause game\n");
                    game_type = PINGIS;
                    game_selected = 1;
                    break;
                case HIGHSCORE_MENU:
                    printf("Back to main menu\n");
                    current_menu = MAIN_MENU;
                    break;
            }
            break;
        case MENU_ITEM_2:
            switch (current_menu) {
                case MAIN_MENU:
                    printf("Go to high Score\n");
                    current_menu = HIGHSCORE_MENU;
                    break;
                case PINGIS_MENU:
                    printf("New game\n");
                    game_type = PINGIS;
                    game_selected = 1;
                    break;
            }
            break;
        case MENU_ITEM_3:
            switch (current_menu) {
                case MAIN_MENU:
                    printf("Go to credits menu\n");
                    current_menu = CREDITS_MENU;
                    break;
                case PINGIS_MENU:
                    printf("Back to main menu\n");
                    current_menu = MAIN_MENU;
                    break;
            }
            break;
        default:
            printf("%d is not an available choise yet\n", current_option);
    }
    current_option = MENU_ITEM_1;       // reset to menu_item 1 for new menu choice
    option_line = line_offset + 1;      // reset to option line 1 for new menu choice
}

uint8_t OLED_navigate_menu(void) // call this function from main
{
    game_selected = 0;
    uint8_t menusize = pgm_read_byte(&MSIZE[current_menu]);
    joystick_direction_t dir = joystick_direction_get();
    if (dir==UP) {                                  // If joystick is 'UP'
        option_line--;                              // current option line goes upwards to next option
        current_option--;                           // also iterate the enum to MENU_ITEM_xx
        if (option_line < (line_offset + 1)) {      // if option line is smaller than offset + title line (1)
            option_line = option_line + menusize;   // go to bottom option (cyclic menu)
            current_option = menusize;              // go to last MENU_ITEM
        }
        OLED_print_menu();
    }
    else if (dir==DOWN) {
        option_line++;
        current_option++;
        if (current_option >= menusize) {
            option_line = line_offset + 1;
            current_option = 0;
        }
        OLED_print_menu();
    }
    else if (joystick_button(JOYSTICKBUTTON) ||
               joystick_button(RBUTTON) ||
               joystick_button(LBUTTON)){
        OLED_choose_option();
        OLED_print_menu();
    }
    return game_selected;
}

void OLED_menu_init(void) {

    printf("Initializing OLED menu...\n\n\r");
    _delay_ms(500);

    menuNo = 0;
    current_menu = MAIN_MENU;
    current_option = MENU_ITEM_1;
    col_offset_main = 10;
    col_offset_sub = 20;
    line_offset = 1;
    option_line = line_offset + 1;
    game_selected = 0;
    strikes = 0;
    survival_time = 0;

    uint8_t ctr_line = (uint8_t)(64-7*OLED_fontsize_get()/2);
    OLED_printf_P(welcome_P,4,ctr_line); // write in centre
    _delay_ms(5000);
    OLED_clear();
    OLED_print_menu();
}

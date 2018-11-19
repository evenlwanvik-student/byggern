#ifndef F_CPU
#define F_CPU 4915200UL	//This is just a macro, it has no data type.
#endif

#include <util/delay.h>
#include <stdio.h>

#include "OLED_menu.h"
#include "OLED_driver.h"
#include "joystick_driver.h"
#include "pingis_game.h"

volatile uint8_t current_menu_size;

volatile uint8_t menuNo;
volatile uint8_t col_offset_main;
volatile uint8_t col_offset_sub;
volatile uint8_t line_offset;
volatile uint8_t option_line;             // arrow that indicates the current coice, initialize to 1st option (skips header)
volatile menues_t current_menu;           // always start program at main menu
volatile menu_options_t current_option;   // Always start program at first item

volatile uint8_t game_selected;

volatile uint8_t game_over;        // boolean for game over
volatile uint16_t prev_points;

volatile char* ext_highscore_ptr = 0x1800;

/*------------------- flash memory stuff -----------------*/

const char welcome_P[] PROGMEM = "Welcome";

const char mainM100[] PROGMEM = "Main menu";
// options in main menu
const char mainM101[] PROGMEM = "Play Game";
const char mainM102[] PROGMEM = "Highscore";
const char mainM103[] PROGMEM = "Credits";
const char * const main_menu_pointer[] PROGMEM = {mainM101, mainM102, mainM103};

const char creditsM200[] PROGMEM = "Credits";
// Credits page names
const char creditsM201[] PROGMEM = "Back to Main";
const char creditsM202[] PROGMEM = "Even Wanvik";
const char creditsM203[] PROGMEM = "Michael Mullins";
const char creditsM204[] PROGMEM = "Unicorns";
const char * const credits_menu_pointer[] PROGMEM = {creditsM201, creditsM202, creditsM203, creditsM204};

const char pingisM200[] PROGMEM = "Pingis";
// Pingis page options
const char pingisM201[] PROGMEM = "New Game";
const char pingisM202[] PROGMEM = "Back To Main";
const char pingisM203[] PROGMEM = "Choose Difficulty";
const char pingisM204[] PROGMEM = "Control mode";
const char * const PINGIS_MENU_pointer[] PROGMEM = {pingisM201, pingisM202, pingisM203, pingisM204};

const char highscoreM200[] PROGMEM = "Highscore";
// Highscore page names
const char highscoreM201[] PROGMEM = "Back To Main";
const char highscoreM202[] PROGMEM = "1. ";
const char highscoreM203[] PROGMEM = "2. ";
const char highscoreM204[] PROGMEM = "3. ";
const char highscoreM205[] PROGMEM = "4. ";
const char highscoreM206[] PROGMEM = "5. ";
const char * const highscore_menu_pointer[] PROGMEM = {highscoreM201, highscoreM202, highscoreM203, highscoreM204, highscoreM205, highscoreM206};

const char gameM200[] PROGMEM = "Good luck!";
// Game page
const char gameM201[] PROGMEM = "Currently playing:";
const char gameM202[] PROGMEM = "PINGIS";
const char gameM203[] PROGMEM = "Points:";
const char * const GAME_PAGE_pointer[] PROGMEM = {gameM201, gameM202, gameM203};

const char postM200[] PROGMEM = "Sorry, you lost";
// Post_game page
const char postM201[] PROGMEM = "Try again";
const char postM202[] PROGMEM = "Back To Main";
const char postM203[] PROGMEM = "Score:";
const char * const POST_GAME_pointer[] PROGMEM = {postM201, postM202, postM203};

const char diffM200[] PROGMEM = "Difficulty";
// Post_game page
const char diffM201[] PROGMEM = "Normal";
const char diffM202[] PROGMEM = "Hard";
const char diffM203[] PROGMEM = "BEASTMODE!";
const char * const DIFF_pointer[] PROGMEM = {diffM201, diffM202, diffM203};

const char modeM200[] PROGMEM = "Control Mode";
// Post_game page
const char modeM201[] PROGMEM = "Normal";
const char modeM202[] PROGMEM = "IoT";
const char * const MODE_pointer[] PROGMEM = {modeM201, modeM202};

//Menu structure
//Eg. MSIZE[1] shows that menu item 1 has 3 submenus
// of which is dynamic, and has more functionality than just printing stuff
const uint8_t MSIZE[] PROGMEM = {
    3,    //number of main menu items
    1,    //Number of credits menu items
    4,    //Number of pingis menu items
    1,    //Number of highscore menu items
    0,    //Number of game page items
    2,    //Number of post-game page items
    3,     //Number of difficulty page items
    2     //Number of mode...
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
            {
                OLED_printf_P((char*)pgm_read_word(&highscore_menu_pointer[i]), line, col_offset_sub);
                OLED_print_value(ext_highscore_ptr[i-1], line++, col_offset_sub + 20);
            }
            break;
        }
        case GAME_PAGE: {
            uint8_t i = 0;
            OLED_alt_font_size(FONT8);                                       // big letters for menu name
            OLED_printf_P(gameM200, line++, col_offset_main);               // print menu name
            OLED_alt_font_size(FONT5);                                       // small letters for menu options
            for (i = 0; i<2; i++)
                OLED_printf_P((char*)pgm_read_word(&GAME_PAGE_pointer[i]), line++, col_offset_sub);
            OLED_printf_P((char*)pgm_read_word(&GAME_PAGE_pointer[i++]), line+=2, col_offset_sub); // print survival time
            OLED_print_value(points, line, col_offset_sub + 40);
            break;
        }
        case POST_PAGE: {
            uint8_t i = 0;
            OLED_alt_font_size(FONT8);                                       // big letters for menu name
            OLED_printf_P(postM200, line++, col_offset_main);               // print menu name
            OLED_alt_font_size(FONT5);                                       // small letters for menu options
            for (i; i<pgm_read_byte(&MSIZE[current_menu]); i++)  // print menu options
                OLED_printf_P((char*)pgm_read_word(&POST_GAME_pointer[i]), line++, col_offset_sub);
            OLED_goto_pos(option_line, col_offset_sub - 6);                  // go to column before option to print the arrow
            OLED_alt_font_size(FONT4);
            OLED_print_char('>');                                            // print arrow for current option
            OLED_printf_P((char*)pgm_read_word(&POST_GAME_pointer[i++]), line+=2, col_offset_sub); // print survival time
            OLED_print_value(prev_points, line, col_offset_sub + 30);
            break;
        }
        case DIFF_PAGE: {
            OLED_alt_font_size(FONT8);                                       // big letters for menu name
            OLED_printf_P(diffM200, line++, col_offset_main);                // print menu name
            OLED_alt_font_size(FONT5);                                       // small letters for menu options
            for (uint8_t i = 0; i<pgm_read_byte(&MSIZE[current_menu]); i++)  // print menu options
                OLED_printf_P((char*)pgm_read_word(&DIFF_pointer[i]), line++, col_offset_sub);
            OLED_alt_font_size(FONT4);
            OLED_goto_pos(option_line, col_offset_sub - 6);                 // go to column before option to print the arrow
            OLED_print_char('>');                                            // print arrow for current option
            break;
        }
        case MODE_PAGE: {
            OLED_alt_font_size(FONT8);                                       // big letters for menu name
            OLED_printf_P(modeM200, line++, col_offset_main);                // print menu name
            OLED_alt_font_size(FONT5);                                       // small letters for menu options
            for (uint8_t i = 0; i<pgm_read_byte(&MSIZE[current_menu]); i++)  // print menu options
                OLED_printf_P((char*)pgm_read_word(&MODE_pointer[i]), line++, col_offset_sub);
            OLED_alt_font_size(FONT4);
            OLED_goto_pos(option_line, col_offset_sub - 6);                 // go to column before option to print the arrow
            OLED_print_char('>');                                            // print arrow for current option
            break;
        }
    }
}

void OLED_choose_option(void) {
    switch (current_option) {
        case MENU_ITEM_1:
            switch (current_menu) {
                case MAIN_MENU:
                    printf("Go to pingis menu\n\r");
                    game_type = NORMAL;
                    current_menu = PINGIS_MENU;
                    break;
                case CREDITS_MENU:
                    printf("Back to main menu\n\r");
                    game_type = NORMAL;
                    current_menu = MAIN_MENU;
                    break;
                case PINGIS_MENU:
                    printf("New game\n\r");
                    game_type = PINGIS;
                    points = 0;
                    game_selected = 1;
                    pingis_calibrate_motor();
                    OLED_print_countdown(3);
                    current_menu = GAME_PAGE;
                    break;
                case HIGHSCORE_MENU:
                    printf("Back to main menu\n\r");
                    game_type = NORMAL;
                    current_menu = MAIN_MENU;
                    break;
                case POST_PAGE:
                    printf("Try again\n\r");
                    points = 0;
                    current_menu = PINGIS_MENU;
                    break;
                case DIFF_PAGE:
                    printf("Difficulty: normal\n\r");
                    current_menu = PINGIS_MENU;
                    difficulty = DEFAULT;
                    break;
                case MODE_PAGE:
                    printf("Normal mode\n\r");
                    current_menu = PINGIS_MENU;
                    game_mode = USB_CARD;
                    break;
            }
            break;
        case MENU_ITEM_2:
            switch (current_menu) {
                case MAIN_MENU:
                    printf("Go to high Score\n\r");
                    game_type = NORMAL;
                    current_menu = HIGHSCORE_MENU;
                    break;
                case PINGIS_MENU:
                    printf("Back to main menu\n\r");
                    game_type = NORMAL;
                    current_menu = MAIN_MENU;
                    break;
                case POST_PAGE:
                    printf("Back to main\n\r");
                    points = 0;
                    current_menu = MAIN_MENU;
                    break;
                case DIFF_PAGE:
                    printf("Difficulty: hard\n\r");
                    current_menu = PINGIS_MENU;
                    difficulty = HARD;
                    break;
                case MODE_PAGE:
                    printf("IoT mode\n\r");
                    current_menu = PINGIS_MENU;
                    game_mode = IOT;
                    break;
            }
            break;
        case MENU_ITEM_3:
            switch (current_menu) {
                case MAIN_MENU:
                    printf("Go to credits menu\n\r");
                    game_type = NORMAL;
                    current_menu = CREDITS_MENU;
                    break;
                case PINGIS_MENU:
                    printf("Choose difficulty\n\r");
                    game_type = NORMAL;
                    current_menu = DIFF_PAGE;
                    break;
                case DIFF_PAGE:
                    printf("Difficulty: BEASTMODE!\n\r");
                    current_menu = PINGIS_MENU;
                    difficulty = BEASTMODE;
                    break;
            }
            break;
        case MENU_ITEM_4:
            switch(current_menu){
                case PINGIS_MENU:
                    printf("Choose mode\n\r");
                    current_menu = MODE_PAGE;
                    break;
            }
            break;
        default:
            printf("%d is not an available choice yet\n\r", current_option);
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
    }
    else if (dir==DOWN) {
        option_line++;
        current_option++;
        if (current_option >= menusize) {
            option_line = line_offset + 1;
            current_option = 0;
        }
    }
    else if (joystick_button(JOYSTICKBUTTON)) OLED_choose_option();
    OLED_print_menu();
    return game_selected;
}

void OLED_menu_init(void) {
    menuNo = 0;
    current_menu = MAIN_MENU;
    current_option = MENU_ITEM_1;
    col_offset_main = 10;
    col_offset_sub = 20;
    line_offset = 1;
    option_line = line_offset + 1;
    game_selected = 0;
    game_over = 0;
    points = 0;
    prev_points = 0;
    game_type = NORMAL;

    difficulty = DEFAULT;
    game_mode = USB_CARD;

    OLED_clear();
    uint8_t ctr_line = (uint8_t)(64-7*OLED_fontsize_get()/2);
    OLED_printf_P(welcome_P,4,ctr_line); // write in centre
    _delay_ms(2000);
    OLED_clear();
    OLED_print_menu();
}

void OLED_game_over(void)
{
    prev_points = points;
    OLED_check_highscore();
    points = 0;
    game_selected = 0;
    game_type = NORMAL;
    current_menu = POST_PAGE;
    OLED_print_menu();
}

void OLED_update_points(int16_t val)
{
    points = val;
}

void OLED_check_highscore()
{
    for (uint8_t i = 0; i<5; i++)
    {
       if (prev_points > ext_highscore_ptr[i])
       {
           for (uint8_t j = 4; j>i; j--)
           {
              ext_highscore_ptr[j] = ext_highscore_ptr[j-1];
           }
           ext_highscore_ptr[i] = prev_points;
           return;
       }
    }
}

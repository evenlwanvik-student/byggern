#ifndef OLED_MENU_H_
#define OLED_MENU_H_

#include <avr/pgmspace.h>

#include "OLED_driver.h"

void OLED_menu_init(void);

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

menu_options_t OLED_menu_selection(void);

#endif /* OLED_MENU_H_ */










/*

typedef void (*func_ptr_t)(void);
func_ptr_t func_ptr;

//Menu names stored in flash
//menu 1
const char* main_MN100[] PROGMEM="<<Main>>\0";
//menu 2
const uint8_t credits_MN200[] PROGMEM="<<Credits>>\0";

//SubMenu Strings in flash
//menu 1 submenus
const uint8_t main_MN101[] PROGMEM="Reset screen\0";
const uint8_t main_MN102[] PROGMEM="Does nothing\0";
const uint8_t main_MN103[] PROGMEM="Credits\0";
//Submenus of menu 2
const uint8_t credits_MN201[] PROGMEM="Does nothing\0";
const uint8_t credits_MN202[] PROGMEM="Back to main\0";

//Then we have to create array pointers to menu strings stored in flash:
const char *MENU[] = {
    main_MN100,      //menu 1 string
    credits_MN200    //menu 2 string
    //add more later
};

const uint8_t *SUBMENU[] = {
    main_MN101, main_MN102, main_MN103,    //submenus of menu 1
    credits_MN201, credits_MN202           //submenus of menu 2
    //add more later
};

//Menu structure
//[0] -Number of level 0 menu items
//[1]...[n] number of second level menu items
//Eg. MSTR2[1] shows that menu item 1 has 3 submenus
const uint8_t MENU_SIZE[] PROGMEM = {
    2,    //number of menu items
    3,    //Number of submenu items of menu item 1
    2     //of menu item 2
    //add more later
}

// //Functions for each menu item
// void func101(void);
// void func102(void);
// void func103(void);
// void func201(void);
// void func202(void);
//
// // Array of pointers to each function in flash
// const func_ptr_t func_ptr_map[] PROGMEM =
//     {
//         func101, func102, func103,   //functions for submenus of menu 1
//         func201, func202             //functions for submenus of menu 2
//         //further functions...
//     };
*/

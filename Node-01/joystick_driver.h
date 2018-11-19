#ifndef JOYSTICK_DRIVER_H_
#define JOYSTICK_DRIVER_H_

#include <stdint.h>

typedef enum {
    JOYSTICK_X,
    JOYSTICK_Y,
    LEFT_SLIDER,
    RIGHT_SLIDER
} state_t;

typedef struct {
    uint8_t x, y;
} joystick_position_t;

typedef struct {
	int8_t left, right;
} sliders_position_t;

typedef enum {
    NEUTRAL,
    RIGHT,
    LEFT,
    UP,
    DOWN
} joystick_direction_t;

typedef enum {
    JOYSTICKBUTTON,
    LBUTTON,
    RBUTTON
} usb_button_t;

void joystick_init(int prescaler);
void auto_calibrate();

int joystick_button(usb_button_t button);
joystick_position_t joystick_position_get();
joystick_direction_t joystick_direction_get();

int slider_left_get(void);
int slider_right_get(void);

int joystick_button(usb_button_t button);

#endif /* JOYSTICK_DRIVER_H_ */

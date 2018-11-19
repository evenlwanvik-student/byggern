#include <stdlib.h>
#include <stdint.h>

#include "IR_driver.h"
#include "ADC_driver.h"

#define FILTER_ORDER 1

static uint16_t obstructed = 200; // ??

uint8_t IR_blocked(void) {
    if (FILTER_ORDER == 1) {
        return (ADC_read() < obstructed);
    }
    else {
        uint16_t moving_avg[FILTER_ORDER];
        // set first val, and add rest to the avg
        uint16_t measured =  ADC_read();
        uint16_t avg = measured;
        for (uint8_t i = 0; i<FILTER_ORDER-1; i++) {
            avg += moving_avg[i];
            moving_avg[i] = moving_avg[i+1];
        }
        // set the current value (last value in array)
        moving_avg[FILTER_ORDER-1] = measured;
        avg /= FILTER_ORDER; // divide by ncumber of coeffs to get avg

        // return 0 or 1 depending on avg is less than threshold
        printf("Average: %d \r\n" , avg);
        return (avg < obstructed);
    }
}

#ifndef F_CPU
#define F_CPU 16000000UL	//This is just a macro, it has no data type.
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <math.h>

#include "pitches.h"
#include "bit_macros.h"

#define TIMER1_PRESCALER (uint8_t) 8

// const uint16_t A4_FREQ = 440;
// const uint16_t C5_FREQ = 523;
// const uint16_t D6_FREQ = 1175;

// OCR1A values
#define A4 (F_CPU / (A4_FREQ * TIMER1_PRESCALER * 2) - 1)
#define C5 (F_CPU / (C5_FREQ * TIMER1_PRESCALER * 2) - 1)
#define D6 (F_CPU / (D6_FREQ * TIMER1_PRESCALER * 2) - 1)


void BUZZER_init(void)
{
   // Set OC5A as output pin (PL3)
   set_bit(DDRL, PL3);

   // Set Timer1, mode CTC, toggle on compare, prescale 8
   TCCR5A = (1 << COM5A0);
   TCCR5B = (1 << WGM52)|(1 << CS50);

}

void BUZZER_game_over()
{
    while (1)
    {
        for (uint8_t i = 0; i < sizeof(melody) ; i++)
        {
          uint16_t melody_new = round(F_CPU*10000 / (melody[i]));
          printf("%d\r\n", melody_new);

          //uint16_t melody_new = (uint16_t)( F_CPU / TIMER1_PRESCALER / 2 / melody[i] ) - 1;

          OCR5A = melody_new;
          for (uint8_t j = 0; j < (uint8_t)tempo[i]; j++)
              _delay_ms(10);
          _delay_ms(100);
        }
    }
}

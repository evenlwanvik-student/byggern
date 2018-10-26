
#ifndef MEMORY_LAYOUT_H_
#define MEMORY_LAYOUT_H_

#include "bit_macros.h"

#define EXT_RAM         ((volatile char*) 0x1800)
#define EXT_RAM_SIZE    0x800
#define EXT_ADC         ((volatile char*) 0x1400)
#define EXT_OLED_CMD    ((volatile char*) 0x1000)
#define EXT_OLED_DATA   ((volatile char*) 0x1200)

// OLEDCMD  from 0x1000-0x11FF : 0001 0000 0000 0000 - 0001 0001 1111 1111
// OLEDDATA from 0x1200-0x13FF : 0001 0010 0000 0000 - 0001 0011 1111 1111
// ADC      from 0x1400-0x17FF : 0001 0100 0000 0000 - 0001 0111 1111 1111
// SRAM     from 0x1800-0x1FFF : 0001 1000 0000 0000 - 0001 1111 1111 1111



#endif /* MEMORY_LAYOUT_H_ */

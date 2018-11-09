

#ifndef BIT_MACROS_H_
#define BIT_MACROS_H_

#include <stdint.h>

// void set_bit(uint8_t reg, uint8_t bit){ reg |= (1 << bit); }
// void clear_bit(uint8_t reg, uint8_t bit){ reg &= ~(1 << bit); }
// void test_bit(uint8_t reg, uint8_t bit){ reg &= (1 << bit); }
// void loop_until_bit_is_set(uint8_t reg, uint8_t bit){ while( !test_bit( reg, bit )); }
// void loop_until_bit_is_clear(uint8_t reg, uint8_t bit){ while( test_bit( reg, bit )); }
// void toggle_bit(uint8_t reg, uint8_t bit){ reg ^= (1 << bit); }


#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))
#define loop_until_bit_is_set( reg, bit ) while( !test_bit( reg, bit ) )
#define loop_until_bit_is_clear( reg, bit ) while( test_bit( reg, bit ) )
#define toggle_bit(reg, bit) (reg ^= (1 << bit))

#endif /* BIT_MACROS_H_ */

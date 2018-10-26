
#ifndef  SRAM_H_
#define  SRAM_H_

int SRAM_write(uint16_t address, char data);
char SRAM_read(uint16_t address);

void SRAM_test(void);

#endif

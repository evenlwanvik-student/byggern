char MCP2515_read(ad_byte);

void MCP2515_write(char ad_byte, char cData);

void MCP2515_request_to_send(char rts_data);

char MCP2515_read_status();

void MCP2515_bit_modify (char address, char mask, char data);

void MCP2515_reset(void);

typedef struct {
    volatile unsigned short int id;
    volatile unsigned char length;
    volatile char data[8];
} can_msg_t;

void CAN_error();

uint8_t CAN_transmit_complete();

void CAN_int_vect();

void CAN_send(can_msg_t* msg);

void CAN_read(can_msg_t* msg_read);

void CAN_init(void);

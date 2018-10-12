typedef struct {
    unsigned short int id;
    unsigned char length;
    char data[8];
} can_msg;

void CAN_send(can_msg*);

can_msg_t CAN_receive(void);


void CAN_init(void);

#ifndef PINGIS_GAME_H_
#define PINGIS_GAME_H_

typedef enum {
    NORMAL,
    PINGIS,
    SNAKE,
    SOUND,
    ANIMATION
} states_t;

void pingis_init();
void pingis_play();
void pingis_destructor();

#endif /* PINGIS_GAME_H_ */

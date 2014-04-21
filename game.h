#ifndef __GAME_H__
#define __GAME_H__

#include "life.h"

typedef struct {
	life_t *life;
} game_t;

game_t *game_create(void);

void game_destroy(game_t *game);

void game_main_loop(game_t *game);

#endif /* __GAME_H__ */


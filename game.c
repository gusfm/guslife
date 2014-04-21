#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

#include "game.h"

int game_screen_init(void)
{
	int ret;
	WINDOW *screen;

	screen = initscr();
	if (screen == NULL) {
		printf("Error: initscr=NULL\n");
		return -1;
	}

	ret = start_color();
	if (ret < 0) {
		printf("Error: keypad=%d\n", ret);
		return -1;
	}

	ret = raw();
	if (ret < 0) {
		printf("Error: raw=%d\n", ret);
		return -1;
	}

	ret = noecho();
	if (ret < 0) {
		printf("Error: noecho=%d\n", ret);
		return -1;
	}

	ret = curs_set(0);
	if (ret < 0) {
		printf("Error: curs_set=%d\n", ret);
		return -1;
	}

	ret = nodelay(stdscr, TRUE);
	if (ret < 0) {
		printf("Error: nodelay=%d\n", ret);
		return -1;
	}

	ret = keypad(stdscr, TRUE);
	if (ret < 0) {
		printf("Error: keypad=%d\n", ret);
		return -1;
	}

	init_pair(0, COLOR_WHITE, COLOR_BLACK);
	init_pair(1, COLOR_BLACK, COLOR_WHITE);

	return 0;
}

game_t *game_create(void)
{
	game_t *game;
	int size_y, size_x, ret;

	/* Initalize screen. */
	ret = game_screen_init();
	if (ret < 0) {
		return NULL;
	}

	/* Alocate game memory. */
	game = (game_t *)malloc(sizeof(game_t));
	if (game == NULL) {
		return NULL;
	}

	/* Get max yx screen size. */
	getmaxyx(stdscr, size_y, size_x);

	/* Create game of life data. */
	game->life = life_create(size_y, size_x);

	return game;
}

void game_destroy(game_t *game)
{
	endwin();
	life_destroy(game->life);
	free(game);
}

static void game_draw_life(life_t *life)
{
	int y, x;
	for (y = 0; y < life->size_y; y++) {
		for (x = 0; x < life->size_x; x++) {
			if (life->cell_matrix_cur[y][x] == ALIVE)
				mvprintw(y, x, "%c", ' ');
		}
	}
}

static void game_draw_screen(game_t *game)
{
	attron(COLOR_PAIR(0));
	clear();
	attron(COLOR_PAIR(1));
	game_draw_life(game->life);
	mvprintw(0, 0, "Press q to quit");
	refresh();
}

static int game_process_key(void)
{
	int key;

	key = getch();

	if (key != ERR) {
		mvprintw(0, 0, "key=%d", key);
		refresh();
	}
	if (key == 'q' || key == 'Q' || key == 27 || key == 17) {
		return 1;
	}

	return 0;
}

static void game_process_turn(game_t *game)
{
	life_evolve(game->life);
}

void game_main_loop(game_t *game)
{
	while (game_process_key() == 0) {
		game_draw_screen(game);
		game_process_turn(game);
		usleep(500000);
	}
}


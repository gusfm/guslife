#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "game.h"

game_t* game;

void destroy(int signum)
{
    game_destroy(game);
    exit(signum);
}

void signal_callback_handler(int signum)
{
    printf("Caught signal %d\n", signum);
    destroy(signum);
}

int main(int argc, char const* argv[])
{
    /* Register signal haldler callback */
    signal(SIGINT, signal_callback_handler);
    signal(SIGKILL, signal_callback_handler);
    signal(SIGTERM, signal_callback_handler);

    game = game_create();
    if (game == NULL) {
        printf("Error: could not create game!\n");
        exit(EXIT_FAILURE);
    }
    game_main_loop(game);
    game_destroy(game);

    return 0;
}

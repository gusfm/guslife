#include <stdlib.h>
#include <time.h>

#include "life.h"

/*
 * A cell C is represented by a 1 when alive or 0 when dead, in an m-by-m
 * square array of cells. We calculate N - the sum of live cells in C's
 * eight-location neighbourhood, then cell C is alive or dead in the next
 * generation based on the following table:
 * C    N                   new C
 * 1    0,1                 0 # Lonely
 * 1    4,5,6,7,8           0 # Overcrowded
 * 1    2,3                 1 # Lives
 * 0    3                   1 # It takes three to give birth!
 * 0    0,1,2,4,5,6,7,8     0 # Barren
 *
 * Reference: https://rosettacode.org/wiki/Conway%27s_Game_of_Life
 */

static void life_init(life_t *life)
{
    int y, x;

    srand(time(NULL));

    for (y = 0; y < life->size_y; y++) {
        for (x = 0; x < life->size_x; x++) {
            if (rand() % 10 == 0)
                life->cell_matrix_cur[y][x] = ALIVE;
        }
    }
}

life_t *life_create(int size_y, int size_x)
{
    life_t *life;
    int i;

    /* Alocate memory */
    life = (life_t *)malloc(sizeof(life_t));
    life->cell_matrix_cur =
        (unsigned char **)malloc(sizeof(unsigned char *) * size_y);
    life->cell_matrix_new =
        (unsigned char **)malloc(sizeof(unsigned char *) * size_y);
    for (i = 0; i < size_y; i++) {
        life->cell_matrix_cur[i] =
            (unsigned char *)malloc(sizeof(unsigned char) * size_x);
        life->cell_matrix_new[i] =
            (unsigned char *)malloc(sizeof(unsigned char) * size_x);
    }

    life->size_y = size_y;
    life->size_x = size_x;

    life_init(life);

    return life;
}

void life_destroy(life_t *life)
{
    int i;
    for (i = 0; i < life->size_y; i++) {
        free(life->cell_matrix_cur[i]);
        free(life->cell_matrix_new[i]);
    }
    free(life->cell_matrix_cur);
    free(life->cell_matrix_new);
    free(life);
}

static void life_evolve_cell(life_t *life, int y, int x)
{
    int n, h, w, ytmp, xtmp;

    n = 0;
    h = life->size_y;
    w = life->size_x;

    /* Calculate number of alive neighbour cells. */
    for (ytmp = y - 1; ytmp <= y + 1; ytmp++)
        for (xtmp = x - 1; xtmp <= x + 1; xtmp++)
            if (life->cell_matrix_cur[(ytmp + h) % h][(xtmp + w) % w] == ALIVE)
                n++;

    /* TODO: check if need this */
    if (life->cell_matrix_cur[y][x] == ALIVE)
        n--;

    life->cell_matrix_new[y][x] =
        (n == 3 || (n == 2 && life->cell_matrix_cur[y][x]));
}

void life_evolve(life_t *life)
{
    int y, x;
    unsigned char **old_ptr;

    /* Process next turn. */
    for (y = 0; y < life->size_y; y++) {
        for (x = 0; x < life->size_x; x++) {
            life_evolve_cell(life, y, x);
        }
    }

    /* Swap buffers. */
    old_ptr = life->cell_matrix_cur;
    life->cell_matrix_cur = life->cell_matrix_new;
    life->cell_matrix_new = old_ptr;
}

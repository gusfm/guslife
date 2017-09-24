#ifndef __LIFE_H__
#define __LIFE_H__

typedef struct {
    int size_y;
    int size_x;
    unsigned char **cell_matrix_cur;
    unsigned char **cell_matrix_new;
} life_t;

enum { DEAD, ALIVE };

life_t *life_create(int size_y, int size_x);

void life_destroy(life_t *life);

void life_evolve(life_t *life);

#endif /* __LIFE_H__ */

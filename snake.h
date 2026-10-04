#ifndef SNAKE_H
#define SNAKE_H

/* Representation du serpent par liste chainee, avec une position par cellule. */

typedef struct Cell {
    int row;
    int col;
    struct Cell *next;
} Cell;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct {
    Cell *head;
    Cell *tail;
    Direction direction;
} Snake;

void init_snake(Snake *snake, int row, int col);
void print_snake(Snake *snake);
void get_next_position(Snake *snake, int *new_row, int *new_col, int size);
void add_head(Snake *snake, int row, int col);
void remove_tail(Snake *snake);
void change_direction(Snake *snake, Direction new_direction);
int snake_contains(Snake *snake, int row, int col);

#endif

#ifndef MAP_H
#define MAP_H

/* Interface de la carte, qui ne contient que EMPTY, WALL et FRUIT. */

#define EMPTY 0
#define WALL 1
#define FRUIT 2

extern int **map;

void init_map(int size);
void print_map(int size);
void set_cell(int row, int col, int value);
int get_cell(int row, int col);
void free_map(int size);

#endif

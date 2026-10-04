#ifndef GAME_H
#define GAME_H

/* Etat et interface de la logique du jeu : score, niveaux, fruits et obstacles. */
#include "snake.h"

typedef struct {
    int score;
    int level;
    int fruits_eaten;
    int speed;
    int level_changed;
} GameState;

void init_game_state(GameState *game);
int fruit_position_valid(int size, Snake *snake, int row, int col);
int move_snake(Snake *snake, int size, GameState *game);
int is_wall(int row, int col);
void place_fruit(int size, Snake *snake);
int is_near_snake_head(Snake *snake, int row, int col);
int can_place_wall(int size, Snake *snake, int row, int col, int length, int direction);
void place_walls(int size, Snake *snake, int number);
void level_up(GameState *game, Snake *snake, int size);

#endif

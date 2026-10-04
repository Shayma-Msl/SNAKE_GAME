#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "map.h"
#include "snake.h"

static void accelerate(GameState *game, int amount);
static void add_wall_cells(int row, int col, int length, int direction);
int move_snake(Snake *snake, int size, GameState *game);
int fruit_position_valid(int size, Snake *snake, int row, int col);
void place_fruit(int size, Snake *snake);
int is_wall(int row, int col);
int is_near_snake_head(Snake *snake, int row, int col);
int can_place_wall(int size, Snake *snake, int row, int col, int length, int direction);
void place_walls(int size, Snake *snake, int number);
void init_game_state(GameState *game);
void level_up(GameState *game, Snake *snake, int size);

/* Reduit le delai entre deux mouvements sans descendre sous 60 ms. */
static void accelerate(GameState *game, int amount) {
    if (game->speed > 60) {
        game->speed -= amount;
        if (game->speed < 60) game->speed = 60;
    }
}

/* Gere un tour complet : collision, mouvement, fruit, score, vitesse et niveau. */
int move_snake(Snake *snake, int size, GameState *game) {
    int new_row;
    int new_col;
    int fruit;
    get_next_position(snake, &new_row, &new_col, size);
    if (is_wall(new_row, new_col)) return 0;
    fruit = get_cell(new_row, new_col) == FRUIT;
    if (!fruit) remove_tail(snake);
    if (snake_contains(snake, new_row, new_col)) return 0;
    add_head(snake, new_row, new_col);
    if (fruit) {
        game->fruits_eaten++;
        game->score += 10;
        set_cell(new_row, new_col, EMPTY);
        accelerate(game, 8);
        if (game->fruits_eaten % 3 == 0) level_up(game, snake, size);
        place_fruit(size, snake);
    }
    return 1;
}

/* Verifie que le fruit est sur une case libre et qu'il reste accessible. */
int fruit_position_valid(int size, Snake *snake, int row, int col) {
    int free_neighbors = 0;
    if (get_cell(row, col) != EMPTY) return 0;
    if (snake_contains(snake, row, col)) return 0;
    if (row > 0 && get_cell(row - 1, col) != WALL) free_neighbors++;
    if (row < size - 1 && get_cell(row + 1, col) != WALL) free_neighbors++;
    if (col > 0 && get_cell(row, col - 1) != WALL) free_neighbors++;
    if (col < size - 1 && get_cell(row, col + 1) != WALL) free_neighbors++;
    return free_neighbors > 0;
}

/* Choisit aleatoirement une position valide puis place le fruit. */
void place_fruit(int size, Snake *snake) {
    int row;
    int col;
    do {
        row = rand() % size;
        col = rand() % size;
    } while (!fruit_position_valid(size, snake, row, col));
    set_cell(row, col, FRUIT);
}

/* Centralise le test de collision avec un mur. */
int is_wall(int row, int col) {
    return get_cell(row, col) == WALL;
}

/* Evite de creer un nouvel obstacle trop pres de la tete. */
int is_near_snake_head(Snake *snake, int row, int col) {
    int row_distance = row - snake->head->row;
    int col_distance = col - snake->head->col;
    if (row_distance < 0) row_distance = -row_distance;
    if (col_distance < 0) col_distance = -col_distance;
    return row_distance <= 2 && col_distance <= 2;
}

/* Verifie toutes les cases d'un futur mur avant de modifier la carte. */
int can_place_wall(int size, Snake *snake, int row, int col, int length, int direction) {
    int i;
    int wall_row;
    int wall_col;
    for (i = 0; i < length; i++) {
        wall_row = direction == 0 ? row : row + i;
        wall_col = direction == 0 ? col + i : col;
        if (wall_row >= size || wall_col >= size) return 0;
        if (get_cell(wall_row, wall_col) != EMPTY) return 0;
        if (snake_contains(snake, wall_row, wall_col)) return 0;
        if (is_near_snake_head(snake, wall_row, wall_col)) return 0;
    }
    return 1;
}

/* Ajoute les cellules d'un mur horizontal ou vertical deja valide. */
static void add_wall_cells(int row, int col, int length, int direction) {
    int i;
    for (i = 0; i < length; i++) {
        if (direction == 0) set_cell(row, col + i, WALL);
        else set_cell(row + i, col, WALL);
    }
}

/* Genere des murs aleatoires sans recouvrir le serpent ni les autres elements. */
void place_walls(int size, Snake *snake, int number) {
    int wall = 0;
    int row;
    int col;
    int length;
    int direction;
    while (wall < number) {
        row = rand() % size;
        col = rand() % size;
        length = 2 + rand() % 4;
        direction = rand() % 2;
        if (can_place_wall(size, snake, row, col, length, direction)) {
            add_wall_cells(row, col, length, direction);
            wall++;
        }
    }
}

/* Initialise le score, le niveau, la vitesse et les compteurs de progression. */
void init_game_state(GameState *game) {
    game->score = 0;
    game->level = 1;
    game->fruits_eaten = 0;
    game->speed = 180;
    game->level_changed = 0;
}

/* Augmente la difficulte en accelerant le serpent et en ajoutant un mur. */
void level_up(GameState *game, Snake *snake, int size) {
    game->level++;
    accelerate(game, 10);
    place_walls(size, snake, 1);
    game->level_changed = 1;
}

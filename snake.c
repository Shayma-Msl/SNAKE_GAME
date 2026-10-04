#include <stdio.h>
#include <stdlib.h>
#include "snake.h"

void init_snake(Snake *snake, int row, int col);
void print_snake(Snake *snake);
void get_next_position(Snake *snake, int *new_row, int *new_col, int size);
void add_head(Snake *snake, int row, int col);
void remove_tail(Snake *snake);
void change_direction(Snake *snake, Direction new_direction);
int snake_contains(Snake *snake, int row, int col);

/* Construit le serpent initial sous forme d'une liste chainee de trois cellules. */
void init_snake(Snake *snake, int row, int col) {
    Cell *head = malloc(sizeof(Cell));
    Cell *body = malloc(sizeof(Cell));
    Cell *tail = malloc(sizeof(Cell));
    head->row = row;
    head->col = col;
    head->next = body;
    body->row = row;
    body->col = col - 1;
    body->next = tail;
    tail->row = row;
    tail->col = col - 2;
    tail->next = NULL;
    snake->head = head;
    snake->tail = tail;
    snake->direction = RIGHT;
}

/* Fonction de diagnostic qui parcourt et affiche la liste chainee. */
void print_snake(Snake *snake) {
    Cell *current = snake->head;
    while (current != NULL) {
        printf("(%d, %d) ", current->row, current->col);
        current = current->next;
    }
    printf("\n");
}

/* Calcule la prochaine position et applique le passage d'un bord au bord oppose. */
void get_next_position(Snake *snake, int *new_row, int *new_col, int size) {
    *new_row = snake->head->row;
    *new_col = snake->head->col;
    if (snake->direction == UP) (*new_row)--;
    else if (snake->direction == DOWN) (*new_row)++;
    else if (snake->direction == LEFT) (*new_col)--;
    else if (snake->direction == RIGHT) (*new_col)++;
    if (*new_row < 0) *new_row = size - 1;
    else if (*new_row >= size) *new_row = 0;
    if (*new_col < 0) *new_col = size - 1;
    else if (*new_col >= size) *new_col = 0;
}

/* Ajoute une nouvelle cellule en tete de liste pour avancer ou grandir. */
void add_head(Snake *snake, int row, int col) {
    Cell *new_cell = malloc(sizeof(Cell));
    new_cell->row = row;
    new_cell->col = col;
    new_cell->next = snake->head;
    snake->head = new_cell;
}

/* Supprime la derniere cellule lors d'un mouvement sans fruit. */
void remove_tail(Snake *snake) {
    Cell *current = snake->head;
    while (current->next != snake->tail) current = current->next;
    free(snake->tail);
    snake->tail = current;
    snake->tail->next = NULL;
}

/* Interdit le demi-tour immediat pour eviter de repartir dans son propre corps. */
void change_direction(Snake *snake, Direction new_direction) {
    if (snake->direction == UP && new_direction != DOWN) snake->direction = new_direction;
    else if (snake->direction == DOWN && new_direction != UP) snake->direction = new_direction;
    else if (snake->direction == LEFT && new_direction != RIGHT) snake->direction = new_direction;
    else if (snake->direction == RIGHT && new_direction != LEFT) snake->direction = new_direction;
}

/* Parcourt la liste pour tester si une position est occupee par le serpent. */
int snake_contains(Snake *snake, int row, int col) {
    Cell *current = snake->head;
    while (current != NULL) {
        if (current->row == row && current->col == col) return 1;
        current = current->next;
    }
    return 0;
}

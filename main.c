#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"
#include "snake.h"
#include "game.h"
#include "display.h"
#include "graphics/graphics.h"

#define DEFAULT_SIZE 20

static int read_size(int argc, char *argv[]);
static void draw_game(Snake *snake, GameState *game, int size);
static void read_direction(Snake *snake);
static void wait_move(Snake *snake, int speed);
static void show_level(GameState *game, Snake *snake, int size);
static void game_loop(Snake *snake, GameState *game, int size);
int main(int argc, char *argv[]);

/* Lit la taille donnee en argument, sinon utilise la taille par defaut. */
static int read_size(int argc, char *argv[]) {
    if (argc == 2) return atoi(argv[1]);
    return DEFAULT_SIZE;
}

/* Regroupe le dessin de la carte, du serpent et du HUD avant l'affichage. */
static void draw_game(Snake *snake, GameState *game, int size) {
    draw_map(size);
    draw_snake(snake);
    draw_hud(game, size);
    affiche_all();
}

/* Traduit les fleches du clavier en directions du serpent. */
static void read_direction(Snake *snake) {
    POINT arrow = get_arrow();
    if (arrow.x < 0) change_direction(snake, LEFT);
    else if (arrow.x > 0) change_direction(snake, RIGHT);
    else if (arrow.y > 0) change_direction(snake, DOWN);
    else if (arrow.y < 0) change_direction(snake, UP);
}

/* Attend le prochain mouvement tout en lisant regulierement le clavier. */
static void wait_move(Snake *snake, int speed) {
    int elapsed = 0;
    while (elapsed < speed) {
        read_direction(snake);
        attendre(10);
        elapsed += 10;
    }
}

/* Affiche la transition de niveau puis restaure le plateau. */
static void show_level(GameState *game, Snake *snake, int size) {
    draw_level_screen(game->level, size);
    attendre(1500);
    draw_game(snake, game, size);
    attendre(1000);
    game->level_changed = 0;
}

/* Boucle principale qui enchaine saisie, mouvement, progression et affichage. */
static void game_loop(Snake *snake, GameState *game, int size) {
    int running = 1;
    while (running) {
        wait_move(snake, game->speed);
        running = move_snake(snake, size, game);
        if (running && game->level_changed) show_level(game, snake, size);
        if (running) draw_game(snake, game, size);
    }
    draw_game_over(game, size);
}

/* Initialise les modules, lance la partie puis libere la memoire de la carte. */
int main(int argc, char *argv[]) {
    int size = read_size(argc, argv);
    Snake snake;
    GameState game;
    srand(time(NULL));
    init_map(size);
    init_snake(&snake, 2, 3);
    init_game_state(&game);
    place_walls(size, &snake, game.level);
    place_fruit(size, &snake);
    init_display(size);
    affiche_auto_off();
    draw_welcome_screen(size);
    wait_start_click(size);
    show_level(&game, &snake, size);
    game_loop(&snake, &game, size);
    wait_escape();
    free_map(size);
    return 0;
}

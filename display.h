#ifndef DISPLAY_H
#define DISPLAY_H

/* Interface publique du module d'affichage. */
#include "snake.h"
#include "game.h"

void init_display(int size);
void draw_map(int size);
void draw_snake(Snake *snake);
void draw_level_screen(int level, int size);
void draw_hud(GameState *game, int size);
void draw_welcome_screen(int size);
void wait_start_click(int size);
void draw_game_over(GameState *game, int size);

#endif

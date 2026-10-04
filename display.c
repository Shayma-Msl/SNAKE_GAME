#include <stdio.h>
#include "display.h"
#include "graphics/graphics.h"
#include "map.h"

#define CELL_SIZE 30
#define HUD_HEIGHT 40
#define HUD_GAP 0

static COULEUR snake_color(void);
static void cell_points(int row, int col, POINT *p1, POINT *p2);
static void draw_checkerboard(int size, int height);
static void draw_apple(int row, int col);
static void draw_wall(int row, int col);
static void find_tail(Snake *snake, Cell **tail, Cell **before_tail);
static void draw_body(Snake *snake, Cell *tail);
static void head_face(Direction direction, POINT center, POINT *eye1, POINT *eye2, POINT *m1, POINT *m2, POINT *m3);
static void connect_head(Direction direction, POINT p1, POINT p2, POINT center, COULEUR color);
static void draw_face(POINT eye1, POINT eye2, POINT m1, POINT m2, POINT m3);
static void draw_head(Snake *snake);
static void draw_tail(Cell *tail, Cell *before_tail);
static void draw_menu_decorations(int size);
static void draw_start_button(int width, int height);
static void draw_menu_snake(int width, int height);
static void draw_dead_eye(POINT eye);
static void draw_dead_snake(int width, int height);
void init_display(int size);
void draw_map(int size);
void draw_snake(Snake *snake);
void draw_level_screen(int level, int size);
void draw_hud(GameState *game, int size);
void draw_welcome_screen(int size);
void wait_start_click(int size);
void draw_game_over(GameState *game, int size);

static COULEUR snake_color(void) {
    return couleur_RGB(40, 110, 220);
}

/* Convertit une ligne et une colonne en coordonnees graphiques. */
static void cell_points(int row, int col, POINT *p1, POINT *p2) {
    p1->x = col * CELL_SIZE;
    p1->y = row * CELL_SIZE;
    p2->x = p1->x + CELL_SIZE;
    p2->y = p1->y + CELL_SIZE;
}

/* Dessine le fond en damier sans stocker d'information graphique dans la carte. */
static void draw_checkerboard(int size, int height) {
    int row;
    int col;
    POINT p1;
    POINT p2;
    COULEUR light = couleur_RGB(170, 215, 81);
    COULEUR dark = couleur_RGB(162, 209, 73);
    for (row = 0; row <= height / CELL_SIZE; row++) {
        for (col = 0; col < size; col++) {
            cell_points(row, col, &p1, &p2);
            draw_fill_rectangle(p1, p2, (row + col) % 2 == 0 ? light : dark);
        }
    }
}

/* Dimensionne la fenetre a partir de la taille dynamique de la carte. */
void init_display(int size) {
    init_graphics(size * CELL_SIZE, size * CELL_SIZE + HUD_GAP + HUD_HEIGHT);
}

/* Dessine un fruit avec les primitives de la bibliotheque graphics. */
static void draw_apple(int row, int col) {
    POINT center;
    POINT stem1;
    POINT stem2;
    POINT leaf1;
    POINT leaf2;
    POINT leaf3;
    center.x = col * CELL_SIZE + CELL_SIZE / 2;
    center.y = row * CELL_SIZE + CELL_SIZE / 2;
    draw_fill_circle(center, 13, couleur_RGB(220, 40, 40));
    stem1.x = center.x;
    stem1.y = center.y + 10;
    stem2.x = center.x + 2;
    stem2.y = center.y + 15;
    draw_line(stem1, stem2, couleur_RGB(100, 60, 25));
    leaf1.x = center.x + 2;
    leaf1.y = center.y + 12;
    leaf2.x = center.x + 10;
    leaf2.y = center.y + 14;
    leaf3.x = center.x + 5;
    leaf3.y = center.y + 9;
    draw_fill_triangle(leaf1, leaf2, leaf3, couleur_RGB(40, 130, 50));
}

/* Dessine graphiquement une case WALL sous forme de briques. */
static void draw_wall(int row, int col) {
    POINT p1;
    POINT p2;
    POINT a;
    POINT b;
    COULEUR wall_brown = couleur_RGB(150, 85, 45);
    COULEUR wall_dark = couleur_RGB(90, 50, 30);
    cell_points(row, col, &p1, &p2);
    draw_fill_rectangle(p1, p2, wall_brown);
    draw_rectangle(p1, p2, wall_dark);
    a.x = p1.x;
    a.y = p1.y + CELL_SIZE / 2;
    b.x = p2.x;
    b.y = a.y;
    draw_line(a, b, wall_dark);
    a.x = b.x = p1.x + CELL_SIZE / 3;
    a.y = p1.y + CELL_SIZE / 2;
    b.y = p2.y;
    draw_line(a, b, wall_dark);
    a.x = b.x = p1.x + 2 * CELL_SIZE / 3;
    a.y = p1.y;
    b.y = p1.y + CELL_SIZE / 2;
    draw_line(a, b, wall_dark);
}

/* Parcourt la carte logique et dessine chaque element correspondant. */
void draw_map(int size) {
    int row;
    int col;
    int value;
    POINT p1;
    POINT p2;
    COULEUR light = couleur_RGB(170, 215, 81);
    COULEUR dark = couleur_RGB(162, 209, 73);
    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            cell_points(row, col, &p1, &p2);
            draw_fill_rectangle(p1, p2, (row + col) % 2 == 0 ? light : dark);
            value = get_cell(row, col);
            if (value == WALL) draw_wall(row, col);
            else if (value == FRUIT) draw_apple(row, col);
        }
    }
}

/* Retrouve la queue et son predecesseur dans la liste chainee. */
static void find_tail(Snake *snake, Cell **tail, Cell **before_tail) {
    *tail = snake->head;
    *before_tail = NULL;
    while ((*tail)->next != NULL) {
        *before_tail = *tail;
        *tail = (*tail)->next;
    }
}

/* Dessine les cellules intermediaires du serpent. */
static void draw_body(Snake *snake, Cell *tail) {
    Cell *current = snake->head->next;
    POINT p1;
    POINT p2;
    while (current != NULL) {
        if (current != tail) {
            cell_points(current->row, current->col, &p1, &p2);
            draw_fill_rectangle(p1, p2, snake_color());
        }
        current = current->next;
    }
}

/* Relie la tete ronde au corps selon la direction courante. */
static void connect_head(Direction direction, POINT p1, POINT p2, POINT center, COULEUR color) {
    if (direction == RIGHT) p2.x = center.x;
    else if (direction == LEFT) p1.x = center.x;
    else if (direction == UP) p1.y = center.y;
    else p2.y = center.y;
    draw_fill_rectangle(p1, p2, color);
}

/* Calcule les positions du visage selon l'orientation de la tete. */
static void head_face(Direction d, POINT c, POINT *e1, POINT *e2, POINT *m1, POINT *m2, POINT *m3) {
    if (d == RIGHT || d == LEFT) {
        int s = d == RIGHT ? 1 : -1;
        e1->x = e2->x = c.x + 6 * s;
        e1->y = c.y + 6;
        e2->y = c.y - 6;
        m1->x = m3->x = c.x + 7 * s;
        m2->x = c.x + 10 * s;
        m1->y = c.y + 3;
        m2->y = c.y;
        m3->y = c.y - 3;
    } else {
        int s = d == DOWN ? 1 : -1;
        e1->x = c.x - 6;
        e2->x = c.x + 6;
        e1->y = e2->y = c.y + 6 * s;
        m1->x = c.x - 3;
        m2->x = c.x;
        m3->x = c.x + 3;
        m1->y = m3->y = c.y + 7 * s;
        m2->y = c.y + 10 * s;
    }
}

/* Dessine les yeux et la bouche aux positions calculees. */
static void draw_face(POINT e1, POINT e2, POINT m1, POINT m2, POINT m3) {
    draw_fill_circle(e1, 3, blanc);
    draw_fill_circle(e2, 3, blanc);
    draw_fill_circle(e1, 1, noir);
    draw_fill_circle(e2, 1, noir);
    draw_line(m1, m2, noir);
    draw_line(m2, m3, noir);
}

/* Dessine la tete et son visage. */
static void draw_head(Snake *snake) {
    POINT p1;
    POINT p2;
    POINT center;
    POINT e1;
    POINT e2;
    POINT m1;
    POINT m2;
    POINT m3;
    cell_points(snake->head->row, snake->head->col, &p1, &p2);
    center.x = p1.x + CELL_SIZE / 2;
    center.y = p1.y + CELL_SIZE / 2;
    draw_fill_circle(center, CELL_SIZE / 2, snake_color());
    connect_head(snake->direction, p1, p2, center, snake_color());
    head_face(snake->direction, center, &e1, &e2, &m1, &m2, &m3);
    draw_face(e1, e2, m1, m2, m3);
}

/* Oriente graphiquement la queue vers l'avant-derniere cellule. */
static void draw_tail(Cell *tail, Cell *before_tail) {
    POINT p1;
    POINT p2;
    POINT center;
    cell_points(tail->row, tail->col, &p1, &p2);
    center.x = p1.x + CELL_SIZE / 2;
    center.y = p1.y + CELL_SIZE / 2;
    draw_fill_circle(center, CELL_SIZE / 2, snake_color());
    if (before_tail->col > tail->col) p1.x = center.x;
    else if (before_tail->col < tail->col) p2.x = center.x;
    else if (before_tail->row > tail->row) p1.y = center.y;
    else p2.y = center.y;
    draw_fill_rectangle(p1, p2, snake_color());
}

/* Reconstruit l'affichage du serpent a partir de sa liste chainee. */
void draw_snake(Snake *snake) {
    Cell *tail;
    Cell *before_tail;
    find_tail(snake, &tail, &before_tail);
    draw_body(snake, tail);
    draw_head(snake);
    draw_tail(tail, before_tail);
}

/* Affiche l'ecran de transition lors d'un changement de niveau. */
void draw_level_screen(int level, int size) {
    char number[20];
    POINT title;
    POINT number_pos;
    int width = size * CELL_SIZE;
    int height = size * CELL_SIZE + HUD_GAP + HUD_HEIGHT;
    sprintf(number, "%d", level);
    draw_checkerboard(size, height);
    title.x = width / 2 - 90;
    title.y = height / 2 + 45;
    number_pos.x = width / 2 - 20;
    number_pos.y = height / 2 - 25;
    aff_pol("LEVEL", 40, title, blanc);
    aff_pol(number, 55, number_pos, snake_color());
    affiche_all();
}

/* Affiche le score et le niveau en dehors des donnees de la carte. */
void draw_hud(GameState *game, int size) {
    char level_text[50];
    char score_text[50];
    POINT p1;
    POINT p2;
    POINT level_pos;
    POINT score_pos;
    int board_height = size * CELL_SIZE;
    int width = size * CELL_SIZE;
    p1.x = 0;
    p1.y = board_height + HUD_GAP;
    p2.x = width;
    p2.y = board_height + HUD_GAP + HUD_HEIGHT;
    draw_fill_rectangle(p1, p2, couleur_RGB(25, 35, 45));
    p2.y = p1.y;
    draw_line(p1, p2, snake_color());
    sprintf(level_text, "LEVEL %d", game->level);
    sprintf(score_text, "SCORE: %d", game->score);
    level_pos.x = 20;
    level_pos.y = board_height + 28;
    score_pos.x = width - 180;
    score_pos.y = board_height + 28;
    aff_pol(level_text, 18, level_pos, blanc);
    aff_pol(score_text, 18, score_pos, blanc);
}

/* Reutilise les elements du jeu pour decorer l'ecran d'accueil. */
static void draw_menu_decorations(int size) {
    draw_apple(size - 4, 3);
    draw_apple(3, size - 4);
    draw_wall(size - 4, size - 6);
    draw_wall(size - 4, size - 5);
    draw_wall(size - 4, size - 4);
    draw_wall(3, 2);
    draw_wall(3, 3);
    draw_wall(3, 4);
}

/* Dessine le bouton START. */
static void draw_start_button(int width, int height) {
    POINT tl;
    POINT br;
    POINT p1;
    POINT p2;
    POINT text;
    tl.x = width / 2 - 70;
    tl.y = height / 2 + 20;
    br.x = width / 2 + 70;
    br.y = height / 2 - 30;
    p1.x = br.x + 5;
    p1.y = br.y - 5;
    p2.x = tl.x + 5;
    p2.y = tl.y - 5;
    draw_fill_rectangle(p1, p2, couleur_RGB(15, 25, 30));
    draw_fill_rectangle(br, tl, snake_color());
    p1.x = br.x + 4;
    p1.y = br.y + 4;
    p2.x = tl.x - 4;
    p2.y = tl.y - 4;
    draw_fill_rectangle(p1, p2, couleur_RGB(30, 45, 55));
    text.x = width / 2 - 40;
    text.y = height / 2 + 4;
    aff_pol("START", 22, text, blanc);
}

/* Dessine le serpent decoratif de l'ecran d'accueil. */
static void draw_menu_snake(int width, int height) {
    POINT p1;
    POINT p2;
    POINT tail;
    POINT head;
    POINT e1;
    POINT e2;
    POINT m1;
    POINT m2;
    POINT m3;
    p1.x = width / 2 - 90;
    p1.y = height / 2 - 130;
    p2.x = width / 2 + 60;
    p2.y = height / 2 - 100;
    draw_fill_rectangle(p1, p2, snake_color());
    tail.x = width / 2 - 90;
    tail.y = height / 2 - 115;
    head.x = width / 2 + 60;
    head.y = tail.y;
    draw_fill_circle(tail, 15, snake_color());
    draw_fill_circle(head, 15, snake_color());
    p1.x = tail.x;
    p1.y = tail.y - 15;
    p2.x = tail.x + 15;
    p2.y = tail.y + 15;
    draw_fill_rectangle(p1, p2, snake_color());
    head_face(RIGHT, head, &e1, &e2, &m1, &m2, &m3);
    draw_face(e1, e2, m1, m2, m3);
}

/* Compose puis affiche l'ecran d'accueil. */
void draw_welcome_screen(int size) {
    POINT title;
    int width = size * CELL_SIZE;
    int height = size * CELL_SIZE + HUD_GAP + HUD_HEIGHT;
    draw_checkerboard(size, height);
    draw_menu_decorations(size);
    title.x = width / 2 - 83;
    title.y = height / 2 + 100;
    aff_pol("SNAKE", 50, title, snake_color());
    draw_start_button(width, height);
    draw_menu_snake(width, height);
    affiche_all();
}

/* Attend un clic situe dans les limites du bouton START. */
void wait_start_click(int size) {
    POINT click;
    int width = size * CELL_SIZE;
    int height = size * CELL_SIZE + HUD_GAP + HUD_HEIGHT;
    int left = width / 2 - 70;
    int right = width / 2 + 70;
    int top = height / 2 + 20;
    int bottom = height / 2 - 30;
    int clicked = 0;
    while (!clicked) {
        click = wait_clic();
        clicked = click.x >= left && click.x <= right && click.y >= bottom && click.y <= top;
    }
}

/* Dessine un oeil en croix pour l'ecran de fin. */
static void draw_dead_eye(POINT eye) {
    POINT p1;
    POINT p2;
    p1.x = eye.x - 3;
    p1.y = eye.y - 3;
    p2.x = eye.x + 3;
    p2.y = eye.y + 3;
    draw_line(p1, p2, blanc);
    p1.y = eye.y + 3;
    p2.y = eye.y - 3;
    draw_line(p1, p2, blanc);
}

/* Dessine le serpent decoratif de l'ecran GAME OVER. */
static void draw_dead_snake(int width, int height) {
    POINT p1;
    POINT p2;
    POINT head;
    POINT eye1;
    POINT eye2;
    p1.x = width / 2 - 60;
    p1.y = height / 2 - 45;
    p2.x = width / 2 + 45;
    p2.y = height / 2 - 15;
    draw_fill_rectangle(p1, p2, snake_color());
    head.x = width / 2 + 45;
    head.y = height / 2 - 30;
    draw_fill_circle(head, 15, snake_color());
    eye1.x = eye2.x = head.x + 4;
    eye1.y = head.y + 6;
    eye2.y = head.y - 6;
    draw_dead_eye(eye1);
    draw_dead_eye(eye2);
}

/* Compose l'ecran final et affiche le score obtenu. */
void draw_game_over(GameState *game, int size) {
    char score[50];
    POINT game_over;
    POINT score_pos;
    int width = size * CELL_SIZE;
    int height = size * CELL_SIZE + HUD_GAP + HUD_HEIGHT;
    sprintf(score, "SCORE: %d", game->score);
    draw_checkerboard(size, height);
    game_over.x = width / 2 - 145;
    game_over.y = height / 2 + 100;
    score_pos.x = width / 2 - 80;
    score_pos.y = height / 2 + 35;
    aff_pol("GAME OVER", 40, game_over, couleur_RGB(210, 45, 45));
    aff_pol(score, 22, score_pos, blanc);
    draw_dead_snake(width, height);
    affiche_all();
}

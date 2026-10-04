#include <stdio.h>
#include <stdlib.h>
#include "map.h"

void init_map(int size);
void print_map(int size);
void free_map(int size);
void set_cell(int row, int col, int value);
int get_cell(int row, int col);

/* Seule variable globale du projet. La carte contient les elements du monde mais pas le serpent. */
int **map;

/* Alloue dynamiquement la carte car sa taille est choisie au lancement. */
void init_map(int size) {
    int row;
    int col;
    map = malloc(size * sizeof(int *));
    for (row = 0; row < size; row++) {
        map[row] = malloc(size * sizeof(int));
        for (col = 0; col < size; col++) {
            map[row][col] = EMPTY;
        }
    }
}

/* Fonction de diagnostic permettant de visualiser la carte dans le terminal. */
void print_map(int size) {
    int row;
    int col;
    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            if (get_cell(row, col) == EMPTY) printf(". ");
            else if (get_cell(row, col) == WALL) printf("# ");
            else if (get_cell(row, col) == FRUIT) printf("F ");
        }
        printf("\n");
    }
}

/* Libere chaque ligne allouee puis le tableau principal. */
void free_map(int size) {
    int row;
    for (row = 0; row < size; row++) free(map[row]);
    free(map);
}

/* Modifie une case par l'intermediaire de l'interface du module carte. */
void set_cell(int row, int col, int value) {
    map[row][col] = value;
}

/* Retourne le contenu logique d'une case. */
int get_cell(int row, int col) {
    return map[row][col];
}

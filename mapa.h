#include"estado.h"

#ifndef MAPA_H
#define MAPA_H


void mapabase(char **map,char **map2, int row, int col);

int contaobstaculo(char **map,int row, int col, int x, int y, int raio, char c);

void copiamatrizes(char **map, char **map2, int row, int col);

void primeira_passagem(char **map, char **map2, int row, int col, int vezes);

void segunda_passagem(char **map, char **map2, int row, int col, int vezes);

void armadilhas(char **map, char **map2, int row, int col);

void portais(char **map, char **map2, int row, int col);

void baus(char **map, char **map2, int row, int col);

void geraescadas (char **map, char **map2, int row, int col);

void retiraobstaculo(char **map, int r, int c);

void desenhamapa (char **map,int row, int col, int x, int y);

void geramapa (char **mapamain, char **mapaaux, int row, int col);

//void colocaPlayer (char map[][COL], Player *player);

#endif


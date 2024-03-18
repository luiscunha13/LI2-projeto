#include"estado.h"

#ifndef JOGADOR_H
#define JOGADOR_H
//pode ser preciso dar include de outros .h


Player * playerSetUp();

void movement(int input, Player * user, char **map, int row, int col, Monstro monstros[], int size, int *bau, int *coords, int *escadas);

void checkPos (int nextY, int nextX, Player * user, char **map, int row, int col);

void playerMove(int y, int x, Player * user );

void proxportal (char **map, int row, int col, Player *user, int r, int c);

int distance (int xp,int yp,int xm,int ym);

void playerAttack (Player * user, Monstro * monstro);

void fov(int x, int y, int raio, char **map, int row, int col, Estado estado);

void colocaPlayer (Player *player, char **map, int row, int col);

Weapon createNewWeapon();

void hud (Player * player, int col);

void getCode (char ch, char code[]);

void easterEgg (Player * player, char code[]);

void heal (Player * player);
#endif
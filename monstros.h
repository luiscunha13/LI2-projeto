#ifndef MONSTROS_H
#define MONSTROS_H
//#include "estado.h"
//pode ser preciso dar include de outros .h

Monstro * selectMonster (int level, int row, int col, char **map);

void moveMonster (Player * player, Monstro *monstro, char **map);

int alldead (Monstro monstros[], int size);

#endif
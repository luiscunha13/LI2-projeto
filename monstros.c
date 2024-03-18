#include<ncurses.h>
#include<stdio.h>
#include<stdlib.h>
#include"estado.h"
#include"jogador.h"
#include"monstros.h"

Monstro * createMonster (char symbol, int health, int attack, int pathfinding, int row, int col, char **map);
void pathRandom (Monstro *monstro, Player *player, char **map);
void pathfinding (Monstro *monstro, Player *player, char **map);
void setStartingPosition (Monstro * monstro, int row, int col, char **map);

Monstro * selectMonster (int level, int row, int col, char **map)
{
    int monster = 1;
    switch (level)
    {
        case 1:
        case 2:
        case 3:
            monster = (rand() % 2) + 1;
            break;
        case 4:
        case 5:
            monster = (rand() % 2) + 2;
            break;
        case 6:
            monster = 3;
            break;
    }
    switch (monster)
    {
        case 1: /* Estupida */
            return createMonster ('X', 30, 10, 0, row, col, map);
            break;
        case 2: /* Cobarde */
            return createMonster ('Y', 50, 25, 1, row, col, map);
            break;
        case 3: /* Esperta */
            return createMonster ('Z', 75, 50, 1, row, col, map);
            break;
        default:
            return createMonster ('X', 30, 10, 0, row, col, map);
            break;
    }
}

Monstro * createMonster (char symbol, int health, int attack, int pathfinding, int row, int col, char **map)
{
    Monstro * newMonster;
    newMonster = malloc (sizeof(Monstro));

    newMonster -> symbol = symbol;
    newMonster -> hp = health;
    newMonster -> isalive = 1;
    newMonster -> attack = attack;
    newMonster -> pathfinding = pathfinding;

    sprintf (newMonster->string, "%c", symbol);

    setStartingPosition (newMonster, row, col, map);
    return newMonster;
}


/*
    char ** save() {
    int x, y;
    char ** positions;
    positions = malloc (sizeof (char *) * 25);
}
*/

void setStartingPosition (Monstro * monstro, int row, int col, char **map) // (sala ficticia criada)
{
    while (1) {
        int y = rand() % row;
        int x = rand() % col;
        if (map[y][x] == '#' || map[y][x] == 'B' || map[y][x] == '*' || map[y][x] == '%') continue;
        else {monstro -> posiX = x; monstro -> posiY = y; break;}
    }

    mvprintw (monstro -> posiY, monstro -> posiX,"%s", monstro -> string);
} 

void moveMonster (Player * player, Monstro *monstro, char **map)
{
    if (monstro->hp < 1) {
        monstro->isalive = 0;
        int ranum = rand();
        if((ranum % 5) == 0) {
            (*player).healpotion++;
        }
        (*player).killcount++;
    }
    //mvprintw(monstros[x]->posiY, monstros[x]->posiX, ".");
    if (monstro->isalive) {
        if (monstro->pathfinding == 0) {
            pathRandom(monstro, player, map);
        }
        else {
            int pathorrand = rand() % 100;
            if (pathorrand > 20) pathfinding(monstro, player, map);
            else pathRandom(monstro, player, map);
        }
        //mvprintw(monstros[x]->posiY, monstros[x]->posiX, "%s", monstros[x] -> string);
    }
}

// Esta funçao faz com que os monstros so andem pelos '.'

void pathfinding (Monstro *monstro, Player *player, char **map)
{
    if ((abs((monstro->posiX - 1) - player->posiX) < abs(monstro->posiX - player->posiX)) && (map[monstro->posiY][monstro->posiX-1] == '.'))
    {
        if(monstro->posiX-1 != player->posiX || monstro->posiY != player->posiY) monstro->posiX--;
    }
    else if ((abs((monstro->posiX + 1) - player->posiX) < abs(monstro->posiX - player->posiX)) && (map[monstro->posiY][monstro->posiX+1] == '.'))
    {
        if(monstro->posiX+1 != player->posiX || monstro->posiY != player->posiY) monstro->posiX++;
    }
    else if ((abs((monstro->posiY + 1) - player->posiY) < abs(monstro->posiY - player->posiY)) && (map[monstro->posiY+1][monstro->posiX] == '.'))
    {
        if(monstro->posiX != player->posiX || monstro->posiY+1 != player->posiY) monstro->posiY++;
    }
    else if((abs((monstro->posiY - 1) - player->posiY) < abs(monstro->posiY - player->posiY)) && (map[monstro->posiY-1][monstro->posiX] == '.'))
    {
        if(monstro->posiX != player->posiX || monstro->posiY-1 != player->posiY) monstro->posiY--;
    }

    
}

void pathRandom (Monstro *monstro, Player *player, char **map)
{
    int random;
    random = rand() % 4;

    switch (random) {
        case 0: //Para baixo
            if (map[monstro->posiY + 1][monstro->posiX] == '.') {
                if(monstro->posiX != player->posiX || monstro->posiY+1 != player->posiY) monstro->posiY++;
                break;
            }
            else break;
        case 1: //Para cima
            if (map[monstro->posiY - 1][monstro->posiX] == '.') {
                if(monstro->posiX != player->posiX || monstro->posiY-1 != player->posiY) monstro->posiY--;
                break;
            }
            else break;
        case 2: //Para a esquerda
            if (map[monstro->posiY][monstro->posiX - 1] == '.') {
                if(monstro->posiX-1 != player->posiX || monstro->posiY != player->posiY) monstro->posiX--;
                break;
            }
            else break;
        case 3: //Para a direita
            if (map[monstro->posiY][monstro->posiX + 1] == '.') {
                if(monstro->posiX+1 != player->posiX || monstro->posiY != player->posiY) monstro->posiX++;
                break;
            }
            else break;
        default:
            break;
    }
}

int monstersalive (Monstro monstros[], int count){
    int acc=0;
    for(int i=0;i<count;i++){
        if (monstros[i].hp > 0) acc++;
    }

    return acc;
}


int alldead (Monstro monstros[], int size){
    int acc=0;
    
    for(int i=0;i<size;i++){
        if(monstros[i].hp<=0) acc++;
    }

    if(acc==size) return 1;
    else return 0;
}
/*
Planing the Monsters:

Estupida: Runs after the monster and goes for melee damage;
Symbol: X; Health: 4; Attack: 3; Speed: 2; Pathfinding: (2) Straight to the player and goes for melee damage ONLY;

Cobarde: Escapes if it is alone, attacks in groups, screams to alert the other monsters;
Symbol: Y; Health: 3; Attack: 2; Speed: 4; Pathfinding: (1) Straight to the player when they see it if not alone, goes for melee damage and range damage;

Esperta: Attacks in groups, ambush the player, alerts the other monsters, uses bombs or rocks to attack;
*/
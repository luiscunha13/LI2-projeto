#ifndef ESTADO_H
#define ESTADO_H

//pode ser preciso dar include de outros .h

#define OBS 45 // percentagem de obstaculos no mapa
#define TRAP 0.01
#define PORTAL 0.002 // numero de portais no mapa
#define BAU 0.002

typedef struct Weapon {
    int dmg;
    int rarity; // 1- common "1-5 dano", 2- uncommon "5-10" , 3- rare "12-25", 4- epic "30-50", 5- legendary "60-100" 
    char weaponType; // este parametro ´e para diferenciar entre varios tipos de armas e os seus efeitos por exemplo uma espada tem range menor, uma lança range maior e um arco ainda maior mas n consegue atacar muito perto
} Weapon;            // 'E' = espada, 'L' = lança, 'A' = arco
                          // alteram chances de drop na loja pra n ser muito OP

typedef struct Player {
    int posiX;
    int posiY;
    int hp;
    int exp;
    int healpotion;
    int killcount;
    Weapon arma;
} Player;

typedef struct Monstro
{
    int posiX;
    int posiY;
    int isalive;
    char string[2];
    char symbol;
    int hp;
    int attack;
    int pathfinding;
} Monstro;


/*
char mapamain [ROW][COL];
char mapaaux [ROW][COL];
*/



typedef struct state {
    Player jogador;
    Monstro monstros[10]; //tobechanged -- E preciso uma lista de montros caso haja varios, o numero 20 e arbitrario e pode ser alterado
    int Score;
} Estado;





#endif
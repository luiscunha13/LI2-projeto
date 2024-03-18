#include<ncurses.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include<SFML/Audio.h>
#include"estado.h"
#include"jogador.h"
#include"mapa.h"

Player * playerSetUp();
void movement(int input, Player * user, char **map, int row, int col, Monstro monstros[], int size, int *bau, int *coords, int *escadas);
void checkPos (int nextY, int nextX, Player * user, char **map, int row, int col);
void playerMove(int y, int x, Player * user );
int distance (int xp,int yp,int xm,int ym);
void playerAttack (Player * user, Monstro * monstro);
void fov(int x, int y, int raio, char **map, int row, int col, Estado estado);
Weapon createNewWeapon();

Player * playerSetUp() {
    Player * newPlayer;
    newPlayer = malloc(sizeof(Player));

    newPlayer-> posiX = 5;
    newPlayer-> posiY = 5;
    newPlayer-> hp = 100;
    newPlayer-> arma.dmg = 2;
    newPlayer-> arma.rarity = 1; //common
    newPlayer-> arma.weaponType = 'E';
    newPlayer-> exp = 0;
    newPlayer->healpotion = 3;
    newPlayer->killcount = 0;
    
    return newPlayer;
}

void movement(int input, Player * user, char **map, int row, int col, Monstro monstros[], int size, int *bau, int *coords, int *escadas) {
    int newX=0;
    int newY=0;
    int monster=0;

    switch (input) {
        case KEY_UP:
        case 'w' : //cima
        case 'W' : 
            newX = user->posiX;
            newY = user->posiY -1;
            break;
        case KEY_DOWN:
        case 's' : //baixo
        case 'S' :
            newX = user->posiX;
            newY = user->posiY +1;
            break;
        case KEY_RIGHT:
        case 'd' : //direita
        case 'D' :
            newX = user->posiX +1;
            newY = user->posiY;
            break;
        case KEY_LEFT:
        case 'a' : //esquerda
        case 'A' :
            newX = user->posiX -1;
            newY = user->posiY;
            break;
        case 'e':
        case 'E':
                for(int i=user->posiY-1;i<=user->posiY+1;i++){
                    for(int j=user->posiX-1;j<=user->posiX+1;j++){
                        if(map[i][j] == 'B' || map[i][j] == 'b') {
                            *bau = 1;
                            coords[0] = i;
                            coords[1] = j;
                        }
                    }
                }

            newX = user->posiX;
            newY = user->posiY;
            break;
        case ' ' :
            if(map[user->posiY][user->posiX] == '%') *escadas=1;
            break;
        default:
           break;
    }

    for(int i=0;i<size;i++){
        if(newX == monstros[i].posiX && newY == monstros[i].posiY && monstros[i].isalive){
            monster = 1;
            break;
        }
    }

    if(monster == 0) checkPos(newY, newX, user, map, row, col);

}

int RandomNumInRange (int menor, int maior) {
    int num = (rand() % (maior - menor + 1)) + menor;
    return num;
}

void checkPos (int newY, int newX, Player * user, char **map, int row, int col) {
    //int space;
    switch(map[newY][newX]) {
        case '.' :  //dps aqui da pra meter mais cases se o boneco poder andar por certas coisas
            playerMove(newY, newX, user);
            break;
        case '*':
            playerMove(newY, newX, user);
            srand(time(0));
            int randN = RandomNumInRange(20, 40);
            user->hp -= randN; // quando pisa na "trap" leva dano no valor entre 20 a 40
            retiraobstaculo(map, newY, newX);
            break;
        case 'o':
            proxportal(map, row, col, user, newY, newX);
            break;
        default:
            move(user->posiY, user->posiX);
            break;
    }
}

void playerMove (int y, int x, Player * user) {
    
    mvprintw(user->posiY, user->posiX, ".");
                                                //mexer o player
    user->posiY = y;
    user->posiX = x;

    mvprintw(user->posiY, user->posiX, "@");
    move(user->posiY,user->posiX);
}

void proxportal (char **map, int row, int col, Player *user, int r, int c) {
    time_t t;
    srand(time(&t));

    mvprintw(user->posiY, user->posiX, ".");

    int x = 0, y = 0, swtch = 1;
    while (swtch) {
        x = RandomNumInRange(0,col-1);
        y = RandomNumInRange(0,row-1);
        if (map[y][x] == 'o' && y != r && x != c) {
            user->posiX = x;
            user->posiY = y;
            swtch = 0;
        }
    }

    mvprintw(user->posiY, user->posiX, "@");
    move(user->posiY,user->posiX);

}

int distance (int xp,int yp,int xm,int ym) {
    int d = sqrt(((xm-xp)^2) + ((ym-yp)^2));
    return d;
}

void playerAttack (Player * user, Monstro * monstro) {
    int xP = user->posiX;
    int yP = user->posiY;
    int xM = monstro->posiX;
    int yM = monstro->posiY;
    switch(user->arma.weaponType) {
        case 'E':
            if(distance(xP, yP, xM, yM) <= 1) {
                monstro->hp -= user->arma.dmg;
            };
            break;
        case 'L':                                
            if(distance(xP, yP, xM, yM) <= 1.5) {
                monstro->hp -= user->arma.dmg;
            };
            break;
        case 'A':
            if(distance(xP, yP, xM, yM) <= 5 && distance(xP, yP, xM, yM) > 1) {
                monstro->hp -= user->arma.dmg;
            };
            break;
        default:
            break;

    }
}

void viewarround(int x, int y, char **map) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (map[y+i][x+j] == '#') mvaddch (y+i, x+j, '#');
        }
    }
}

void fov(int x, int y, int raio, char **map, int row, int col, Estado estado) {
    int largura = col;
    int altura = row;
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);

    mvaddch(y, x, '@');
    
    viewarround(x, y, map);
    for(float i = 0; i < 360; i += 0.1) {
        float angle = i * (3.1416 / 180); //tranforma graus em radianos
        float nx = sin(angle);
        float ny = cos(angle);
        float luz_x = x;
        float luz_y = y;
        //int parede = 0;
        for(int j = 0; j < raio; j++) {
            luz_x += nx;
            luz_y += ny;
            int lightx = round(luz_x);
            int lighty = round(luz_y);
            if(lightx < 0 || lighty < 0 || lightx >= largura || lighty >=  altura) {
                break;
            }
            if(map[lighty][lightx] == '#') {
                //parede = 1;
                break;
            }
            float simbolo_x = x;
            float simbolo_y = y;
            for (int p = 0; p < raio; p++) {
                simbolo_x += nx; 
                simbolo_y += ny;
                int auxX = round(simbolo_x);
                int auxY = round(simbolo_y);
                if (auxX < 0 || auxY < 0 || auxX >= largura || auxY >= altura) {
                    break;
                }
                mvaddch(auxY, auxX, map[auxY][auxX]);
                for (int o = 0; o < 10; o++) {
                    if (estado.monstros[o].posiX == auxX && estado.monstros[o].posiY == auxY && estado.monstros[o].isalive) {
                        mvaddch (estado.monstros[o].posiY, estado.monstros[o].posiX, estado.monstros[o].symbol);
                    }
                }
                if(map[auxY][auxX] == '#') {
                    break;
                }
            }
        }

    }
}    

void colocaPlayer (Player *player, char **map, int row, int col) {
    time_t t;
    srand(time(&t));
    int x = 0, y = 0, swtch = 1;
    while (swtch) {
        x = rand () % col;
        y = rand () % row;
        if (map[y][x] == '.') {
            player->posiX = x;
            player->posiY = y;
            swtch = 0;
        }
    }
}

/*
void createNewWeapon (Player * player) {
    int rare = (rand() % 100) + 1;
    int weaponTyp = (rand() % 3) + 1; // 1 = "E" (espada); 2 = "L" (lança); 3 = "A" (arco);
    int damage = 0;
    char wType;
    if(weaponTyp == 1) { wType = 'E'; }
    if(weaponTyp == 2) { wType = 'L'; }
    if(weaponTyp == 3) { wType = 'A'; }
    if (rare <= 50) { damage = RandomNumInRange(1, 5); }
    if (rare > 50 && rare <= 80) { damage = RandomNumInRange(6, 12); }
    if (rare > 80 && rare <= 95) { damage = RandomNumInRange(13, 18); }
    if (rare > 95 && rare <= 99) { damage = RandomNumInRange(19, 25); }
    if (rare == 100) { damage = RandomNumInRange(30, 50); }
    (*player).arma.rarity = rare;
    (*player).arma.weaponType = wType;
    (*player).arma.dmg = damage;
}
*/

Weapon createNewWeapon () {
    int rare = (rand() % 100) + 1;
    int weaponTyp = (rand() % 3) + 1; // 1 = "E" (espada); 2 = "L" (lança); 3 = "A" (arco);
    int damage = 0;
    char wType;
    if(weaponTyp == 1) { wType = 'E'; }
    if(weaponTyp == 2) { wType = 'L'; }
    if(weaponTyp == 3) { wType = 'A'; }
    if (rare <= 50) { damage = RandomNumInRange(1, 5); }
    if (rare > 50 && rare <= 80) { damage = RandomNumInRange(6, 12); }
    if (rare > 80 && rare <= 95) { damage = RandomNumInRange(13, 18); }
    if (rare > 95 && rare <= 99) { damage = RandomNumInRange(19, 25); }
    if (rare == 100) { damage = RandomNumInRange(30, 50); }
    Weapon arma;
    arma.rarity = rare;
    arma.weaponType = wType;
    arma.dmg = damage;
    return arma;
}

void hud (Player * player, int col) {
    init_pair (1, COLOR_RED, COLOR_BLACK);
    init_pair (2, COLOR_GREEN, COLOR_BLACK);
    init_pair (3, COLOR_BLUE, COLOR_BLACK);
    init_pair (4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair (5, COLOR_YELLOW, COLOR_BLACK);
    init_pair (6, COLOR_CYAN, COLOR_BLACK);
    char hp[4]; hp[3] = 0;
    char dmg[6];
    char heal[4];
    char kills[5];
    sprintf(hp, "%d", (*player).hp);
    sprintf(dmg, "%d", (*player).arma.dmg);
    sprintf(heal, "%d", (*player).healpotion);
    sprintf(kills, "%d", (*player).killcount);

    // player related information
    attron(COLOR_PAIR(1));
    mvaddstr (1, col + 3, "HP: ");
    attroff(COLOR_PAIR(1));
    mvaddstr (1, col + 7, hp);

    mvaddstr (3, col + 3, "WEAPON: ");

    mvaddstr (4, col + 3, "RARITY: ");

    switch((*player).arma.weaponType) {
        case 'E':
        mvaddstr(3, col + 11, "Espada");
        break;
        case 'L':
        mvaddstr(3, col + 11, "Lança");
        break;
        case 'A':
        mvaddstr(3, col + 11, "Arco");
        break;

        default:
            break;
    }

    int rare = (*player).arma.rarity;
    if(rare <= 50) {
        mvaddstr (4, col + 11, "COMMON");
    }
    if(rare > 50 && rare <= 80) {
        attron(COLOR_PAIR(2));
        mvaddstr (4, col+ 11, "UNCOMMON");
        attroff(COLOR_PAIR(2));
    }
    if(rare > 80 && rare <= 95) {
        attron(COLOR_PAIR(3));
        mvaddstr (4, col + 11, "RARE");
        attroff(COLOR_PAIR(3));
    }
    if(rare > 95 && rare <= 99) {
        attron(COLOR_PAIR(4));
        mvaddstr (4, col + 11, "EPIC");
        attroff(COLOR_PAIR(4));
    }
    if(rare == 100) {
        attron(COLOR_PAIR(5));
        mvaddstr (4, col + 11, "LEGENDARY");
        attroff(COLOR_PAIR(5));
    }

    if(rare == 777) {
        attron(COLOR_PAIR(1));
        mvaddstr (4, col + 11, "MYTHIC");
        attroff(COLOR_PAIR(1));
    }

    mvaddstr (5, col + 3, "DAMAGE: ");
    mvaddstr (5, col + 11, dmg);

    mvaddstr (7, col + 3, "Potions: ");
    mvaddstr (7, col + 11, heal);

    mvaddstr (9, col + 3, "Kills: ");
    mvaddstr (9, col + 9, kills);

    mvaddstr(12, col+3, "INSTRUCTIONS:");
    mvaddstr(14, col+3, "UP - W / UP ARROW");
    mvaddstr(15, col+3, "DOWN - S / DOWN ARROW");
    mvaddstr(16, col+3, "RIGHT - D / RIGHT ARROW");
    mvaddstr(17, col+3, "LEFT - A / LEFT ARROW");

    mvaddstr(19, col+3, "ATACK - F");
    mvaddstr(20, col+3, "HEAL - H");
    mvaddstr(21, col+3, "OPEN CHEST - E");

    mvaddstr(27, col+3, "QUIT - Q");
}

void getCode (char ch, char code[]) {
    for (int i = 0; i < 9; i++) {
        code[i] = code[i + 1];
    }
    code[9] = ch;
    code[10] = '\0';
}

void easterEgg (Player * player, char code[]) {
    char password[10] = "WWSSADADBA";
    if(strcmp(code, password) == 0) {
        (*player).arma.rarity = 777; //threefold perfection of the trinity
        (*player).arma.dmg = 99999;
        (*player).arma.weaponType = 'E';
    }
} 


void heal (Player * player) {
    if((*player).healpotion > 0) {
        if((*player).hp >= 75) {
            (*player).hp = 100;
        }
        else {
            (*player).hp += 25;
        }
        (*player).healpotion--;
    }
}


/*
void rainbow (int col) {
    int nM = (rand() % 6) + 1;
    int nY = (rand() % 6) + 1; 
    int nT = (rand() % 6) + 1; 
    int nH = (rand() % 6) + 1; 
    int nI = (rand() % 6) + 1; 
    int nC = (rand() % 6) + 1; 
    attron(COLOR_PAIR(nM));
    mvaddstr (4, col + 11, "M");
    attroff(COLOR_PAIR(nM));
    attron(COLOR_PAIR(nY));
    mvaddstr (4, col + 12, "Y");
    attroff(COLOR_PAIR(nY));
    attron(COLOR_PAIR(nT));             // ainda n funciona
    mvaddstr (4, col + 13, "T");
    attroff(COLOR_PAIR(nT));
    attron(COLOR_PAIR(nH));
    mvaddstr (4, col + 14, "H");
    attroff(COLOR_PAIR(nH));
    attron(COLOR_PAIR(nI));
    mvaddstr (4, col + 15, "I");
    attroff(COLOR_PAIR(nI));
    attron(COLOR_PAIR(nC));
    mvaddstr (4, col + 16, "C");
    attroff(COLOR_PAIR(nC));
}
*/


/*
void abrebau (Player * player){
    int x = RandomNumInRange(1,100);



}
*/

//dar mais vida
//melhorar a arma
//









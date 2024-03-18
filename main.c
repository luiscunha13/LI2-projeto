#include<ncurses.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<errno.h>
#include<math.h>
#include<SFML/Audio.h>
#include"estado.h"
#include"jogador.h"
#include"mapa.h"
#include"monstros.h"

int msleep(long msec) {     //input in miliseconds, sleeps for given ammount w. no interruptions
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}


void update (Estado estado,char **map, int row, int col, char ch, int *range, int *draw, int *breaks) {
    if (ch == 'i' || ch == 'I') {
            if (*range == 10) *range = 1000;
            else (*range) = 10;
        }
    if (ch == 'm' || ch == 'M') {
        if (*draw) *draw = 0;
        else *draw = 1;
    }
    clear();
    if (!*draw) fov(estado.jogador.posiX, estado.jogador.posiY, *range, map, row, col, estado);
    else desenhamapa(map,row, col, estado.jogador.posiX, estado.jogador.posiY);
    hud (&estado.jogador,col);
    /*
    for (int l = 0; l < 10; l++) {
        if (estado.monstros[l].isalive) mvaddch(estado.monstros[l].posiY, estado.monstros[l].posiX, estado.monstros[l].symbol);
    }
    */
    
    msleep (33);
    if (estado.jogador.hp < 1) *breaks = 1;
}


int main() {
    Estado estado;
    int col = 1, row = 1;
    WINDOW *wnd = initscr(); // ativa ncurses
    getmaxyx(wnd, row, col);
    if (row > 10000) row = 1;
    int row2 = row + 10;
    col -=26;
    char **mapamain = malloc(sizeof(int *) * row2);
    char **mapaaux = malloc(sizeof(int *) * row2);
    //char mapamain[row][col];
    //char mapaaux[row][col];
    //char *mapabaus[row];

    for(int i=0;i<row;i++){
        //for (int j=0;j<col;j++) {
        //    mapamain[i][j] = malloc(sizeof(char));
        //    mapaaux[i][j] = malloc(sizeof(char));
        //}
        mapamain[i]= (char *)malloc(sizeof(int) * col);
        mapaaux[i]= (char *)malloc(sizeof(int) * col);
        //mapabaus[i] = malloc(sizeof(char) * col);
    }

    start_color();  //ativa cores
	noecho(); // não mostra o input no terminal
    raw();
    curs_set(0); // cursor a piscar é desativado
    nodelay(stdscr, TRUE); //nao espera pelo input do jogador
    keypad(stdscr, TRUE); //ativa keypad

    int level=1;
    int range = 10;
    int draw = 0;
    int breaks = 0;
    int cycle = 0;
    int exit = 0;
    int stairs = 0;
    int nextlevel = 0;
    int nummonsters = 0;
    int ch;
    char code[10] = "aaaaaaaaaa";

    while(1){
        
        mvaddstr (row/2-4, col/2+5, "Roguelite");
        
        mvaddstr (row/2, col/2, "Press space to start");
        int exit = 0;
        while(1){
            char c = getch();
            if(c == ' '){
                exit=1;
                clear();
                break;
            }
        }
        if(exit == 1) break;
    }

    while(1){
        nummonsters = level + 5;
        estado.jogador = *(playerSetUp (mapamain));
        geramapa (mapamain, mapaaux, row, col);
        colocaPlayer(&(estado.jogador), mapamain, row, col);
        for (int monstersetup = 0; monstersetup < nummonsters; monstersetup++) {
            estado.monstros[monstersetup] = *(selectMonster(1, row, col, mapamain));
        }
        nodelay(stdscr, TRUE);
        //update(estado, mapamain, row, col, ch, &range, &draw, &breaks);

        while (1) {
            if ((ch = getch()) == ERR) ch = '-';
            if (ch == 'q' || ch == 'Q'){
                exit = 1;
                break;
            }
        
            if (ch == 'W' || ch == 'S' || ch == 'A' || ch == 'D' || ch == 'B') {
                getCode(ch, code);
                easterEgg(&estado.jogador, code);   //cheats do joguinho com o konami code               
            }

            if (ch == 'h' || ch == 'H') {
                heal(&estado.jogador);
            }

            if(alldead(estado.monstros,nummonsters) == 1 && stairs == 0){
                stairs=1;
                geraescadas(mapamain,mapaaux,row,col);

            }

            int escadas = 0;
            int bau = 0;
            int coords[2];
            movement (ch, &estado.jogador, mapamain, row, col, estado.monstros, 10, &bau, coords, &escadas);
            int storedist = 1000, storeindex=0;

            if (bau) {          //caso o jogador esteja a abrir um bau
                nodelay(stdscr, FALSE);

                Weapon arma = createNewWeapon();

                for (int l = 6; l >= 0; l--) {
                    mvaddstr(row - l, 0, "                                                                                                                                                         ");
                }
                mvaddstr(row-5, col/2+9, "Tipo: ");
                switch (arma.weaponType)
                {
                case 'E':
                    mvaddstr(row-5, col/2+15, "Espada");
                    break;
                case 'L':
                    mvaddstr(row-5, col/2+15, "Lança");
                    break;
                case 'A':
                    mvaddstr(row-5, col/2+15, "Arco");
                    break;
                default:
                    break;
                }

                mvaddstr(row-4, col/2+7, "Raridade: ");

                int rare = arma.rarity;
                if(rare <= 50) {
                    mvaddstr (row - 4, col/2 + 18, "COMMON");
                }
                if(rare > 50 && rare <= 80) {
                    attron(COLOR_PAIR(2));
                    mvaddstr (row - 4, col/2+ 18, "UNCOMMON");
                    attroff(COLOR_PAIR(2));
                }
                if(rare > 80 && rare <= 95) {
                    attron(COLOR_PAIR(3));
                    mvaddstr (row - 4, col/2 + 18, "RARE");
                    attroff(COLOR_PAIR(3));
                }
                if(rare > 95 && rare <= 99) {
                    attron(COLOR_PAIR(4));
                    mvaddstr (row - 4, col/2 + 18, "EPIC");
                    attroff(COLOR_PAIR(4));
                }
                if(rare == 100) {
                    attron(COLOR_PAIR(5));
                    mvaddstr (row - 4, col/2 + 18, "LEGENDARY");
                    attroff(COLOR_PAIR(5));
                }

                mvaddstr (row - 2, col/2, "Queres os conteudos do bau? (y/Y)");
                char chtemp = '.';
                chtemp = getch();



                if (chtemp == 'y' || chtemp == 'Y') {
                    estado.jogador.arma = arma;
                    mapamain[coords[0]][coords[1]] = '.';
                }
                else {
                    mapamain[coords[0]][coords[1]] = '.';
                }

                nodelay(stdscr, TRUE);
            }

            if(escadas){
                nextlevel = 1;
                break;
            }

            for (int monstemovement = 0; monstemovement < 10; monstemovement++) {
                int dist = round(sqrt((estado.monstros[monstemovement].posiY - estado.jogador.posiY)*(estado.monstros[monstemovement].posiY - estado.jogador.posiY) + (estado.monstros[monstemovement].posiX - estado.jogador.posiX)*(estado.monstros[monstemovement].posiX - estado.jogador.posiX)));
                if (cycle == 30 && estado.monstros[monstemovement].isalive) {
                    moveMonster (&estado.jogador, &estado.monstros[monstemovement], mapamain); 

                    int willattack = rand() % 2;
                    if (dist < 3 && willattack && estado.monstros[monstemovement].isalive) {
                        estado.jogador.hp -= estado.monstros[monstemovement].attack;
                    }
                }

                if (dist < storedist && estado.monstros[monstemovement].isalive) {
                    storedist = dist;
                    storeindex = monstemovement;
                }
            }
            if (cycle==30) cycle=0;

            if (ch == 'f' || ch == 'F') {
                playerAttack(&estado.jogador, &estado.monstros[storeindex]);     
            }

            update(estado, mapamain, row, col, ch, &range, &draw, &breaks);

            if (breaks) break;
            cycle++;
        }

        if(exit == 1){
            exit = 0;
            break;
        }
        breaks =0;
        cycle=0;

        if(nextlevel == 1){
            level++;
            
        }
        else{
          clear();
            init_pair (9, COLOR_RED, COLOR_BLACK);
            nodelay(stdscr, FALSE);
            attron(COLOR_PAIR (1));
            mvaddstr (row/2-4, col/2+5, "YOU LOST");
            attroff(COLOR_PAIR (1));
            mvaddstr (row/2, col/2-5, "Press r to restart or q to quit");
        
            while(1){
                ch = getch();
                if(ch == 'r' || ch == 'R') break;
                if(ch == 'q' || ch == 'Q'){
                    exit = 1;
                    break;
                }
            }
            if(exit == 1) break;  
        }
        
        
    }
    
    endwin(); // fechar o ncurses
    return 0;
}
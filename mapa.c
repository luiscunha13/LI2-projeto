#include<ncurses.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"mapa.h"
#include"jogador.h"
#include"estado.h"

void mapabase(char **map,char **map2, int row, int col){
    time_t t;
    srand(time(&t));

    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            if(i == 0 || i == row-1 || j == 0 || j == col-1) map[i][j]='#';
            else{
                if(rand()%100 < OBS) map2[i][j] = map[i][j] = '#';
 	            else map2[i][j] = map[i][j] = '.';   //alterei para ponto para ser mais visivel (antes era espaco). pode se mudar outra vez no futuro
            }
            
        }
    }
}

int contaobstaculo (char **map,int row, int col, int x, int y, int raio, char c){
    int contagem_parede=0, breaks = 0;

    for (int i = -raio; i <= raio; i++) { // quadrado 3x3
        if (breaks) break;
        for (int j = -raio; j <= raio; j++) {
            if (x + i < 0 || x + i >= row || y + j < 0 || y + j >= col) {breaks = 1; break;} // Fora do mapa
            if (map[x + i][y + j] == c) contagem_parede++;
        }
    }

    return contagem_parede;
}

void copiamatrizes (char **map, char **map2, int row, int col){
    for (int i = 1; i < row-1; i++) {
            for (int j = 1; j < col-1; j++) map[i][j] = map2[i][j];
    }
}

void primeira_passagem(char **map, char **map2, int row, int col, int vezes){ // tem que receber o mapa criado na função mapabase, e o número de vezes que a função é chamada
   for(int a=0;a<vezes;a++){
        for (int i = 1; i < row-1; i++) {
            for (int j = 1; j < col-1; j++) {

                if (contaobstaculo(map,row,col,i,j,1,'#') >= 5 || contaobstaculo(map,row, col,i,j,2,'#') <= 2) map2[i][j] = '#';
                else map2[i][j] = '.';

            }
        }

        copiamatrizes(map,map2,row,col);
    }    
}

void segunda_passagem(char **map, char **map2, int row, int col, int vezes){
    for(int a=0;a<vezes;a++){
        for (int i = 1; i < row-1; i++) {
            for (int j = 1; j < col-1; j++) {

                if (contaobstaculo(map,row,col,i,j,1,'#') >= 5) map2[i][j] = '#';
                else map2[i][j] = '.';
            }
        }

        copiamatrizes(map,map2,row,col);
    }    
}

void armadilhas(char **map, char **map2, int row, int col){

    time_t t;
    srand(time(&t));

    
    int r = 0, c = 0, acc = 0;
    while (acc<round(TRAP*(row*col))) { // pode-se mudar depois
        c = rand () % col;
        r = rand () % row;
        if (map[r][c] == '.') {
            if(rand()%100 < 1){
                if(contaobstaculo(map,row, col, r, c, 3, '*') == 0){
                   map2[r][c] = map[r][c] = '*';
                   acc++;  
                }
               
            }
            
        }
    }
}

void portais(char **map, char **map2, int row, int col){
    
    time_t t;
    srand(time(&t));

    int r = 0, c = 0, acc = 0;
    while (acc<round(PORTAL*(row*col))) { // pode-se mudar depois
        c = rand () % col;
        r = rand () % row;
        if (map[r][c] == '.') {
            if(rand()%100 < 1){
                if(contaobstaculo(map,row, col, r, c, 5, 'o') == 0){
                   map2[r][c] = map[r][c] = 'o';
                   acc++;  
                }
               
            }
            
        }
    }
}

void baus(char **map, char **map2, int row, int col){

    time_t t;
    srand(time(&t));

    int r = 0, c = 0, acc = 0;
    while (acc<round(BAU*(row*col))) { // pode-se mudar depois
        c = rand () % col;
        r = rand () % row;
        if (map[r][c] == '.' && (map[r+1][c] == '#' || map[r-1][c] == '#' || map[r][c+1] == '#' || map[r][c-1] == '#')){
            if(rand()%100 < 1){
                if(contaobstaculo(map,row, col, r, c, 5, 'B') == 0){
                   map2[r][c] = map[r][c] = 'B';
                   acc++;  
                }
            }
            
        }
    }
}

void geraescadas (char **map, char **map2, int row, int col){
    time_t t;
    srand(time(&t));

    int r = 0, c = 0, acc = 0;
    while (acc<1) { // pode-se mudar depois
        c = rand () % col;
        r = rand () % row;
        if (map[r][c] == '.') {
            if(rand()%100 < 1){
                if(contaobstaculo(map,row, col, r, c, 1, '%') == 0){
                   map2[r][c] = map[r][c] = '+';
                   acc++;  
                }
               
            }
            
        }
    }
     
}


void retiraobstaculo(char **map, int r, int c){ // usado em jogador.c
    map[r][c] = '.';
}
 

void desenhamapa (char **map,int row, int col, int x, int y){
    for (int i = 0; i < row; i++) {
        printf("\n");
        for (int j = 0; j < col; j++) mvaddch(i, j, map[i][j]);
    }
    mvaddch(y, x, '@');
}



void geramapa (char **mapamain, char **mapaaux, int row, int col) {
    mapabase (mapamain, mapaaux, row, col);
    primeira_passagem (mapamain, mapaaux, row, col, 4);
    segunda_passagem (mapamain, mapaaux, row, col, 4);
    armadilhas(mapamain,mapaaux, row, col);
    portais(mapamain, mapaaux, row, col);
    baus(mapamain, mapaaux, row, col);
}

/*
void colocaPlayer (char map[][COL], Player *player) {
    time_t t;
    srand(time(&t));
    int x = 0, y = 0, swtch = 1;
    while (swtch) {
        x = rand () % COL;
        y = rand () % ROW;
        if (map[y][x] == '.') {
            player->posiX = x;
            player->posiY = y;
            swtch = 0;
        }
    }
}


void mapsetup (char map[][COL], char mapaux[][COL]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            map[i][j] = '.';
            mapaux[i][j] = '.';
        }
    }
}

int eobstaculo(char map[][COL], int x, int y){ // usado em jogador.c

    if (map[x][y] == '#' || x<0 || y<0 || x>=ROW || y>=COL) return 1;
    else return 0;

}
*/
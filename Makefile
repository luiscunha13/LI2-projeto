CC=gcc
CFLAGS=-Wall -Wextra -pedantic -O2
LIBS=-lcurses -lm -lcsfml-audio

all: main

main: main.o estado.o jogador.o mapa.o monstros.o
	$(CC) $(CFLAGS) -o main main.o estado.o jogador.o mapa.o monstros.o $(LIBS)

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c

estado.o: estado.c estado.h
	$(CC) $(CFLAGS) -c estado.c

jogador.o: jogador.c jogador.h 
	$(CC) $(CFLAGS) -c jogador.c

mapa.o: mapa.c mapa.h jogador.h 
	$(CC) $(CFLAGS) -c mapa.c

monstros.o: monstros.c monstros.h 
	$(CC) $(CFLAGS) -c monstros.c

clean:
	rm -f jogo *.o


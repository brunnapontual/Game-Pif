#ifndef MYLIB_H
#define MYLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SAIR 27
#define ENTRAR 10
#define CIMA 119
#define BAIXO 115
#define ESQUERDA 97
#define DIREITA 100

typedef struct SnakeP {
    int posX, posY;
    struct SnakeP *prox;
} SnakeP;

void movimentoCobra(SnakeP **cabeca, int x, int y);

#endif 

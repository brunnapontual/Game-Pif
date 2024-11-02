#include "mylib.h"
#include "screen.h"
#include "keyboard.h"

#define SAIR 27
#define ENTRAR 10
#define CIMA 119
#define BAIXO 115
#define ESQUERDA 97
#define DIREITA 100

void desenharCobra(SnakeP *cabeca) {
    while (cabeca != NULL) {
        screenSetColor(RED, BLUE);
        screenGotoxy(cabeca->posX, cabeca->posY);
        printf("#");
        cabeca = cabeca->prox;
    }
}
void movimentoCobra(SnakeP **cabeca, int x, int y) {
    SnakeP *novaParte = malloc(sizeof(SnakeP));
    novaParte->posX = x;
    novaParte->posY = y;
    novaParte->prox = *cabeca;
    *cabeca = novaParte;

    SnakeP *temp = *cabeca;
    while (temp->prox->prox != NULL) temp = temp->prox;
    free(temp->prox);
    temp->prox = NULL;
}

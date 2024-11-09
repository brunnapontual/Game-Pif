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

typedef struct {
    char nome[20];
    int pontos;
} Jogador;

typedef struct SnakeP {
    int posX, posY;
    struct SnakeP *prox;
} SnakeP;

typedef enum {
    FRUTA_COLORIDA,
    FRUTA_PRETO
} TipoFruta;

typedef struct {
    int posX, posY;
    TipoFruta tipo;
} Fruta;

typedef struct Placar {
    Jogador jogador;
    struct Placar *prox;
} Placar;

void desenharCobra(SnakeP *cabeca);
void movimentoCobra(SnakeP **cabeca, int x, int y);
void aumentarTamanho(SnakeP **cabeca, int x, int y);
void diminuirTamanho(SnakeP **cabeca);
void desenharFruta(int x, int y, TipoFruta tipo);
void novaFruta(int *x, int *y, TipoFruta *tipo);
void verificarFrutaComida(SnakeP **cabeca, Fruta *fruta);
int verificarColisao(SnakeP *cabeca, int x, int y);
void GameOver();
void atualizarPlacar(int pontos);
void imprimirRanking(Placar *cabeca);
void freePlacar(Placar **cabeca);
void atualizarPlacar(int pontos);
void salvarPlacar(Placar *cabeca, FILE *arquivo);
void adicionarPlacar(Placar **cabeca, Jogador jogador);
void freeCobra(SnakeP **cabeca);
void limparCobra(SnakeP *cabeca);

#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "keyboard.h"
#include "screen.h"
#include "timer.h"
#define COLUNAS 40
#define LINHAS 19
#define MAX_SCORES 5
typedef struct {
    int x, y;
} Ponto;
typedef struct {
    Ponto *partes;
    int comprimento;
} Cobra;
typedef struct {
    Ponto local;
} Alimento;
typedef struct {
    int score;
    char nome[50];
} Placar;
Cobra cobra;
int jogoAtivo = 1;
char nomeJogador[50];
int main() {
    return 0;
}

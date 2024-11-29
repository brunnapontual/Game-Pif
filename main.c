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
Alimento comida;
int direcaoX = 1;
int direcaoY = 0;
int jogoAtivo = 1;
char nomeJogador[50];
Placar placares[MAX_SCORES];

void iniciarJogo() {
    printf("Digite seu nome: ");
    scanf("%s", nomeJogador);

    screenHideCursor();
    keyboardInit();
    timerInit(55);

    cobra.comprimento = 1;
    cobra.partes = (Ponto *)malloc(cobra.comprimento * sizeof(Ponto));
    if (cobra.partes == NULL) {
        printf("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    cobra.partes[0].x = COLUNAS / 2;
    cobra.partes[0].y = LINHAS / 2;
}

void desenharLimites() {
    screenClear();
    printf("┌");
    for (int i = 0; i < COLUNAS; i++)
        printf("─");
    printf("┐\n");
    for (int j = 0; j < LINHAS; j++) {
        printf("│");
        for (int i = 0; i < COLUNAS; i++)
            printf(" ");
        printf("│\n");
    }
    printf("└");
    for (int i = 0; i < COLUNAS; i++)
        printf("─");
    printf("┘");
}

void desenharJogo() {
    for (int i = 0; i < cobra.comprimento; i++) {
        screenGotoxy(cobra.partes[i].x, cobra.partes[i].y);
        printf("0");
    }
    screenGotoxy(comida.local.x, comida.local.y);
    printf("*");

    screenUpdate();
}

void criarComida() {
    int valido;
    do {
        valido = 1;
        comida.local.x = rand() % COLUNAS;
        comida.local.y = rand() % LINHAS;

        for (int i = 0; i < cobra.comprimento; i++) {
            if (cobra.partes[i].x == comida.local.x && cobra.partes[i].y == comida.local.y) {
                valido = 0;
                break;
            }
        }
    } while (!valido);
}

void atualizarJogo() {
    if (!jogoAtivo) return;

    int novaX = (cobra.partes[0].x + direcaoX + COLUNAS) % COLUNAS;
    int novaY = (cobra.partes[0].y + direcaoY + LINHAS) % LINHAS;

    if (novaX == comida.local.x && novaY == comida.local.y) {
        cobra.comprimento++;
        cobra.partes = (Ponto *)realloc(cobra.partes, cobra.comprimento * sizeof(Ponto));
        if (cobra.partes == NULL) {
            printf("Erro ao realocar memória");
            exit(EXIT_FAILURE);
        }
        criarComida();
    } else {
        screenGotoxy(cobra.partes[cobra.comprimento - 1].x, cobra.partes[cobra.comprimento - 1].y);
        printf(" ");
    }

    for (int i = cobra.comprimento - 1; i > 0; i--) {
        cobra.partes[i] = cobra.partes[i - 1];
    }

    cobra.partes[0].x = novaX;
    cobra.partes[0].y = novaY;

    for (int i = 1; i < cobra.comprimento; i++) {
        if (cobra.partes[i].x == novaX && cobra.partes[i].y == novaY) {
            jogoAtivo = 0;
        }
    }
}

void salvarPlacar(int score) {
    int i;
    for (i = 0; i < MAX_SCORES; i++) {
        if (score > placares[i].score) {
            break;
        }
    }

    if (i < MAX_SCORES) {
        for (int j = MAX_SCORES - 1; j > i; j--) {
            placares[j] = placares[j - 1];
        }
        placares[i].score = score;
        strcpy(placares[i].nome, nomeJogador);

        FILE *arquivo = fopen("scores.txt", "w");
        if (arquivo != NULL) {
            for (int k = 0; k < MAX_SCORES; k++) {
                fprintf(arquivo, "%s %d\n", placares[k].nome, placares[k].score);
            }
            fclose(arquivo);
        }
    }
}

void carregarPlacares() {
    FILE *arquivo = fopen("scores.txt", "r");
    if (!arquivo) {
        for (int i = 0; i < MAX_SCORES; i++) {
            placares[i].score = 0;
            strcpy(placares[i].nome, "N/A");
        }
    } else {
        for (int i = 0; i < MAX_SCORES; i++) {
            fscanf(arquivo, "%s %d", placares[i].nome, &placares[i].score);
        }
        fclose(arquivo);
    }
}

void exibirPlacares() {
    screenClear();
    printf("=== Fim do Jogo ===\n");
    printf("Jogador: %s\n", nomeJogador);
    printf("Pontuação Final: %d ponto(s)\n\n", cobra.comprimento - 1);
    printf("Ranking:\n");
    for (int i = 0; i < MAX_SCORES; i++) {
        printf("%d. %s - %d pontos\n", i + 1, placares[i].nome, placares[i].score);
    }
}

int main() {
    iniciarJogo();
    carregarPlacares();
    criarComida();
    desenharLimites();
    desenharJogo();

    while (jogoAtivo) {
        if (timerTimeOver()) {
            if (keyhit()) {
                int tecla = readch();
                if (tecla == 27 || tecla == 'q') {
                    jogoAtivo = 0;
                } else if (tecla == 'w' && direcaoY != 1) {
                    direcaoX = 0;
                    direcaoY = -1;
                } else if (tecla == 'a' && direcaoX != 1) {
                    direcaoX = -1;
                    direcaoY = 0;
                } else if (tecla == 's' && direcaoY != -1) {
                    direcaoX = 0;
                    direcaoY = 1;
                } else if (tecla == 'd' && direcaoX != -1) {
                    direcaoX = 1;
                    direcaoY = 0;
                }
            }
            atualizarJogo();
            desenharJogo();
        }
    }

    salvarPlacar(cobra.comprimento - 1);
    exibirPlacares();
    free(cobra.partes);
    screenShowCursor();
    keyboardDestroy();
    timerDestroy();
    return 0;
}

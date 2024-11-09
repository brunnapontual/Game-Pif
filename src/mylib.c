#include "mylib.h"
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
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
void aumentarTamanho(SnakeP **cabeca, int x, int y) {
    SnakeP *novaParte = malloc(sizeof(SnakeP));
    novaParte->posX = x;
    novaParte->posY = y;
    novaParte->prox = NULL;
    if (*cabeca == NULL) {
        *cabeca = novaParte;
    } else {
        SnakeP *temp = *cabeca;
        while (temp->prox != NULL) temp = temp->prox;
        temp->prox = novaParte;
    }
}
void diminuirTamanho(SnakeP **cabeca) {
    if (*cabeca == NULL) {
        return;
    } if ((*cabeca)->prox == NULL) {
        free(*cabeca);
        *cabeca = NULL;
        return;
    }
    SnakeP *temp = *cabeca;
    while (temp->prox->prox != NULL) {
        temp = temp->prox;
    }
    free(temp->prox);
    temp->prox = NULL;
}
void desenharFruta(int x, int y, TipoFruta tipo) {
    if (tipo == FRUTA_COLORIDA){
        screenSetColor(GREEN, BLACK);//pensar na cor da fruta depois
    } else if (tipo == FRUTA_PRETO) {
        screenSetColor(BLACK, WHITE);
    }
    screenGotoxy(x, y);
    printf("@");
}
void verificarFrutaComida(SnakeP **cabeca, Fruta *fruta) {
    if (fruta->tipo == FRUTA_COLORIDA) {
        aumentarTamanho(cabeca, fruta->posX, fruta->posY);
    } else if (fruta->tipo == FRUTA_PRETO) {
        diminuirTamanho(cabeca);
    }
}
void novaFruta(int *x, int *y, TipoFruta *tipo) {
    *x = rand() % 68 + 8;
    *y = rand() % 18 + 2;
    *tipo = rand() % 2 == 0 ? FRUTA_COLORIDA : FRUTA_PRETO;
}
void limparCobra(SnakeP *cabeca) {
    while (cabeca != NULL) {
        screenGotoxy(cabeca->posX, cabeca->posY);
        printf(" ");
        cabeca = cabeca->prox;
    }
}
int verificarColisao(SnakeP *cabeca, int x, int y) {
    while (cabeca != NULL) {
        if (cabeca->posX == x && cabeca->posY == y) return 1;
        cabeca = cabeca->prox;
    }
    return 0;
}
void freeCobra(SnakeP **cabeca) {
    while (*cabeca != NULL) {
        SnakeP *temp = *cabeca;
        *cabeca = (*cabeca)->prox;
        free(temp);
    }
}
void adicionarPlacar(Placar **cabeca, Jogador jogador) {
    Placar *novo = malloc(sizeof(Placar));
    novo->jogador = jogador;
    novo->prox = NULL;

    if (*cabeca == NULL || (*cabeca)->jogador.pontos < jogador.pontos) {
        novo->prox = *cabeca;
        *cabeca = novo;
    } else {
        Placar *temp = *cabeca;
        while (temp->prox != NULL && temp->prox->jogador.pontos >= jogador.pontos) {
            temp = temp->prox;
        }
        novo->prox = temp->prox;
        temp->prox = novo;
    }
}
void salvarPlacar(Placar *cabeca, FILE *arquivo) {
    while (cabeca != NULL) {
        fwrite(&cabeca->jogador, sizeof(Jogador), 1, arquivo);
        cabeca = cabeca->prox;
    }
}
void atualizarPlacar(int pontos) {
    screenGotoxy(34,21);
    printf("Pontos: %d", pontos);
}
void freePlacar(Placar **cabeca) {
    while (*cabeca != NULL) {
        Placar *temp = *cabeca;
        *cabeca = (*cabeca)->prox;
        free(temp);
    }
}
void imprimirRanking(Placar *cabeca) {
    int posicao = 1;
    while (cabeca != NULL && posicao <= 3) {
        printf("%dº Posição:\nNome: %s\nPontuação: %d\n", posicao, cabeca->jogador.nome, cabeca->jogador.pontos);
        cabeca = cabeca->prox;
        posicao++;
    }
}
void GameOver() {
    screenGotoxy(30, 10);
    prinf("GAMER OVER");
    //lembrar de ajustar posicao e estilo depois
}

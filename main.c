#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "screen.h"
#include "cJSON.h"
#include <cjson/cJSON.h>

#define MAX_RANKING 100
#define MAX_TEMA 20
#define MAX_NOME 50
#define TOP_JOGADORES 3

typedef struct {
    char nome[MAX_NOME];
    int pontos;
} Jogador;

typedef struct {
    char** palavras;
    int tamanho;
} ListaPalavras;

ListaPalavras carregarPalavrasDoJSON(const char* arquivo, const char* tema) {
    FILE* file = fopen(arquivo, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo JSON");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* data = (char*)malloc(length + 1);
    fread(data, 1, length, file);
    fclose(file);
    data[length] = '\0';

    cJSON* json = cJSON_Parse(data);
    if (!json) {
        fprintf(stderr, "Erro ao analisar o JSON\n");
        free(data);
        exit(EXIT_FAILURE);
    }

    cJSON* array = cJSON_GetObjectItem(json, tema);
    if (!cJSON_IsArray(array)) {
        fprintf(stderr, "Tema não encontrado no JSON\n");
        cJSON_Delete(json);
        free(data);
        exit(EXIT_FAILURE);
    }

    int tamanho = cJSON_GetArraySize(array);
    char** palavras = (char**)malloc(tamanho * sizeof(char*));
    for (int i = 0; i < tamanho; i++) {
        cJSON* item = cJSON_GetArrayItem(array, i);
        palavras[i] = strdup(item->valuestring);
    }

    cJSON_Delete(json);
    free(data);

    ListaPalavras lista;
    lista.palavras = palavras;
    lista.tamanho = tamanho;
    return lista;
}

void liberarLista(ListaPalavras* lista) {
    for (int i = 0; i < lista->tamanho; i++) {
        free(lista->palavras[i]);
    }
    free(lista->palavras);
}

void desenharForca(int tentativasRestantes) {
    screenClear();
    screenSetColor(LIGHTRED, BLACK);
    printf("\nTentativas Restantes: %d\n", tentativasRestantes);
    switch (tentativasRestantes) {
        case 6: printf("\n\n\n\n\n=====\n"); break;
        case 5: printf("  O  \n\n\n\n\n=====\n"); break;
        case 4: printf("  O  \n  |  \n\n\n\n=====\n"); break;
        case 3: printf("  O  \n /|  \n\n\n\n=====\n"); break;
        case 2: printf("  O  \n /|\\ \n\n\n\n=====\n"); break;
        case 1: printf("  O  \n /|\\ \n /   \n\n\n=====\n"); break;
        case 0: printf("  O  \n /|\\ \n / \\ \n\n=====\n"); break;
    }
    screenSetColor(LIGHTGRAY, BLACK); 
}

void embaralharPalavras(char* palavras[], int tamanho) {
    for (int i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char* temp = palavras[i];
        palavras[i] = palavras[j];
        palavras[j] = temp;
    }
}

char* escolherPalavraDoTema(const ListaPalavras* lista) {
    embaralharPalavras(lista->palavras, lista->tamanho);
    char* palavraSelecionada = (char*)malloc(strlen(lista->palavras[0]) + 1);
    if (!palavraSelecionada) {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }
    strcpy(palavraSelecionada, lista->palavras[0]);
    return palavraSelecionada;
}

void escolherTema(char* tema) {
    int valido = 0;
    while (!valido) {
        printf("Escolha um tema (tecnologia, esportes, animais): ");
        fgets(tema, MAX_TEMA, stdin);
        tema[strcspn(tema, "\n")] = '\0';
        if (strcmp(tema, "tecnologia") == 0 || strcmp(tema, "esportes") == 0 || strcmp(tema, "animais") == 0) {
            valido = 1;
        } else {
            printf("Tema inválido! Tente novamente.\n");
        }
    }
}

void exibirDica(char* tema) {
    if (strcmp(tema, "tecnologia") == 0) {
        printf("Dica: Algo relacionado a tecnologia.\n");
    } else if (strcmp(tema, "esportes") == 0) {
        printf("Dica: Um esporte popular.\n");
    } else {
        printf("Dica: Um animal.\n");
    }
}

int ganhou(char* palavra, char* palavraAdivinhada) {
    return strcmp(palavra, palavraAdivinhada) == 0;
}

int carregarRanking(Jogador ranking[], int tamanhoMaximo) {
    FILE *file = fopen("ranking.txt", "r");
    if (!file) return 0;

    int count = 0;
    while (fscanf(file, "Nome: %49[^,], Pontos: %d\n", ranking[count].nome, &ranking[count].pontos) == 2) {
        count++;
        if (count >= tamanhoMaximo) break;
    }

    fclose(file);
    return count;
}

void salvarRanking(Jogador ranking[], int tamanho) {
    FILE *file = fopen("ranking.txt", "w");
    if (!file) {
        printf("Erro ao salvar o ranking.\n");
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        fprintf(file, "Nome: %s, Pontos: %d\n", ranking[i].nome, ranking[i].pontos);
    }

    fclose(file);
}

int compararJogadores(const void *a, const void *b) {
    Jogador *j1 = (Jogador *)a;
    Jogador *j2 = (Jogador *)b;
    return j2->pontos - j1->pontos;
}

void atualizarRanking(Jogador ranking[], int *tamanho, Jogador *jogadorAtual) {
    int encontrado = 0;

    for (int i = 0; i < *tamanho; i++) {
        if (strcmp(ranking[i].nome, jogadorAtual->nome) == 0) {
            ranking[i].pontos += jogadorAtual->pontos;
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        ranking[*tamanho] = *jogadorAtual;
        (*tamanho)++;
    }

    qsort(ranking, *tamanho, sizeof(Jogador), compararJogadores);
}

void exibirTop3(Jogador ranking[], int tamanho) {
    printf("\n=== Ranking dos 3 Melhores Jogadores ===\n");
    for (int i = 0; i < tamanho && i < TOP_JOGADORES; i++) {
        printf("%d. Nome: %s, Pontos: %d\n", i + 1, ranking[i].nome, ranking[i].pontos);
    }
}

int selecionarDificuldade() {
    int dificuldade = 0;
    while (dificuldade < 1 || dificuldade > 3) {
        printf("Escolha o nível de dificuldade: 1. Fácil (8 tentativas), 2. Médio (6 tentativas), 3. Difícil (4 tentativas): ");
        scanf("%d", &dificuldade);
        getchar();
    }

    if (dificuldade == 1) return 8;
    else if (dificuldade == 3) return 4;
    return 6;
}

void jogar(Jogador *jogador, char* tema) {
    ListaPalavras lista = carregarPalavrasDoJSON("palavras.json", tema);
    char* palavra = escolherPalavraDoTema(&lista);
    int tamanhoPalavra = strlen(palavra);
    char* palavraAdivinhada = (char*)malloc(tamanhoPalavra + 1);
    if (!palavraAdivinhada) {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    memset(palavraAdivinhada, '_', tamanhoPalavra);
    palavraAdivinhada[tamanhoPalavra] = '\0';

    int tentativasRestantes = selecionarDificuldade();
    char letra;

    exibirDica(tema);

    while (tentativasRestantes > 0 && !ganhou(palavra, palavraAdivinhada)) {
        screenClear();
        screenSetColor(LIGHTCYAN, BLACK);
        printf("=== Jogo da Forca ===\n");
        screenSetColor(LIGHTGRAY, BLACK);
        desenharForca(tentativasRestantes);
        printf("Palavra: %s\n", palavraAdivinhada);
        printf("Digite uma letra: ");
        scanf(" %c", &letra);
        letra = tolower(letra);

        int acertou = 0;
        for (int i = 0; i < tamanhoPalavra; i++) {
            if (palavra[i] == letra && palavraAdivinhada[i] == '_') {
                palavraAdivinhada[i] = letra;
                acertou = 1;
            }
        }

        if (!acertou) {
            tentativasRestantes--;
        }
    }

    if (ganhou(palavra, palavraAdivinhada)) {
        printf("\nParabéns, você venceu!\n");
        jogador->pontos += tamanhoPalavra * 10;
    } else {
        printf("\nVocê perdeu! A palavra era: %s\n", palavra);
    }

    free(palavra);
    free(palavraAdivinhada);
    liberarLista(&lista);
}

int main() {
    srand(time(NULL));
    char tema[MAX_TEMA];
    Jogador jogador;
    Jogador ranking[MAX_RANKING];
    int tamanhoRanking = carregarRanking(ranking, MAX_RANKING);
    char jogarNovamente;
    exibirTop3(ranking, tamanhoRanking);
    printf("\nDigite seu nome: ");
    fgets(jogador.nome, MAX_NOME, stdin);
    jogador.nome[strcspn(jogador.nome, "\n")] = '\0';
    jogador.pontos = 0;

    do {
        escolherTema(tema);
        jogar(&jogador, tema);
        atualizarRanking(ranking, &tamanhoRanking, &jogador);
        salvarRanking(ranking, tamanhoRanking);
        exibirTop3(ranking, tamanhoRanking);

        printf("\nDeseja jogar novamente? (s/n): ");
        jogarNovamente = getchar();
        getchar();
    } while (tolower(jogarNovamente) == 's');

    return 0;
}
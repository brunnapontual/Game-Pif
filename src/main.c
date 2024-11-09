/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
*/
#include "keyboard.h"
#include "mylib.h"
#include "screen.h"
#include "timer.h"


int main() {
  SnakeP *cobra = NULL;
  int tecla = 0, pontos = 0, dirX = 1, dirY = 0, intervalo = 100;
  FILE *arquivo;
  Jogador jogador;

  printf("Digite o nome do jogador: ");
  scanf("%s", jogador.nome);
  screenInit(1);
  keyboardInit();
  timerInit(intervalo);

  aumentarTamanho(&cobra, 34, 12);
  diminuirTamanho(&cobra);
  srand((unsigned int)time(NULL));
  int frutaX = rand() % 68 + 8, frutaY = rand() % 18 + 2;
  TipoFruta tipoFruta = rand() % 2 == 0 ? FRUTA_COLORIDA : FRUTA_PRETO;
  desenharFruta(frutaX, frutaY, tipoFruta);
  Fruta fruta = {frutaX, frutaY, tipoFruta};
  verificarFrutaComida(&cobra, &fruta);
  screenUpdate();
  sleep(3);

  while (tecla != SAIR) {
    if (keyhit()) {
      tecla = readch();
      switch (tecla) {
      case CIMA:
        if (dirY != 1) {
          dirX = 0;
          dirY = -1;
        }
        break;
      case BAIXO:
        if (dirY != -1) {
          dirX = 0;
          dirY = 1;
        }
        break;
      case ESQUERDA:
        if (dirX != 1) {
          dirX = -1;
          dirY = 0;
        }
        break;
      case DIREITA:
        if (dirX != -1) {
          dirX = 1;
          dirY = 0;
        }
        break;
      }
      screenUpdate();
    }
    if (timerTimeOver)) {
      int novoX = cobra->posX + dirX, novoY = cobra->posY + dirY;
      if (novoX >= MAXX || novoX <= MINX || novoY >= MAXY || novoY <= MINY || verificarColisao(cobra, novoX, novoY)){
        break;
      }
      if (novoX == frutaX && novoY == frutaY) {
        Fruta fruta = {frutaX, frutaY, tipoFruta};
        verificarFrutaComida(&cobra, &fruta);
        novaFruta(&frutaX, &frutaY, &tipoFruta);
        desenharFruta(frutaX, frutaY, tipoFruta);
        pontos++;
        intervalo = (intervalo > 20) ? (intervalo - 5) : 20;
        timerInit(intervalo);
      }
      atualizarPlacar(pontos);
      limparCobra(cobra);
      movimentoCobra(&cobra, novoX, novoY);
      desenharCobra(cobra);
      screenUpdate();
    }
  }
  freeCobra(&cobra);
  keyboardDestroy();
  screenDestroy();
  jogador.pontos = pontos;
  arquivo = fopen("ranking.txt", "a");
  fwrite(&jogador, sizeof(Jogador), 1, arquivo);
  fclose(arquivo);

  Placar *ranking = NULL;
  arquivo = fopen("ranking.txt", "r");
  while (fread(&jogador, sizeof(Jogador), 1, arquivo) == 1) {
    adicionarPlacar(&ranking, jogador);
  }
  fclose(arquivo);
  arquivo = fopen("ranking.txt", "w");
  salvarPlacar(ranking, arquivo);
  fclose(arquivo);
  imprimirRanking(ranking);
  GameOver();
  freePlacar(&ranking);
  timerDestroy();
  
  return 0;
}

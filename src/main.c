/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
*/

#include "keyboard.h"
#include "mylib.h"
#include "screen.h"

int main() {
  SnakeP *cobra = NULL;
  int tecla = 0, pontos = 0, dirX = 1, dirY = 0, intervalo = 100;
  FILE *arquivo;
  Jogador jogador;

  screenInit(1);
  keyboardInit();
  screenUpdate();

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
    }
  }
  return 0;
}

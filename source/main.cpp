#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

void press_enter(int accion);
void esperar(int milisegundos);

using namespace std;

#include "video.h"
#include "menu.h"
#include "fuentes.h"
#include "cartas.h"
#include "jugador.h"
#include "texto.h"
#include "present.h"

int main(int argc, char **argv) {

  /* Superficie y Fuente principal */
  SDL_Surface *pantalla;
  TTF_Font *fuente, *small;
  /* Instancias */
  Video video;
  Fuentes ft;
  Menu menu;
  Present pre;
  /* Obtener Superficie Principal */
  pantalla = video.init();
  /* Obtener Fuente Principal */
  fuente = ft.init("xirod.ttf",14);
  small = ft.init("xirod.ttf",10);
  /* Iniciar Presentación */
  pre.iniciar(pantalla, fuente);
  pre.free();
  /* Menú Principal */
  menu.controlar(pantalla, fuente, small);
  /* Liberar Memoria y Finalizar */
  TTF_CloseFont(fuente);
  TTF_CloseFont(small);
  TTF_Quit();
  SDL_FreeSurface(pantalla);
  SDL_Quit();
  return 0;
}

// Esperar Enter ó Clic en Botón Menú //
void press_enter(int accion) {
  int terminar = 0, mx = 0, my = 0;
  SDL_Event event;
// Esperar hasta que presione Enter //
  while(!terminar)
  {
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_KEYDOWN)
        if(event.key.keysym.sym == SDLK_RETURN)
          terminar = 1;
      if((event.type == SDL_MOUSEBUTTONUP) && (accion))
        if(event.button.button==SDL_BUTTON_LEFT)
        {
          SDL_GetMouseState(&mx, &my);
          //Volver al menú
          if((mx >= 604) && (my >= 10) && (mx <= 738) && (my <= 56))
            terminar = 1;
        }
    }
  }
}

// Esperar tantos milisegundos //
void esperar(int milisegundos) {
  int ahora, despues;
  ahora = SDL_GetTicks();
  do
  {
    despues = SDL_GetTicks();
  }
  while((despues - ahora) < milisegundos);
}

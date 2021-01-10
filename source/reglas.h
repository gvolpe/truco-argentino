#ifndef REGLAS_H
#define REGLAS_H

#include "imagen.h"

class Reglas
{
private:
  Imagen img;
  SDL_Surface *spr_reglas, *pantalla, *boton;
  SDL_Event event;
public:
  Reglas(SDL_Surface *scr);
  void free();
  void mostrar(SDL_Surface *fondo);
};

// Mostrar Reglamento //
void Reglas::mostrar(SDL_Surface *fondo)
{
  int y, seguir, mx, my;
  SDL_Rect f, d, f2, d2;
  Uint8 alpha;
  for(int i=0; i<6; i++)
  {
    y = (598*i);
    f = (SDL_Rect) {
      0, y, 770, 598
    };
    d = (SDL_Rect) {
      15, 0, 770, 598
    };
    /////////// Fade In /////////////
    for(alpha=0; alpha<255; alpha+=15)
    {
      SDL_SetAlpha(spr_reglas, SDL_SRCALPHA | SDL_RLEACCEL, alpha);
      SDL_BlitSurface(spr_reglas, &f, pantalla, &d);
      SDL_Flip(pantalla);
    }
    f2 = (SDL_Rect) {
      0, 0, 48, 30
    };
    d2 = (SDL_Rect) {
      752, 570, 48, 30
    };
    SDL_BlitSurface(boton, &f2, pantalla, &d2);
    SDL_Flip(pantalla);
    seguir = 0;
    while(!seguir)
    {
      while(SDL_PollEvent(&event))
      {
        if(event.type==SDL_MOUSEBUTTONUP)
          if(event.button.button==SDL_BUTTON_LEFT)
          {
            SDL_GetMouseState(&mx, &my);
            if((mx >= 752) && (my >= 570) && (mx <= 800) && (my <= 600))
              seguir = 1;
          }
        if(event.type == SDL_KEYDOWN)
          if(event.key.keysym.sym == SDLK_RETURN)
            seguir = 1;
      }
    }
    // Limpiar Flecha //
    SDL_BlitSurface(fondo, &d2, pantalla, &d2);
    SDL_Flip(pantalla);
  }
}

/* Constructor */
Reglas::Reglas(SDL_Surface *scr)
{
  spr_reglas = img.cargar_img("reglamento.png");
  boton = img.cargar_img("sig.png");
  SDL_SetColorKey(boton,SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(boton->format,255,0,0));
  pantalla = scr;
}

/* Liberar Memoria */
void Reglas::free()
{
  SDL_FreeSurface(spr_reglas);
  SDL_FreeSurface(pantalla);
  SDL_FreeSurface(boton);
}

#endif

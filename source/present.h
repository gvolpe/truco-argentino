#ifndef PRESENT_H
#define PRESENT_H

#include "imagen.h"

#define pi2 6.28318

class Present
{
private:
  SDL_Surface *fondo, *pres;
public:
  Present();
  void free();
  void limpiar(SDL_Surface *screen);
  void animar (SDL_Surface *ventana);
  void iniciar(SDL_Surface *screen, TTF_Font * font);
};

// Constructor //
Present::Present()
{
  Imagen img;
  pres = img.cargar_img("present.png");
  SDL_SetColorKey(pres,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(pres->format,255,255,0));
  fondo = img.cargar_img("inicio.png");
}

// Liberar Memoria //
void Present::free()
{
  SDL_FreeSurface(pres);
  SDL_FreeSurface(fondo);
}

// Iniciar la presentación //
void Present::iniciar(SDL_Surface *screen, TTF_Font * font)
{
  Texto txt;
  SDL_ShowCursor(SDL_DISABLE);
// Mostrar las imágenes //
  SDL_BlitSurface(fondo, 0, screen, 0);
  SDL_Rect d = {228, 169, pres->w, pres->h};
  SDL_BlitSurface(pres, 0, screen, &d);
  SDL_Flip(screen);
  txt.mostrar(294, 410, screen, "Presione Enter", 255, 180, 0, 255, font);
  press_enter(0);
  this->animar(screen);
  esperar(20);
}

// Animación de los naipes //
void Present::animar (SDL_Surface *ventana)
{
  Imagen ima;
  int flag = 0;
  double angulo = 0, zoom = 1, rotar = 0;
  SDL_Surface * imagen_copia;
  SDL_Event evento;
  SDL_Rect rect1;

  rect1.x = 393;
  rect1.y = 270;

  while(angulo<360)
  {
    // Rotación de la imagen //
    if(flag)
    {
      if(rotar<17)
        rotar++;
      if(rotar==17)
        flag=0;
    }
    else
    {
      if(rotar>-17)
        rotar--;
      if(rotar==-17)
        flag=1;
    }
    // Zoom y muestra de la imagen //
    if (angulo < 360)
    {
      angulo+=5;
      zoom = (angulo * pi2) / 360;
      imagen_copia = rotozoomSurface (this->pres, rotar, fabs(cos(zoom)), 1);

      rect1.x = 393 - imagen_copia->w / 2;
      rect1.y = 270 - imagen_copia->h / 2;

      limpiar(ventana);
      SDL_BlitSurface (imagen_copia, NULL, ventana, &rect1);
      SDL_FreeSurface (imagen_copia);
      SDL_Flip (ventana);
    }
    esperar (20);
  }
}

// Limpiar Pantalla //
void Present::limpiar(SDL_Surface *screen)
{
  SDL_Rect fd = {230, 150, 320, 250};
  SDL_BlitSurface(this->fondo, &fd, screen, &fd);
}

#endif

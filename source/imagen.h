#ifndef IMAGEN_H
#define IMAGEN_H

#include <string.h>

class Imagen
{
public:
  SDL_Surface * cargar_img (const char * fname);
};

SDL_Surface * Imagen::cargar_img (const char * fname)
{
  SDL_Surface *fondo;
  string imgName = "img/";
  imgName.append(fname);
  fondo = IMG_Load (imgName.c_str());
  if (fondo == NULL) {
    printf("SDL could not load image! SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }
  return fondo;
}

#endif

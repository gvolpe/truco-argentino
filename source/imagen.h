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
  if (fondo == NULL)
    exit(1);
  return fondo;
}

#endif

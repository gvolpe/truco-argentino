#ifndef IMAGEN_H
#define IMAGEN_H

class Imagen
{
public:
       SDL_Surface * cargar_img (const char * fname);
};

SDL_Surface * Imagen::cargar_img (const char * fname)
{
	SDL_Surface *fondo;
	fondo = IMG_Load (fname);
	if (fondo == NULL)	
	   exit(1);	       
	return fondo;	
}

#endif

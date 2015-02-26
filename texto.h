#ifndef TEXTO_H
#define TEXTO_H

class Texto
{
private:
	SDL_Surface *txt_img;	
public:
	void mostrar(int x, int y, SDL_Surface *pantalla, const char *txt, int r, int g, int b, int a, TTF_Font *font);		
	void free();
};

/* Mostrar texto en pantalla */
void Texto::mostrar(int x, int y, SDL_Surface *pantalla, const char *txt, int r, int g, int b, int a, TTF_Font *font)
{
// Sombra //
SDL_Color sombra = {0,0,0,255};
txt_img = TTF_RenderText_Blended(font, txt, sombra);
SDL_Rect f = {0, 0, txt_img->w, txt_img->h};
SDL_Rect d = {x-1, y-1, 0, 0}; 
SDL_BlitSurface(txt_img, &f, pantalla, &d);
// Texto en color //
SDL_Color color = {r,g,b,a};
txt_img = TTF_RenderText_Blended(font, txt, color);
f = (SDL_Rect) {0, 0, txt_img->w, txt_img->h};
d = (SDL_Rect) {x, y, 0, 0}; 
SDL_BlitSurface(txt_img, &f, pantalla, &d);
SDL_Flip(pantalla);
}

/* Liberar Memoria */
void Texto::free()
{
SDL_FreeSurface(txt_img);
}

#endif

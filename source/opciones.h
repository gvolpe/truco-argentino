#ifndef OPCIONES_H
#define OPCIONES_H

#include "constantes.h"

class Opciones
{
private:
	SDL_Surface *fondo, *options, *btn, *bmenu;
	int mano, faltas;
public:
	void mostrar(SDL_Surface *pantalla, int *m, int *f);
	void free();
	void limpiar_boton(SDL_Surface *pantalla, int x, int y);
	int getMano() {return mano;}
	int getFaltas() {return faltas;}
	void setMano(int m) {this->mano = m;}
	void setFaltas(int f) {this->faltas = f;}	
};

// Mostrar Opciones //
void Opciones::mostrar(SDL_Surface *pantalla, int *m, int *f)
{
Imagen img;
bmenu = img.cargar_img("boton_menu.png");
fondo = img.cargar_img("fondo.png");
options = img.cargar_img("opciones.png");
SDL_SetColorKey(options,SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(options->format,255,0,0));	
btn = img.cargar_img("select.png");
SDL_SetColorKey(btn,SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(btn->format,255,0,0));	
	
SDL_BlitSurface(fondo, 0, pantalla, 0);
SDL_Flip(pantalla);
int terminar = 0;
SDL_Event event;
Uint8 alpha;

int mx, my, sel_faltas, sel_mano;
int x = 121, y = 30;
int xb = 460, y_pc = 150, y_humano = 180, y_1 = 248, y_2 = 282;
SDL_Rect ft, d, fb, db1, db2;

// Opciones //
ft = (SDL_Rect) {0, 0, options->w, options->h};
d = (SDL_Rect) {x, y, 0, 0};

// Boton de Selección //
fb = (SDL_Rect) {0, 0, btn->w, btn->h};

if(*m == 0)
	db1 = (SDL_Rect) {xb, y_pc, 0, 0};
if(*m == 1)
	db1 = (SDL_Rect) {xb, y_humano, 0, 0};
if(*f == 1)
	db2 = (SDL_Rect) {xb, y_1, 0, 0};
if(*f == 2)
	db2 = (SDL_Rect) {xb, y_2, 0, 0};

for(alpha=0;alpha<255;alpha+=15)
	{
	SDL_SetAlpha(options, SDL_SRCALPHA | SDL_RLEACCEL, alpha);
	SDL_SetAlpha(btn, SDL_SRCALPHA | SDL_RLEACCEL, alpha);
	SDL_BlitSurface(options, &ft, pantalla, &d);	
	SDL_BlitSurface(btn, &fb, pantalla, &db1);	
	SDL_BlitSurface(btn, &fb, pantalla, &db2);	
	SDL_Flip(pantalla);
	esperar(30);
	}

// Mostrar Boton Menu //
int xbm = (400 - (bmenu->w / 2));
int ybm = (580 - bmenu->h);
SDL_Rect ori = {0, 0, bmenu->w, bmenu->h};
SDL_Rect dty = {xbm, ybm, 0, 0};
SDL_BlitSurface(bmenu, &ori, pantalla, &dty);
SDL_Flip(pantalla);

sel_mano = *m;
sel_faltas = *f;

// Esperar hasta que presione Enter ó Escape//
while(!terminar)
	{		
	while(SDL_PollEvent(&event))
		{		
		// Clic del mouse //
		if(event.type == SDL_MOUSEBUTTONUP)
			{
			if(event.button.button == SDL_BUTTON_LEFT)
				{
				SDL_GetMouseState(&mx, &my);
				// Volver al menu //
				if((mx >= xbm) && (my >= ybm) && (mx <= (xbm + bmenu->w)) && (my <= 580))
					terminar = 1;
				// Clic en los selectores //
            	if((mx >= xb - 10) && (my >= y_pc - 3) && (mx <= (xb + btn->w + 10)) && (my <= (y_pc + btn->h + 3)))
		          	{
					if(sel_mano == MANO_HUMANO)
						{					
						sel_mano = MANO_PC;
						this->limpiar_boton(pantalla, xb, y_humano);
						db1 = (SDL_Rect) {xb, y_pc, 0, 0};
						SDL_BlitSurface(btn, &fb, pantalla, &db1);	
						SDL_Flip(pantalla);
						}
					}
				if((mx >= xb - 10) && (my >= y_humano - 3) && (mx <= (xb + btn->w + 10)) && (my <= (y_humano + btn->h + 3)))
		          	{
					if(sel_mano == MANO_PC)
						{					
						sel_mano = MANO_HUMANO;
						this->limpiar_boton(pantalla, xb, y_pc);
						db1 = (SDL_Rect) {xb, y_humano, 0, 0};
						SDL_BlitSurface(btn, &fb, pantalla, &db1);	
						SDL_Flip(pantalla);
						}
					}
				if((mx >= xb - 10) && (my >= y_1 - 3) && (mx <= (xb + btn->w + 10)) && (my <= (y_1 + btn->h + 3)))
		          	{
					if(sel_faltas == DOS_FALTAS)
						{
						sel_faltas = UNA_FALTA;
						this->limpiar_boton(pantalla, xb, y_2);
						db2 = (SDL_Rect) {xb, y_1, 0, 0};
						SDL_BlitSurface(btn, &fb, pantalla, &db2);	
						SDL_Flip(pantalla);
						}
					}
				if((mx >= xb - 10) && (my >= y_2 - 3) && (mx <= (xb + btn->w + 10)) && (my <= (y_2 + btn->h + 3)))
		          	{ 
					if(sel_faltas == UNA_FALTA)
						{
						sel_faltas = DOS_FALTAS;
						this->limpiar_boton(pantalla, xb, y_1);
						db2 = (SDL_Rect) {xb, y_2, 0, 0};
						SDL_BlitSurface(btn, &fb, pantalla, &db2);	
						SDL_Flip(pantalla);
						}
					}
				}
			}
		// Controlar los eventos de teclado //	
		if(event.type == SDL_KEYDOWN)
			{
			if(event.key.keysym.sym == SDLK_RETURN)
				terminar = 1;			
			}
		}
	}
*m = sel_mano;
*f = sel_faltas;
}

// Limpiar Botón de Selección //
void Opciones::limpiar_boton(SDL_Surface *pantalla, int x, int y)
{
SDL_Rect fd = {x, y, 20, 20};
SDL_BlitSurface(fondo, &fd, pantalla, &fd);
SDL_Flip(pantalla);
}

// Liberar Memoria //
void Opciones::free()
{
SDL_FreeSurface(fondo);
SDL_FreeSurface(options);
SDL_FreeSurface(btn);
SDL_FreeSurface(bmenu);
}


#endif

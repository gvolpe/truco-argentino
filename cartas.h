#ifndef CARTAS_H
#define CARTAS_H

class Carta
{
private:
	int tam_naipe;			
	SDL_Surface *spr_naipes, *back;
public:		
	Carta(); //Constructor	
	void free();
	void mostrar_jugadas(SDL_Surface *fondo, SDL_Surface *screen, int cj, int cs, int *p_naipes);
	void mostrar_jugadas_pc(SDL_Surface *fondo, SDL_Surface *screen, int cj, int cs, int *c_naipes);
	void mostrar_servidas(SDL_Surface *fondo, SDL_Surface *screen, int *p_naipes);
};

/*************** Visualizar Naipes Jugadas por Computadora ****************/
void Carta::mostrar_jugadas_pc(SDL_Surface *fondo, SDL_Surface *screen, int cj, int cs, int *c_naipes)
{
// cj: Posición carta jugada            cs: Posición carta servida //
int x, x2;
SDL_Rect f, d;
/* Limpiar Cartas PC Ocultas */	
x = (27 + (tam_naipe+10) * (cs));
f = (SDL_Rect) {x, 410, back->w, back->h};
d = (SDL_Rect) {x, 410, 0, 0};
SDL_BlitSurface(fondo, &f, screen, &d);	
SDL_Flip(screen);
/* Cartas PC Jugadas */
x = (tam_naipe * (c_naipes[cs]));
x2 = (((tam_naipe+10) * (cj))-50);
d = (SDL_Rect) {x2, 20, 0, 0};
f = (SDL_Rect) {x, 0, (spr_naipes->w / 40), spr_naipes->h};
SDL_BlitSurface(spr_naipes, &f, screen, &d);	
SDL_Flip(screen);	
}

/**************** Visualizar Naipes Jugadas por Player ******************/
void Carta::mostrar_jugadas(SDL_Surface *fondo, SDL_Surface *screen, int cj, int cs, int *p_naipes)
{
// cj: Posición carta jugada            cs: Posición carta servida //
int x, x2;
SDL_Rect f, d;
/* Limpiar Cartas Servidas */		
x  = (345+((tam_naipe+10) * (cs+1)));
f = (SDL_Rect) {x, 410, tam_naipe, 137};
d = (SDL_Rect) {x, 410, tam_naipe, 137};
SDL_BlitSurface(fondo, &f, screen, &d);	
SDL_Flip(screen);		
/* Cartas Jugador Jugadas */
x = (tam_naipe * (p_naipes[cs]));
x2 = (((tam_naipe+10) * (cj))-50);
d = (SDL_Rect) {x2, 170, 0, 0};
f = (SDL_Rect) {x, 0, (spr_naipes->w / 40), spr_naipes->h};
SDL_BlitSurface(spr_naipes, &f, screen, &d);	
SDL_Flip(screen);	
}

/**************** Visualizar Naipes Servidas ******************/
void Carta::mostrar_servidas(SDL_Surface *fondo, SDL_Surface *screen, int *p_naipes)
{
int i, x, x2;
SDL_Rect f, d;

for(i=0;i<3;i++)
	{		
	/* Limpiar Cartas PC Jugadas */	
	x = (((tam_naipe+10) * (i+1))-50);
	f = (SDL_Rect) {x, 20, 99, 137};
	d = (SDL_Rect) {x, 20, 99, 137};	
	SDL_BlitSurface(fondo, &f, screen, &d);	
	SDL_Flip(screen);	
	/* Limpiar Cartas Player Jugadas */		
	x  = (((tam_naipe+10) * (i+1))-50);
	f = (SDL_Rect) {x, 170, 99, 137};
	d = (SDL_Rect) {x, 170, 99, 137};
	SDL_BlitSurface(fondo, &f, screen, &d);	
	SDL_Flip(screen);		
	/* Cartas PC Ocultas */	
	x = (27 + (tam_naipe+10) * i);
	f = (SDL_Rect) {0, 0, back->w, back->h};
	d = (SDL_Rect) {x, 410, 0, 0};
	SDL_BlitSurface(back, &f, screen, &d);	
	SDL_Flip(screen);
	/* Cartas Jugador Servidas */
	x = (tam_naipe * (p_naipes[i]));
	x2 = (345+((tam_naipe+10) * (i+1)));
	d = (SDL_Rect) {x2, 410, 0, 0};
	f = (SDL_Rect) {x, 0, (spr_naipes->w / 40), spr_naipes->h};
	SDL_BlitSurface(spr_naipes, &f, screen, &d);	
	SDL_Flip(screen);
	}	
}

/**************** Inicializaciones ******************/
Carta::Carta()
{
spr_naipes = IMG_Load ("naipes.png"); if (spr_naipes == NULL) exit(1);
back = IMG_Load ("back_naipe.png"); if (back == NULL) exit(1);
tam_naipe = 99;  // Tamaño de cada Sprite
}
/**************** Liberar Memoria de los Sprites ******************/
void Carta::free()
{
SDL_FreeSurface(spr_naipes);
SDL_FreeSurface(back);
}
#endif

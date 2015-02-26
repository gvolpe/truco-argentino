#ifndef CREDITOS_H
#define CREDITOS_H

class Creditos
{
private:
	SDL_Surface *fondo, *creds, *bmenu;	
public:	
	void free();
	void mostrar(SDL_Surface *pantalla);		
};

// Mostrar Créditos //
void Creditos::mostrar(SDL_Surface *pantalla)
{
// Cargar las imágenes //
Imagen img;
bmenu = img.cargar_img("boton_menu.png");
fondo = img.cargar_img("fondo.png");
creds = SDL_LoadBMP("creditos.bmp");
SDL_SetColorKey(creds,SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(creds->format,255,0,0));		
// Mostrarlas //	
SDL_BlitSurface(fondo, 0, pantalla, 0);
SDL_Flip(pantalla);
int terminar = 0, mx, my;
Uint8 alpha;
SDL_Event event;
int x = 112, y = 25;
SDL_Rect f, d;

f = (SDL_Rect) {0, 0, creds->w, creds->h};
d = (SDL_Rect) {x, y, 0, 0};

esperar(300);
// Mostrar Créditos //	
for(alpha=0;alpha<255;alpha+=15)
	{
	SDL_SetAlpha(creds, SDL_SRCALPHA | SDL_RLEACCEL, alpha);
	SDL_BlitSurface(creds, &f, pantalla, &d);	
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

// Esperar hasta que presione Enter ó Escape//
while(!terminar)
	{		
	// Controlar los eventos de teclado //
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
				}
			}
		// Teclado //
		if(event.type == SDL_KEYDOWN)
			{
			if(event.key.keysym.sym == SDLK_RETURN)
				terminar = 1;		
			if(event.key.keysym.sym == SDLK_ESCAPE)
				terminar = 1;
			}
		}
	}
}

// Liberar Memoria //
void Creditos::free()
{
SDL_FreeSurface(fondo);
SDL_FreeSurface(creds);
SDL_FreeSurface(bmenu);
}

#endif

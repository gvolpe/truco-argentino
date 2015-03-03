#ifndef MENU_H
#define MENU_H

#include "imagen.h"
#include "texto.h"
#include "opciones.h"
#include "juego.h"
#include "reglas.h"
#include "creditos.h"

void juegoNuevo(SDL_Surface *pantalla, TTF_Font *font, Opciones op);
void mostrarOpciones(SDL_Surface *pantalla, Opciones op, int *m, int *f);
void mostrarReglamento(SDL_Surface *pantalla, SDL_Surface *fondo);
void mostrarCreditos(SDL_Surface *pantalla);

class Menu
{
private:	   		   
	   int x,y,dist;       
	   Texto txt;	
	   Imagen img;        
	   SDL_Surface *img_fondo, *gaucho;              
public:
       void mostrar(int op, SDL_Surface * pantalla, TTF_Font * font);
       void controlar(SDL_Surface *pantalla, TTF_Font *font, TTF_Font *sfont);
       void mostrar_menu(SDL_Surface * pantalla);
       Menu();       
};

/* Mostrar Fondo de Menu */
void Menu::mostrar_menu(SDL_Surface * pantalla)
{
SDL_Rect f = {22,0,800,600};
SDL_Rect d = {x-130,y-20,gaucho->w,gaucho->h};
SDL_BlitSurface(img_fondo, &f, pantalla, NULL);
SDL_BlitSurface(gaucho, 0, pantalla, &d);
SDL_Flip(pantalla);
}

/******************************** Inicializaciones *********************************/
Menu::Menu()
{
x = 378;
y = 200;
dist = 40;
}

/******************************** Mostrar Opciones ***************************************/
void Menu::mostrar(int op, SDL_Surface * pantalla, TTF_Font * font)
{
txt.mostrar(x,y,pantalla,"JUEGO NUEVO",255,149,53,255,font);
txt.mostrar(x,y+dist,pantalla,"OPCIONES",255,149,53,255,font);
txt.mostrar(x,y+(dist*2),pantalla,"REGLAMENTO",255,149,53,255,font);
txt.mostrar(x,y+(dist*3),pantalla,"CREDITOS",255,149,53,255,font);
txt.mostrar(x,y+(dist*4),pantalla,"SALIR",255,149,53,255,font);
switch(op)
      {
      case 1: txt.mostrar(x,y,pantalla,"JUEGO NUEVO",255,255,180,255,font); break;
      case 2: txt.mostrar(x,y+dist,pantalla,"OPCIONES",255,255,180,255,font); break;
      case 3: txt.mostrar(x,y+(dist*2),pantalla,"REGLAMENTO",255,255,180,255,font); break;
      case 4: txt.mostrar(x,y+(dist*3),pantalla,"CREDITOS",255,255,180,255,font); break;
      case 5: txt.mostrar(x,y+(dist*4),pantalla,"SALIR",255,255,180,255,font); break;
      }
}

/******************************** Menú de opciones ***************************************/
void Menu::controlar(SDL_Surface *pantalla, TTF_Font *font, TTF_Font *sfont)
{
SDL_Event event;
	
int salida = 0; //Controla la Salida
int dx = 163, dy = 15; //Distancias Horizontal y Vertical
int mx, my; //Coordenadas del mouse
int sel = 1; //Opción seleccionada (Comenzamos con la primera)
int m, f; // Mano y Faltas

img_fondo = img.cargar_img("menu.png");
gaucho = img.cargar_img("gaucho.png");
SDL_SetColorKey(gaucho,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(gaucho->format,255,255,0));
this->mostrar_menu(pantalla);
this->mostrar(1, pantalla, font); //Comenzar con la primera opción seleccionada
SDL_ShowCursor(SDL_ENABLE);

/* Establecer Valores de las Opciones */
Opciones opt;
opt.setMano(0);
opt.setFaltas(1);

/* Ciclo de Manejo de Eventos de Usuario */
while(!salida)
	{
	while(SDL_PollEvent(&event))
		{
		if(salida==1) break;
		switch(event.type)
			{             						
			/// Manejar Moviemientos del ratón ///
			case SDL_MOUSEMOTION: 				
					SDL_GetMouseState(&mx, &my);
					//Juego Nuevo
					if((mx >= x) && (my >= y) && (mx <= (x+dx)) && (my <= (y+dy)))								
							{	
							if(sel != JUEGO_NUEVO)														
	                            this->mostrar(1, pantalla, font); 
	                        sel = JUEGO_NUEVO;
                            }
					//Opciones
					if((mx >= x) && (my >= y+dist) && (mx <= (x+dx)) && (my <= (y+dist+dy)))
							{
							if(sel != OPCIONES)	
	                            this->mostrar(2, pantalla, font); 
	                        sel = OPCIONES;
                            }
					//Reglamento
					if((mx >= x) && (my >= y+(dist*2)) && (mx <= (x+dx)) && (my <= (y+(dist*2)+dy)))
					       {
						   if(sel != REGLAMENTO)	
                           		this->mostrar(3, pantalla, font);
                           	sel = REGLAMENTO;
                           }
					//Créditos
					if((mx >= x) && (my >= y+(dist*3)) && (mx <= (x+dx)) && (my <= (y+(dist*3)+dy)))
					       {
						   if(sel != CREDITOS) 	
						   		this->mostrar(4, pantalla, font);
						   sel = CREDITOS;	
                           }
					//Salir
					if((mx >= x) && (my >= y+(dist*4)) && (mx <= (x+dx)) && (my <= (y+(dist*4)+dy)))
					       {
						   if(sel != SALIR) 		
                            	this->mostrar(5, pantalla, font);                            
                           sel = SALIR; 
                           }					
					break;		
			/// Manejar pulsaciones de teclas ///
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
					{			
					// Abajo		
					case SDLK_DOWN:
						if(sel < SALIR)
							sel++;
						else
							sel = JUEGO_NUEVO;						
						this->mostrar(sel, pantalla, font); 
						break;							
					// Arriba
					case SDLK_UP:
						if(sel > JUEGO_NUEVO)
							sel--;
						else
							sel = SALIR;
						this->mostrar(sel, pantalla, font); 
						break;		
					// Escape
					case SDLK_ESCAPE:
						salida = 1;
						break;								
					// Enter
					case SDLK_RETURN:
						switch(sel)
							{
							case JUEGO_NUEVO:
								juegoNuevo(pantalla, font, opt);
								this->mostrar_menu(pantalla);
								this->mostrar(1, pantalla, font);
								break;								
							case OPCIONES:
								mostrarOpciones(pantalla, opt, &m, &f);
								opt.setMano(m);
								opt.setFaltas(f);
								this->mostrar_menu(pantalla);
						        this->mostrar(2, pantalla, font);
								break;
							case REGLAMENTO:
								mostrarReglamento(pantalla, img_fondo);								   
			     			    this->mostrar_menu(pantalla);
						        this->mostrar(3, pantalla, font);						   
							    break;
							case CREDITOS:
								mostrarCreditos(pantalla);
							    this->mostrar_menu(pantalla);
						        this->mostrar(4, pantalla, font);
								break;
							case SALIR:
								salida = 1;
								break;
							}
						break;
					}
				break;	
			/// Manejar Clics del ratón ///
			case SDL_MOUSEBUTTONUP:	
				if(event.button.button==SDL_BUTTON_LEFT)					
					{
					SDL_GetMouseState(&mx, &my);					
					if((mx >= x) && (my >= y) && (mx <= (x+dx)) && (my <= (y+dy)))								
							{								
							juegoNuevo(pantalla, font, opt);
							this->mostrar_menu(pantalla);
							this->mostrar(1, pantalla, font);							
                            }					
					if((mx >= x) && (my >= y+dist) && (mx <= (x+dx)) && (my <= (y+dist+dy)))
							{
							mostrarOpciones(pantalla, opt, &m, &f);
							opt.setMano(m);
							opt.setFaltas(f);
							this->mostrar_menu(pantalla);
					        this->mostrar(2, pantalla, font);						    
                            }					
					if((mx >= x) && (my >= y+(dist*2)) && (mx <= (x+dx)) && (my <= (y+(dist*2)+dy)))
					       {
						   mostrarReglamento(pantalla, img_fondo);
						   this->mostrar_menu(pantalla);
					       this->mostrar(3, pantalla, font);						   
                           }					
					if((mx >= x) && (my >= y+(dist*3)) && (mx <= (x+dx)) && (my <= (y+(dist*3)+dy)))
					       {						   
						   mostrarCreditos(pantalla);
						   this->mostrar_menu(pantalla);
					       this->mostrar(4, pantalla, font);
                           }					
					if((mx >= x) && (my >= y+(dist*4)) && (mx <= (x+dx)) && (my <= (y+(dist*4)+dy)))
					       {
						   salida=1;
                           }					
					}
				break;					
			}
		}
	}
txt.free();
SDL_FreeSurface(img_fondo);
SDL_FreeSurface(gaucho);
}

void juegoNuevo(SDL_Surface *pantalla, TTF_Font *font, Opciones op)
{
Game game(pantalla);
game.nuevo(font, op);	                        
game.free();
}

void mostrarOpciones(SDL_Surface *pantalla, Opciones op, int *m, int *f)
{
*m = op.getMano();
*f = op.getFaltas();
op.mostrar(pantalla, m, f);
op.free();
}

void mostrarReglamento(SDL_Surface *pantalla, SDL_Surface *fondo)
{
Reglas reg(pantalla);
reg.mostrar(fondo);   
reg.free();
}

void mostrarCreditos(SDL_Surface *pantalla)
{
Creditos cred;
cred.mostrar(pantalla);
cred.free();
}

#endif

#ifndef JUGADOR_H
#define JUGADOR_H

#include "cartas.h"
#include "mazo.h"

class Jugador
{
private:
  Mazo mazo;
  SDL_Event event;
  int cx1, cy1, cx2, cy2, dist;
public:
  int jugar(SDL_Surface *img_fondo, SDL_Surface *pantalla,int *naipes,Carta carta, int *cant_j, int *truco_player, int *flag, int accion);
  int ingresar_tantos(SDL_Surface *img_fondo, SDL_Surface *pantalla, TTF_Font *font, int accion);
  void limpiar_texto(SDL_Surface *fondo, SDL_Surface *pantalla, int pos);
  Jugador(int Cx1, int Cy1, int Cx2, int Cy2, int Dist);
};

//////////////////////////////// Limpiar Tantos /////////////////////////////////
void Jugador::limpiar_texto(SDL_Surface *fondo, SDL_Surface *pantalla, int pos)
{
  SDL_Rect f = {pos, 245, 18, 20};
  SDL_BlitSurface(fondo, &f, pantalla, &f);
  SDL_Flip(pantalla);
}

///////////////////////////////  Ingresar tantos del Envido ///////////////////////////////
int Jugador::ingresar_tantos(SDL_Surface *img_fondo, SDL_Surface *pantalla, TTF_Font *font, int accion)
{
  SDL_Event evento;
  SDL_Surface *son_buenas;
  SDL_Rect f, dest;
  int salir = 0, tantos = -1, cant = 0, buenas = 0;
  int x = 463, y = 250, d = 18;
  int r = 200, g = 150, b = 0; //Colores
  int mx, my; //Coordenadas del mouse
  char cad[2];
  Texto txt;
  Imagen img;
  if(accion)
  {
    /// Mostrar el botón son buenas ///
    son_buenas = img.cargar_img("son_buenas.png");
    f = (SDL_Rect) {
      0,0,son_buenas->w,son_buenas->h
    };
    dest = (SDL_Rect) {
      423,180,son_buenas->w,son_buenas->h
    };
    SDL_BlitSurface(son_buenas, &f, pantalla, &dest);
    SDL_Flip(pantalla);
    SDL_FreeSurface(son_buenas);
  }
/// Ciclo de manejo de eventos ///
  while(!salir)
  {
    while(SDL_PollEvent(&evento))
    {
      // Clic del mouse //
      if((evento.type == SDL_MOUSEBUTTONUP) && (accion))
      {
        if(evento.button.button == SDL_BUTTON_LEFT)
        {
          SDL_GetMouseState(&mx, &my);
          if((mx >= 423) && (my >= 180) && (mx <= (423 + son_buenas->w)) && (my <= (180 + son_buenas->h)))
          {
            salir = 1;
            buenas = 1;
          }
        }
      }
      // Eventos del teclado //
      if(evento.type == SDL_KEYDOWN)
      {
        switch(evento.key.keysym.sym)
        {
        case SDLK_RETURN:
          if(((cant==2) || (cant==1)) && ((tantos >= 0) && (tantos <=33)))
            salir = 1;
          break;
        case SDLK_KP_ENTER:
          if(((cant==2) || (cant==1)) && ((tantos >= 0) && (tantos <=33)))
            salir = 1;
          break;
        case SDLK_BACKSPACE:
          if(cant>0)
          {
            cant--;
            cad[cant] = ' ';
            this->limpiar_texto(img_fondo, pantalla, x+(cant*d));
          }
          break;
        case SDLK_KP0:
          if(cant<2)
          {
            cad[cant] = '0';
            txt.mostrar(x+(cant*d), y, pantalla, "0", r, g, b, 200, font);
            cant++;
          }
          break;
        case SDLK_KP1:
          if(cant<2)
          {
            cad[cant] = '1';
            txt.mostrar(x+(cant*d), y, pantalla, "1", r, g, b, 200, font);
            cant++;
          }
          break;
        case SDLK_KP2:
          if(cant<2)
          {
            cad[cant] = '2';
            txt.mostrar(x+(cant*d), y, pantalla, "2", r, g, b, 200, font);
            cant++;
          }
          break;
        case SDLK_KP3:
          if(cant<2)
          {
            cad[cant] = '3';
            txt.mostrar(x+(cant*d), y, pantalla, "3", r, g, b, 200, font);
            cant++;
          }
          break;
        case SDLK_KP4:
          if(cant<2)
          {
            cad[cant] = '4';
            txt.mostrar(x+(cant*d), y, pantalla, "4", r, g, b, 200, font);
            cant++;
          }
          break;
        case SDLK_KP5:
          if(cant<2)
          {
            cad[cant] = '5';
            txt.mostrar(x+(cant*d), y, pantalla, "5", r, g, b, 200, font);
            cant++;
          }
          break;
        case SDLK_KP6:
          if(cant<2)
          {
            cad[cant] = '6';
            txt.mostrar(x+(cant*d), y, pantalla, "6", r, g, b, 200, font);
            cant++;
          }
          break;
        case SDLK_KP7:
          if(cant<2)
          {
            cad[cant] = '7';
            txt.mostrar(x+(cant*d), y, pantalla, "7", r, g, b, 200, font);
            cant++;
          }
          break;
        case SDLK_KP8:
          if(cant<2)
          {
            cad[cant] = '8';
            txt.mostrar(x+(cant*d), y, pantalla, "8", r, g, b, 200, font);
            cant++;
          }
          break;
        case SDLK_KP9:
          if(cant<2)
          {
            cad[cant] = '9';
            txt.mostrar(x+(cant*d), y, pantalla, "9", r, g, b, 200, font);
            cant++;
          }
          break;
        }
      }
    }
    tantos = int(atoi(cad));
  }
  if(accion)
  {
    // Borrar el botón Son buenas //
    SDL_BlitSurface(img_fondo, &dest, pantalla, &dest);
    SDL_Flip(pantalla);
  }
  if(buenas)
    return -1;
// Retornar tantos //
  return tantos;
}

///////////////////////////////  Constructor ///////////////////////////////
Jugador::Jugador(int Cx1, int Cy1, int Cx2, int Cy2, int Dist)
{
  cx1 = Cx1;
  cy1 = Cy1;
  cx2 = Cx2;
  cy2 = Cy2;
  dist = Dist;
}

///////////////////////////// Manejar Eventos del Humano //////////////////////////////
int Jugador::jugar(SDL_Surface *img_fondo, SDL_Surface *pantalla, int *naipes, Carta carta, int *cant_j, int *truco_player, int *flag, int accion)
{
  int salir=0;
  int mx, my; //Coordenadas del mouse
  int bx = 555, bx2 = 116, by = 100, by2 = 6; //Coordenadas de los botones
  int ydist = 34, xdist = 110;
// Ciclo de Manejo de Eventos de Usuario
  while(!salir)
  {
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
      // Manejar pulsaciones de teclas
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
          salir = 1;
          break;
        }
        break;
      // Manejar el ratón
      case SDL_MOUSEBUTTONUP:
        if(event.button.button==SDL_BUTTON_LEFT)
        {
          SDL_GetMouseState(&mx, &my);

          //Volver al menú
          if((mx >= 604) && (my >= 10) && (mx <= 738) && (my <= 56))
            salir = 1;

          //Botón Al Mazo
          if((mx >= (bx+58)) && (my >= (by+(by2+ydist)*4)) && (mx <= (bx+58+xdist)) && (my <= (by+(by2+ydist)*4)+ydist))
            *truco_player = AL_MAZO;

          //Jugar alguna naipe
          if(accion!=2)
          {
            //Primera Carta
            if((mx >= cx1) && (my >= cy1) && (mx <= cx2) && (my <= cy2))
            {
              if(cant_j[0]==0)
              {
                cant_j[0]=1;
                carta.mostrar_jugadas(img_fondo,pantalla,(cant_j[0]+cant_j[1]+cant_j[2]),0,naipes);
                return 0;
              }
            }
            //Segunda Carta
            if((mx >= cx1+dist) && (my >= cy1) && (mx <= cx2+dist) && (my <= cy2))
            {
              if(cant_j[1]==0)
              {
                cant_j[1]=1;
                carta.mostrar_jugadas(img_fondo,pantalla,(cant_j[0]+cant_j[1]+cant_j[2]),1,naipes);
                return 1;
              }
            }
            //Tercera Carta
            if((mx >= cx1+(dist*2)) && (my >= cy1) && (mx <= cx2+(dist*2)) && (my <= cy2))
            {
              if(cant_j[2]==0)
              {
                cant_j[2]=1;
                carta.mostrar_jugadas(img_fondo,pantalla,(cant_j[0]+cant_j[1]+cant_j[2]),2,naipes);
                return 2;
              }
            }
          }

          //Elegir alguna opción
          if(accion!=1)
          {
            //Botón Truco
            if((mx >= bx) && (my >= by) && (mx <= (bx+xdist)) && (my <= (by+ydist)))
            {
              if(flag[0]==1)
                *truco_player = TRUCO;
            }
            //Botón Retruco
            if((mx >= bx) && (my >= (by+by2+ydist)) && (mx <= (bx+xdist)) && (my <= (by+by2+ydist*2)))
            {
              if(flag[1]==1)
                *truco_player = RETRUCO;
            }
            //Botón Vale 4
            if((mx >= bx) && (my >= (by+(by2+ydist)*2)) && (mx <= (bx+xdist)) && (my <= (by+(by2+ydist)*2)+ydist))
            {
              if(flag[2]==1)
                *truco_player = VALE4;
            }
            //Botón Quiero
            if((mx >= bx) && (my >= (by+(by2+ydist)*3)) && (mx <= (bx+xdist)) && (my <= (by+(by2+ydist)*3)+ydist))
            {
              if(flag[3]==1)
                *truco_player = QUIERO;
            }
            //Botón Envido
            if((mx >= (bx+bx2)) && (my >= by) && (mx <= (bx+bx2+xdist)) && (my <= (by+ydist)))
            {
              if(flag[4]==1)
                *truco_player = ENVIDO;
            }
            //Botón Real Envido
            if((mx >= (bx+bx2)) && (my >= (by+by2+ydist)) && (mx <= (bx+bx2+xdist)) && (my <= (by+by2+ydist*2)))
            {
              if(flag[5]==1)
                *truco_player = REAL_ENVIDO;
            }
            //Botón Falta Envido
            if((mx >= (bx+bx2)) && (my >= (by+(by2+ydist)*2)) && (mx <= (bx+bx2+xdist)) && (my <= (by+(by2+ydist)*2)+ydist))
            {
              if(flag[6]==1)
                *truco_player = FALTA_ENVIDO;
            }
            //Botón No Quiero
            if((mx >= (bx+bx2)) && (my >= (by+(by2+ydist)*3)) && (mx <= (bx+bx2+xdist)) && (my <= (by+(by2+ydist)*3)+ydist))
            {
              if(flag[7]==1)
                *truco_player = NO_QUIERO;
            }
          }
        }
        break;
      }
    }
    if(*truco_player>-1)
      return 3;
  }
  return -1;
}

#endif

#ifndef JUEGO_H
#define JUEGO_H

using namespace std;

#include <stdlib.h>
#include <string.h>
#include "itoa.h"
#include "cartas.h"
#include "mazo.h"
#include "jugador.h"
#include "imagen.h"
#include "compu.h"

class Game
{
private:
  Mazo mazo;
  Imagen img;
  int cx1, cx2, cy1, cy2, dist; // Coordenadas de las cartas
  int botones_flag[9]; // Botones Truco, Envido, Etc.
  int jerarquia[40]; // Jerarquía de los naipes
  int valores[40]; // Valor de cada naipe en el envido (A la par de Naipes[40] de la clase Mazo)
  int carta_pc[3]; // Naipes jugadas por PC
  int carta_player[3]; // Naipes jugadas por el humano
  SDL_Event event;
  SDL_Surface *pantalla, *fondo, *bmenu, *b_enabled, *b_disabled;
  int c_naipes[3]; // Naipes repartidas a PC
  int p_naipes[3]; // Naipes repartidas al humano
public:
  void nuevo(TTF_Font *font, Opciones opt);
  void set_status(int b0, int b1, int b2, int b3, int b4, int b5, int b6, int b7, int b8);
  int comparar(int pos_pc, int pos_humano, int canto_4, TTF_Font *font);
  int calcular_tantos(int player, int accion); //0: Humano/Tantos    1: Computadora/Pos
  int menor_valor(int *naipes);
  int pos_menor_valor(int *naipes);
  int mayor_valor(int *naipes);
  int pos_mayor_valor(int *naipes);
  Game(SDL_Surface *screen); //Constructor
  void free();
  void mostrar_botones();
  void limpiar_texto();
  void limpiar_puntaje();
  void mostrar_msj(int num_msj, TTF_Font *font);
  void mostrar_puntaje(int pc, int player, TTF_Font *font);
};


////////////////////////////// Mostrar Opciones de la Partida ////////////////////////////////
void Game::mostrar_botones()
{
  /* 0: Truco         1: Retruco        2: Vale 4            3: Quiero
     4: Envido        5: Real Envido    6: Falta Envido      7: No Quiero
     8: Al mazo       */
  SDL_Rect f, d;
  int x=0, y=0;
  for(int i = TRUCO; i <= AL_MAZO; i++)
  {
    if(i<4)
    {
      y = i;
      x = 0;
    }
    else
    {
      y = (i-4);
      x = (b_enabled->w + 6);
    }
    if(i==AL_MAZO) x = ((b_enabled->w / 2) + 3);

    if(botones_flag[i] == ON) //Mostrar Activados
    {
      f = (SDL_Rect) {
        0, i * ((b_enabled->h)/9), b_enabled->w, (b_enabled->h)/9
      };
      d = (SDL_Rect) {
        555+x, 100 + (y*40), 0, 0
      };
      SDL_BlitSurface(b_enabled, &f, pantalla, &d);
    }
    if(botones_flag[i] == OFF) //Mostrar Desactivados
    {
      f = (SDL_Rect) {
        0, i * ((b_disabled->h)/9), b_disabled->w, (b_disabled->h)/9
      };
      d = (SDL_Rect) {
        555+x, 100 + (y*40), 0, 0
      };
      SDL_BlitSurface(b_disabled, &f, pantalla, &d);
    }
    SDL_Flip(pantalla);
  }
}

//////////////////////////// Modificar Estado de los Botones ////////////////////////
void Game::set_status(int b_truco,
                      int b_retruco,
                      int b_vale4,
                      int b_quiero,
                      int b_envido,
                      int b_real_envido,
                      int b_falta_envido,
                      int b_no_quiero,
                      int b_al_mazo)
{
  botones_flag[0] = b_truco;
  botones_flag[1] = b_retruco;
  botones_flag[2] = b_vale4;
  botones_flag[3] = b_quiero;
  botones_flag[4] = b_envido;
  botones_flag[5] = b_real_envido;
  botones_flag[6] = b_falta_envido;
  botones_flag[7] = b_no_quiero;
  botones_flag[8] = b_al_mazo;
}

/////////////////////// Calcula la menor en tantos de 3 naipes //////////////////////
int Game::menor_valor(int *naipes)
{
  int menor = valores[naipes[0]];

  if(valores[naipes[1]] < menor)
    menor = valores[naipes[1]];

  if(valores[naipes[2]] < menor)
    menor = valores[naipes[2]];

  return menor;
}

///////////////// Calcula posición de la menor en tantos de 3 naipes ////////////////
int Game::pos_menor_valor(int *naipes)
{
  int menor = valores[naipes[0]];
  int pos = 0;
  if(valores[naipes[1]] < menor)
  {
    menor = valores[naipes[1]];
    pos = 1;
  }
  if(valores[naipes[2]] < menor)
  {
    menor = valores[naipes[2]];
    pos = 2;
  }

  return pos;
}

/////////////////////// Calcula la mayor en tantos de 3 naipes //////////////////////
int Game::mayor_valor(int *naipes)
{
  int mayor = valores[naipes[0]];

  if(valores[naipes[1]] > mayor)
    mayor = valores[naipes[1]];

  if(valores[naipes[2]] > mayor)
    mayor = valores[naipes[2]];

  return mayor;
}

///////////////// Calcula posición de la menor en tantos de 3 naipes ////////////////
int Game::pos_mayor_valor(int *naipes)
{
  int mayor = valores[naipes[0]];
  int pos = 0;
  if(valores[naipes[1]] > mayor)
  {
    mayor = valores[naipes[1]];
    pos = 1;
  }
  if(valores[naipes[2]] > mayor)
  {
    mayor = valores[naipes[2]];
    pos = 2;
  }

  return pos;
}

//////////////////////////// Calcular tantos del envido ////////////////////////////
int Game::calcular_tantos(int player, int accion)
{
  int i, j, menor = -1, pos, tantos, mayor = -1;
  int palo[3];

// Inicializamos el vector palo
  for(i=0; i<3; i++)
    palo[i] = -1;

// Verificamos Jugador
/////////////////////////// Calcular tantos del Humano \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
  if(!player)
  {
    tantos = 0;
    /// Verificamos los palos ///
    for(i=0; i<3; i++)
    {
      if(p_naipes[i]<40)	{
        palo[i] = ORO;
      }
      if(p_naipes[i]<30)	{
        palo[i] = ESPADA;
      }
      if(p_naipes[i]<20)	{
        palo[i] = COPA;
      }
      if(p_naipes[i]<10)	{
        palo[i] = BASTO;
      }
    }
    /// Calculamos los tantos ///
    if(palo[0]==palo[1] && palo[0]==palo[2])  //Las tres naipes del mismo palo
    {
      // Calculamos la menor así nos quedan las dos mayores

      menor = this->menor_valor(p_naipes);
      pos = this->pos_menor_valor(p_naipes);

      // Ahora si calculamos los tantos
      for(i=0; i<3; i++)
        if(i!=pos)
        {
          tantos += valores[p_naipes[i]];
        }
      tantos += TANTO_BASE;
    }
    else
    {
      // Si alguno de los naipes es del mismo palo
      if((palo[0]==palo[1]) || (palo[0]==palo[2]) || (palo[1]==palo[2]))
      {
        pos = -1;
        // Buscamos coincidencias entre palos
        if(palo[0]==palo[1])
        {
          pos = 2;
        }
        if(palo[0]==palo[2])
        {
          pos = 1;
        }
        if(palo[1]==palo[2])
        {
          pos = 0;
        }
        // Ahora si calculamos los tantos
        if(pos!=-1)
        {
          for(i=0; i<3; i++)
            if(i!=pos)
            {
              tantos += valores[p_naipes[i]];
            }
          tantos += TANTO_BASE;
        }
      }
      else
      {
        // No hay ninguna del mismo palo
        // Calculamos la mayor y ese será nuestro Envido
        mayor = this->mayor_valor(p_naipes);
        pos = this->pos_mayor_valor(p_naipes);

        tantos += valores[p_naipes[pos]];
      }
    }
  }
/////////////////////////// Calcular tantos de la Pc //////////////////////////////
  if(player)
  {
    tantos = 0;
    /// Verificamos los palos ///
    for(i=0; i<3; i++)
    {
      if(c_naipes[i]<40)	{
        palo[i] = 3;
      }
      if(c_naipes[i]<30)	{
        palo[i] = 2;
      }
      if(c_naipes[i]<20)	{
        palo[i] = 1;
      }
      if(c_naipes[i]<10)	{
        palo[i] = 0;
      }
    }
    /// Calculamos los tantos ///
    if(palo[0]==palo[1] && palo[0]==palo[2])  //Las tres naipes del mismo palo
    {
      // Calculamos la menor así nos quedan las dos mayores
      menor = this->menor_valor(c_naipes);
      pos = this->pos_menor_valor(c_naipes);

      // Ahora si calculamos los tantos
      for(i=0; i<3; i++)
        if(i!=pos)
        {
          tantos += valores[c_naipes[i]];
        }
      tantos += TANTO_BASE;
    }
    else
    {
      // Si alguno de los naipes es del mismo palo
      if((palo[0]==palo[1]) || (palo[0]==palo[2]) || (palo[1]==palo[2]))
      {
        pos = -1;
        // Buscamos coincidencias entre palos
        if(palo[0]==palo[1])
        {
          pos = 2;
        }
        if(palo[0]==palo[2])
        {
          pos = 1;
        }
        if(palo[1]==palo[2])
        {
          pos = 0;
        }
        // Ahora si calculamos los tantos
        if(pos!=-1)
        {
          for(i=0; i<3; i++)
            if(i!=pos)
            {
              tantos += valores[c_naipes[i]];
            }
          tantos += TANTO_BASE;
        }
      }
      else
      {
        // Calculamos la mayor y ese será nuestro Envido
        mayor = this->mayor_valor(c_naipes);
        pos = this->pos_mayor_valor(c_naipes);

        tantos += valores[c_naipes[pos]];
      }
    }
  }
  if(accion)
    return pos;
  else
    return tantos;
}

//////////////////////////////// Nueva Partida //////////////////////////////////////
void Game::nuevo(TTF_Font *font, Opciones opt)
{
  int jugada_player, jugada_pc, i, j, mano, faltas, gana = -1, turno, naipe_pc, naipe_player;
  int puntos_truco_pc, puntos_envido_pc, puntos_truco_player, puntos_envido_player;
  int jugadas_player, jugadas_pc, parda, primera, segunda, canto_player, canto_pc;
  int puntos_pc = 0, puntos_pl = 0, fin_de_mano, fin_de_juego = 0, accion_player, accion_pc;
  int tantos_pc, tantos_reales_player, tantos_cantados_player, ganador_envido;
  int truco, retruco, vale4, envido, real, falta, envido_querido, pos_env, tantos_mostrados;
  int pendiente, canto_truco, tercera, segunda_1, orden_jugada_pc[3], canto_al_4, truco_querido;
  char msj_tantos[3];

  Jugador jugador(cx1, cy1, cx2, cy2, dist);
  Computer compu(fondo, pantalla);
  Carta carta;
  Texto txt;

  mano = (1 - opt.getMano());
  faltas = opt.getFaltas();

  while(!fin_de_juego)
  {
    // Inicializar variables del juego //
    for(i=0; i<3; i++)
      orden_jugada_pc[i] = -1;
    jugada_player = jugada_pc = jugadas_player = jugadas_pc = 0;
    carta_player[0] = carta_player[1] = carta_player[2] = 0;
    carta_pc[0] = carta_pc[1] = carta_pc[2] = 0;
    msj_tantos[0] = msj_tantos[1] = msj_tantos[2] = '0';
    puntos_truco_pc = puntos_envido_pc = puntos_truco_player = puntos_envido_player = 0;
    accion_player = accion_pc = fin_de_mano = 0;
    tantos_pc = tantos_cantados_player = tantos_reales_player = 0;
    canto_al_4 = 0; // 0: No se cantó    1: Se cantó
    truco = retruco = vale4 = falta = 0; // 0: No se cantó    1: Se cantó
    canto_truco = -1; // Quien cantó Truco?    0: Humano      1: Pc
    envido = real = 0; // 0: No se cantó    1: Se cantó una vez     2: Se cantó dos veces    3: Etc.
    envido_querido = -1; // 0: No se aceptó    1: Se aceptó
    truco_querido = -1; // 0: No se aceptó    1: Se aceptó
    parda = 0;  // 0: No hay parda    1: Hay parda
    primera = -1; // 0: Primera del humano    1: Tengo primera yo     2: Parda
    segunda = -1; // Idem Primera
    segunda_1 = tercera = -1; // Quien jugó primero en tercera/segunda?   0: Humano      1: Pc
    canto_player = -1; // Si Humano cantó Truco, Envido, etc; (Valores del botones_flag[9])
    canto_pc = -1; // Si Pc cantó Truco, Envido, etc; (Valores del botones_flag[9])
    tantos_mostrados = 0; // Flag
    ganador_envido = -1; // 0: Gana Humano       1: Gana Pc
    pendiente = 0; // Humano canta Truco y Pc retruca "Envido". Queda pendiente si Pc acepta o no el Truco.
    // Decidir quien juega primero //
    if(!mano)
    {
      turno = HUMANO;
      this->mostrar_msj(0, font);
    }
    else
    {
      turno = PC;
      this->mostrar_msj(1, font);
    }
    // Estado de los botones //
    this->set_status(TRUCO_ON,
                     RETRUCO_OFF,
                     VALE4_OFF,
                     QUIERO_OFF,
                     ENVIDO_ON,
                     REAL_ENVIDO_ON,
                     FALTA_ENVIDO_ON,
                     NO_QUIERO_OFF,
                     AL_MAZO_ON);
    // Inicializar baraja //
    mazo.barajar();
    mazo.repartir(c_naipes, p_naipes, 3);
    carta.mostrar_servidas(fondo, pantalla, p_naipes);
    this->mostrar_puntaje(puntos_pc,puntos_pl,font);
    // Calcular tantos reales de cada jugador //
    tantos_pc = this->calcular_tantos(1,0);
    tantos_reales_player = this->calcular_tantos(0,0);
    //////////////////////////////// Mano a jugar /////////////////////////////////////
    while(!fin_de_mano)
    {
      this->mostrar_botones();

      if(!turno && !fin_de_mano) /////////////////// Juega el humano ////////////////////
      {
        // Seleccionar acción de juego del Humano //
        if(canto_pc!=3 && canto_pc!=7 && canto_pc!=-1)
          accion_player = SOLO_PRESIONAR_BOTON;
        else
          accion_player = JUGAR_NAIPE_PRESIONAR_BOTON;

        jugada_player = jugador.jugar(fondo,
                                      pantalla,
                                      p_naipes,
                                      carta,
                                      carta_player,
                                      &canto_player,
                                      botones_flag,
                                      accion_player);
        turno = PC;

        gana = -1;
        if(jugada_player!=3 && jugada_player!=-1)
        {
          jugadas_player++;
          naipe_player = jugada_player;
          if((jugadas_pc == jugadas_player) && (jugadas_pc!=0))
            gana = this->comparar(naipe_pc, naipe_player, canto_al_4, font);
        }

        switch(canto_pc)
        {
        case TRUCO:
          canto_pc = RTA_TRUCO;
          break;
        case ENVIDO:
          canto_pc = RTA_ENVIDO;
          break;
        case REAL_ENVIDO:
          canto_pc = RTA_REAL_ENVIDO;
          break;
        case FALTA_ENVIDO:
          canto_pc = RTA_FALTA_ENVIDO;
          break;
        case RETRUCO:
          canto_pc = RTA_RETRUCO;
          break;
        case VALE4:
          canto_pc = RTA_VALE4;
          break;
        default:
          canto_pc = RTA_OFF;
          break;
        }
      }
      else /////////////////// Juega Pc ////////////////////
      {
        if( !fin_de_mano)
        {
          // Seleccionar acción de juego de la Pc //
          if(jugadas_player == jugadas_pc)
            accion_pc = JUGAR_SIN_VERIFICAR_NAIPE; // Naipe humano
          else
            accion_pc = JUGAR_VERIFICANDO_NAIPE; // Naipe humano

          //Humano cantó Truco y Pc que el Envido está primero //
          if(pendiente)
          {
            canto_player = 0;
            pendiente = 0;
          }

          jugada_pc = compu.jugar(c_naipes,
                                  p_naipes,
                                  jugada_player,
                                  carta,
                                  jerarquia,
                                  carta_pc,
                                  parda,
                                  canto_player,
                                  &canto_pc,
                                  accion_pc,
                                  tantos_pc,
                                  (envido + real + falta),
                                  &pendiente,
                                  primera,
                                  segunda_1,
                                  tercera,
                                  truco,
                                  puntos_pl,
                                  puntos_pc,
                                  falta,
                                  canto_truco,
                                  orden_jugada_pc,
                                  &canto_al_4,
                                  segunda);
          turno = HUMANO;

          gana = -1;
          if(jugada_pc!=3)
          {
            jugadas_pc++;
            naipe_pc = jugada_pc;

            for(i=0; i<3; i++)
              if(orden_jugada_pc[i] == -1)
              {
                orden_jugada_pc[i] = naipe_pc;
                break;
              }

            if(jugadas_pc==jugadas_player && jugadas_pc!=0)
              gana = this->comparar(naipe_pc, naipe_player, canto_al_4, font);
          }

          switch(canto_player)
          {
          case TRUCO:
            canto_player = RTA_TRUCO;
            break;
          case ENVIDO:
            canto_player = RTA_ENVIDO;
            break;
          case REAL_ENVIDO:
            canto_player = RTA_REAL_ENVIDO;
            break;
          case FALTA_ENVIDO:
            canto_player = RTA_FALTA_ENVIDO;
            break;
          case RETRUCO:
            canto_player = RTA_RETRUCO;
            break;
          case VALE4:
            canto_player = RTA_VALE4;
            break;
          default:
            canto_player = RTA_OFF;
            break;
          }
        }
      }

      // Quien juega primero en tercera //
      if(jugadas_player == 3)
        if(tercera == -1)
          tercera = HUMANO;
      if(jugadas_pc == 3)
        if(tercera == -1)
          tercera = PC;

      // Quien juega primero en segunda //
      if(jugadas_player == 2)
        if(segunda_1 == -1)
          segunda_1 = HUMANO;
      if(jugadas_pc == 2)
        if(segunda_1 == -1)
          segunda_1 = PC;

      // Si pasó la primera jugada de ambos //
      if((jugadas_player == 1) && (jugadas_pc == 1) && (truco == NO_CANTADO)) {
        // Desactivamos el Envido
        this->set_status(TRUCO_ON,
                         RETRUCO_OFF,
                         VALE4_OFF,
                         QUIERO_OFF,
                         ENVIDO_OFF,
                         REAL_ENVIDO_OFF,
                         FALTA_ENVIDO_OFF,
                         NO_QUIERO_OFF,
                         AL_MAZO_ON);
      }
      //////////////////////////// Verificar si se cantó algo /////////////////////////////
      if(canto_player>-1 && canto_player<9)
      {
        this->mostrar_msj(canto_player+20, font);
        if((canto_player != 7) && (canto_pc != 9) && (canto_pc != 13) && (canto_pc != 14))
          esperar(500);
        switch(canto_player)
        {
        case TRUCO:
          this->set_status(TRUCO_OFF,
                           RETRUCO_OFF,
                           VALE4_OFF,
                           QUIERO_OFF,
                           ENVIDO_OFF,
                           REAL_ENVIDO_OFF,
                           FALTA_ENVIDO_OFF,
                           NO_QUIERO_OFF,
                           AL_MAZO_ON);

          truco = CANTADO;

          if(canto_truco == -1)
            canto_truco = HUMANO;
          break;
        case RETRUCO:
          // Estado de los botones //
          this->set_status(TRUCO_OFF,
                           RETRUCO_OFF,
                           VALE4_OFF,
                           QUIERO_OFF,
                           ENVIDO_OFF,
                           REAL_ENVIDO_OFF,
                           FALTA_ENVIDO_OFF,
                           NO_QUIERO_OFF,
                           AL_MAZO_ON);
          retruco = CANTADO;
          break;
        case VALE4:
          // Estado de los botones //
          this->set_status(TRUCO_OFF,
                           RETRUCO_OFF,
                           VALE4_OFF,
                           QUIERO_OFF,
                           ENVIDO_OFF,
                           REAL_ENVIDO_OFF,
                           FALTA_ENVIDO_OFF,
                           NO_QUIERO_OFF,
                           AL_MAZO_ON);
          vale4 = CANTADO;
          break;
        case QUIERO:
          if((canto_pc == RTA_ENVIDO) ||
              (canto_pc == RTA_REAL_ENVIDO) ||
              (canto_pc == RTA_FALTA_ENVIDO))
          {
            envido_querido = ACEPTADO;
            // Activamos el Truco
            this->set_status(TRUCO_ON,
                             RETRUCO_OFF,
                             VALE4_OFF,
                             QUIERO_OFF,
                             ENVIDO_OFF,
                             REAL_ENVIDO_OFF,
                             FALTA_ENVIDO_OFF,
                             NO_QUIERO_OFF,
                             AL_MAZO_ON);
          }
          if(canto_pc == RTA_VALE4) {
            this->set_status(TRUCO_OFF,
                             RETRUCO_OFF,
                             VALE4_OFF,
                             QUIERO_OFF,
                             ENVIDO_OFF,
                             REAL_ENVIDO_OFF,
                             FALTA_ENVIDO_OFF,
                             NO_QUIERO_OFF,
                             AL_MAZO_ON);
          }
          if(canto_pc == RTA_RETRUCO) {
            this->set_status(TRUCO_OFF,
                             RETRUCO_OFF,
                             VALE4_ON,
                             QUIERO_OFF,
                             ENVIDO_OFF,
                             REAL_ENVIDO_OFF,
                             FALTA_ENVIDO_OFF,
                             NO_QUIERO_OFF,
                             AL_MAZO_ON);
          }
          if(canto_pc == RTA_TRUCO)
          {
            this->set_status(TRUCO_OFF,
                             RETRUCO_ON,
                             VALE4_OFF,
                             QUIERO_OFF,
                             ENVIDO_OFF,
                             REAL_ENVIDO_OFF,
                             FALTA_ENVIDO_OFF,
                             NO_QUIERO_OFF,
                             AL_MAZO_ON);

            truco_querido = ACEPTADO;
          }
          if(!pendiente)
          {
            if((jugadas_player == 0) && (jugadas_pc == 1))
              turno = HUMANO;
            if((jugadas_player == 1) && (jugadas_pc == 2))
              turno = HUMANO;
            if((jugadas_player == 2) && (jugadas_pc == 3))
              turno = HUMANO;
            if((jugadas_pc == 1) && (jugadas_player == 1))
              if(primera == DE_HUMANO)
                turno = HUMANO;
            if((jugadas_pc == 2) && (jugadas_player == 2))
              if(segunda == DE_HUMANO)
                turno = HUMANO;
            if((jugadas_player == 0) && (jugadas_pc == 0) && (!mano))
              turno = HUMANO;
          }
          break;
        case ENVIDO:
          this->set_status(TRUCO_ON,
                           RETRUCO_OFF,
                           VALE4_OFF,
                           QUIERO_OFF,
                           ENVIDO_OFF,
                           REAL_ENVIDO_OFF,
                           FALTA_ENVIDO_OFF,
                           NO_QUIERO_OFF,
                           AL_MAZO_ON);
          envido++;
          break;
        case REAL_ENVIDO:
          this->set_status(TRUCO_ON,
                           RETRUCO_OFF,
                           VALE4_OFF,
                           QUIERO_OFF,
                           ENVIDO_OFF,
                           REAL_ENVIDO_OFF,
                           FALTA_ENVIDO_OFF,
                           NO_QUIERO_OFF,
                           AL_MAZO_ON);
          real++;
          break;
        case FALTA_ENVIDO:
          this->set_status(TRUCO_ON,
                           RETRUCO_OFF,
                           VALE4_OFF,
                           QUIERO_OFF,
                           ENVIDO_OFF,
                           REAL_ENVIDO_OFF,
                           FALTA_ENVIDO_OFF,
                           NO_QUIERO_OFF,
                           AL_MAZO_ON);
          falta = CANTADO;
          break;
        case NO_QUIERO:
          if((canto_pc == RTA_TRUCO) ||
              (canto_pc == RTA_RETRUCO) ||
              (canto_pc == RTA_VALE4))
          {
            fin_de_mano = 1;
            if(truco == CANTADO)
              puntos_truco_pc ++;
            if(retruco == CANTADO)
              puntos_truco_pc ++;
            if(vale4 == CANTADO)
              puntos_truco_pc ++;
          }
          else
          {
            if(falta == CANTADO)
            {
              if(!envido && !real)
                puntos_envido_pc ++;
              else
                puntos_envido_pc += ((2*envido) + (3*real));
            }
            else
            {
              if(envido > 0)
                puntos_envido_pc += envido;
              if(real > 0)
                puntos_envido_pc += real;
            }
            this->set_status(TRUCO_ON,
                             RETRUCO_OFF,
                             VALE4_OFF,
                             QUIERO_OFF,
                             ENVIDO_OFF,
                             REAL_ENVIDO_OFF,
                             FALTA_ENVIDO_OFF,
                             NO_QUIERO_OFF,
                             AL_MAZO_ON);

            envido_querido = RECHAZADO;
          }
          if(!pendiente)
          {
            if((jugadas_player == 0) && (jugadas_pc == 1))
              turno = HUMANO;
            if((jugadas_player == 1) && (jugadas_pc == 2))
              turno = HUMANO;
            if((jugadas_player == 2) && (jugadas_pc == 3))
              turno = HUMANO;
            if((jugadas_pc == 1) && (jugadas_player == 1))
              if(primera == DE_HUMANO)
                turno = HUMANO;
            if((jugadas_pc == 2) && (jugadas_player == 2))
              if(segunda == DE_HUMANO)
                turno = HUMANO;
            if((jugadas_player == 0) && (jugadas_pc == 0) && (!mano))
              turno = HUMANO;
          }
          break;
        case AL_MAZO:
          puntos_truco_pc ++;
          if((envido_querido == -1) && (jugadas_player <= 1) && (jugadas_pc == 0) && (truco_querido == -1))
            puntos_truco_pc ++;
          if(truco_querido == ACEPTADO)
            puntos_truco_pc ++;
          //if((envido_querido == 0) && (ganador_envido == 1))
          //puntos_truco_pc ++;
          fin_de_mano = 1;
          break;
        default:
          break;
        }

        if(((canto_pc == RTA_ENVIDO) ||
            (canto_pc == RTA_REAL_ENVIDO) ||
            (canto_pc == RTA_FALTA_ENVIDO)) && (canto_player == QUIERO))
        {
          // Canta primero los tantos el que es mano
          if(!mano)
          {
            this->mostrar_msj(-1, font);
            tantos_cantados_player = jugador.ingresar_tantos(fondo, pantalla, font, 0);
            /// Verificar quien ganó el Envido ///
            if((tantos_cantados_player > tantos_pc) || ((tantos_cantados_player == tantos_pc) && (!mano)))
            {
              ganador_envido = HUMANO;
              this->mostrar_msj(-2, font);
            }
            if((tantos_cantados_player < tantos_pc) || ((tantos_cantados_player == tantos_pc) && (mano)))
            {
              ganador_envido = PC;
              this->limpiar_texto();
              txt.mostrar(450, 350, pantalla, "Los míos...", 0, 55, 255, 200, font);
              txt.mostrar(590, 350, pantalla, itoa(tantos_pc,msj_tantos,10), 0, 55, 255, 200, font);
              esperar(1000);
            }
          }
          else
          {
            this->limpiar_texto();
            txt.mostrar(450, 350, pantalla, "Los míos...", 0, 55, 255, 200, font);
            txt.mostrar(590, 350, pantalla, itoa(tantos_pc,msj_tantos,10), 0, 55, 255, 200, font);
            esperar(1000);
            this->mostrar_msj(-1, font);
            tantos_cantados_player = jugador.ingresar_tantos(fondo, pantalla, font, 1);
            if(tantos_cantados_player == -1)
            {
              this->mostrar_msj(-5, font);
              esperar(800);
              ganador_envido = PC;
            }
            else
            {
              /// Verificar quien ganó el Envido ///
              if((tantos_cantados_player > tantos_pc) || ((tantos_cantados_player == tantos_pc) && (!mano)))
                ganador_envido = HUMANO;
              if((tantos_cantados_player < tantos_pc) || ((tantos_cantados_player == tantos_pc) && (mano)))
                ganador_envido = PC;
            }
          }
          /// Sumar puntos al ganador ///
          if(ganador_envido == HUMANO)
          {
            switch(canto_pc)
            {
            case RTA_ENVIDO:
              puntos_envido_player += 2*envido;
              break;
            case RTA_REAL_ENVIDO:
              if(envido > 0)
                puntos_envido_player += 2*envido;
              puntos_envido_player += 3*real;
              break;
            case RTA_FALTA_ENVIDO:
              if(faltas == 1)
              {
                if(puntos_pc > puntos_pl)
                  puntos_envido_player = 30 - puntos_pc;
                else
                  puntos_envido_player = 30 - puntos_pl;
              }
              else if(faltas == 2)
              {
                if((puntos_pc >= 15) || (puntos_pl >= 15))
                {
                  if(puntos_pc > puntos_pl)
                    puntos_envido_player = 30 - puntos_pc;
                  else
                    puntos_envido_player = 30 - puntos_pl;
                }
                else
                {
                  if(puntos_pc > puntos_pl)
                    puntos_envido_player = 15 - puntos_pc;
                  else
                    puntos_envido_player = 15 - puntos_pl;
                }
              }
              break;
            }
          }
          else
          {
            switch(canto_pc)
            {
            case RTA_ENVIDO:
              puntos_envido_pc += 2*envido;
              break;
            case RTA_REAL_ENVIDO:
              if(envido > 0)
                puntos_envido_pc += 2*envido;
              puntos_envido_pc += 3*real;
              break;
            case RTA_FALTA_ENVIDO:
              if(faltas == 1)
              {
                if(puntos_pc > puntos_pl)
                  puntos_envido_pc = 30 - puntos_pc;
                else
                  puntos_envido_pc = 30 - puntos_pl;
              }
              if(faltas == 2)
              {
                if((puntos_pc >= 15) || (puntos_pl >= 15))
                {
                  if(puntos_pc > puntos_pl)
                    puntos_envido_pc = 30 - puntos_pc;
                  else
                    puntos_envido_pc = 30 - puntos_pl;
                }
                else
                {
                  if(puntos_pc > puntos_pl)
                    puntos_envido_pc = 15 - puntos_pc;
                  else
                    puntos_envido_pc = 15 - puntos_pl;
                }
              }
              break;
            }
          }
        }
        if(turno == 0)
          canto_player = RTA_OFF;
        canto_pc = RTA_OFF;
      }
      if((canto_pc>-1) && (canto_pc<9))
      {
        this->mostrar_msj(canto_pc+30, font);
        esperar(500);
        switch(canto_pc)
        {
        case TRUCO:
          this->set_status(TRUCO_OFF,
                           RETRUCO_ON,
                           VALE4_OFF,
                           QUIERO_ON,
                           ENVIDO_OFF,
                           REAL_ENVIDO_OFF,
                           FALTA_ENVIDO_OFF,
                           NO_QUIERO_ON,
                           AL_MAZO_ON);

          truco = CANTADO;

          if(canto_truco == -1)
            canto_truco = PC;
          break;
        case RETRUCO:
          this->set_status(0,0,1,1,0,0,0,1,1);
          this->set_status(TRUCO_OFF,
                           RETRUCO_OFF,
                           VALE4_ON,
                           QUIERO_ON,
                           ENVIDO_OFF,
                           REAL_ENVIDO_OFF,
                           FALTA_ENVIDO_OFF,
                           NO_QUIERO_ON,
                           AL_MAZO_ON);
          retruco = CANTADO;
          break;
        case VALE4:
          this->set_status(TRUCO_OFF,
                           RETRUCO_OFF,
                           VALE4_OFF,
                           QUIERO_ON,
                           ENVIDO_OFF,
                           REAL_ENVIDO_OFF,
                           FALTA_ENVIDO_OFF,
                           NO_QUIERO_ON,
                           AL_MAZO_ON);
          vale4 = CANTADO;
          break;
        case QUIERO:
          if((canto_player == RTA_TRUCO) ||
              (canto_player == RTA_RETRUCO) ||
              (canto_player == RTA_VALE4))
          {
            this->set_status(TRUCO_OFF,
                             RETRUCO_OFF,
                             VALE4_OFF,
                             QUIERO_OFF,
                             ENVIDO_OFF,
                             REAL_ENVIDO_OFF,
                             FALTA_ENVIDO_OFF,
                             NO_QUIERO_OFF,
                             AL_MAZO_ON);

            truco_querido = ACEPTADO;
          }
          if((canto_player == RTA_ENVIDO) ||
              (canto_player == RTA_REAL_ENVIDO) ||
              (canto_player == RTA_FALTA_ENVIDO))
          {
            envido_querido = ACEPTADO;
            // Activamos el Truco
            this->set_status(TRUCO_ON,
                             RETRUCO_OFF,
                             VALE4_OFF,
                             QUIERO_OFF,
                             ENVIDO_OFF,
                             REAL_ENVIDO_OFF,
                             FALTA_ENVIDO_OFF,
                             NO_QUIERO_OFF,
                             AL_MAZO_ON);
          }
          if(!pendiente)
          {
            if((jugadas_pc == 0) && (jugadas_player == 1))
              turno = PC;
            if((jugadas_pc == 1) && (jugadas_player == 2))
              turno = PC;
            if((jugadas_pc == 2) && (jugadas_player == 3))
              turno = PC;
            if((jugadas_pc == 1) && (jugadas_player == 1))
              if(primera == DE_PC)
                turno = PC;
            if((jugadas_pc == 2) && (jugadas_player == 2))
              if(segunda == DE_PC)
                turno = PC;
            if((jugadas_pc == 0) && (jugadas_player == 0) && (mano))
              turno = PC;
          }
          break;
        case ENVIDO:
          this->set_status(TRUCO_OFF,
                           RETRUCO_OFF,
                           VALE4_OFF,
                           QUIERO_ON,
                           ENVIDO_ON,
                           REAL_ENVIDO_ON,
                           FALTA_ENVIDO_ON,
                           NO_QUIERO_ON,
                           AL_MAZO_ON);
          envido++;
          break;
        case REAL_ENVIDO:
          this->set_status(TRUCO_OFF,
                           RETRUCO_OFF,
                           VALE4_OFF,
                           QUIERO_ON,
                           ENVIDO_OFF,
                           REAL_ENVIDO_ON,
                           FALTA_ENVIDO_ON,
                           NO_QUIERO_ON,
                           AL_MAZO_ON);
          real++;
          break;
        case FALTA_ENVIDO:
          this->set_status(TRUCO_OFF,
                           RETRUCO_OFF,
                           VALE4_OFF,
                           QUIERO_ON,
                           ENVIDO_OFF,
                           REAL_ENVIDO_OFF,
                           FALTA_ENVIDO_OFF,
                           NO_QUIERO_ON,
                           AL_MAZO_ON);
          falta = CANTADO;
          break;
        case NO_QUIERO:
          if((canto_player == RTA_TRUCO) ||
              (canto_player == RTA_RETRUCO) ||
              (canto_player == RTA_VALE4))
          {
            fin_de_mano = 1;
            gana = -1;
            if(truco == CANTADO)
              puntos_truco_player ++;
            if(retruco == CANTADO)
              puntos_truco_player ++;
            if(vale4 == CANTADO)
              puntos_truco_player ++;
            this->mostrar_msj(3, font);
          }
          else
          {
            if(falta == CANTADO)
            {
              if(!envido && !real)
                puntos_envido_player ++;
              else
                puntos_envido_player += ((2*envido) + (3*real));
            }
            else
            {
              if(envido > 0)
                puntos_envido_player += envido;
              if(real > 0)
                puntos_envido_player += real;
            }
            this->set_status(TRUCO_ON,
                             RETRUCO_OFF,
                             VALE4_OFF,
                             QUIERO_OFF,
                             ENVIDO_OFF,
                             REAL_ENVIDO_OFF,
                             FALTA_ENVIDO_OFF,
                             NO_QUIERO_OFF,
                             AL_MAZO_ON);

            envido_querido = RECHAZADO;
          }
          if((!pendiente) && (!fin_de_mano))
          {
            if((jugadas_pc == 0) && (jugadas_player == 1))
              turno = PC;
            if((jugadas_pc == 1) && (jugadas_player == 2))
              turno = PC;
            if((jugadas_pc == 2) && (jugadas_player == 3))
              turno = PC;
            if((jugadas_pc == 1) && (jugadas_player == 1))
              if(primera == DE_PC)
                turno = PC;
            if((jugadas_pc == 2) && (jugadas_player == 2))
              if(segunda == DE_PC)
                turno = PC;
            if((jugadas_pc == 0) && (jugadas_player == 0) && (mano))
              turno = PC;
          }
          canto_pc = RTA_OFF;
          break;
        case AL_MAZO:
          if((envido_querido == -1) && (jugadas_player <= 1) && (jugadas_pc == 0) && (truco_querido == -1))
            puntos_truco_player += 2;
          if(truco_querido == ACEPTADO)
            puntos_truco_player += 2;
          if((envido_querido == RECHAZADO) && (ganador_envido == HUMANO))
            puntos_truco_player ++;
          fin_de_mano = 1;
          break;
        }

        if(((canto_player == RTA_ENVIDO) ||
            (canto_player == RTA_REAL_ENVIDO) ||
            (canto_player == RTA_FALTA_ENVIDO)) && canto_pc == QUIERO)
        {
          // Canta primero los tantos el que es mano
          if(!mano)
          {
            this->mostrar_msj(-1, font);
            tantos_cantados_player = jugador.ingresar_tantos(fondo, pantalla, font, 0);
            /// Verificar quien ganó el Envido ///
            if((tantos_cantados_player > tantos_pc) || ((tantos_cantados_player == tantos_pc) && (!mano)))
            {
              ganador_envido = HUMANO;
              this->mostrar_msj(-2, font);
            }
            if((tantos_cantados_player < tantos_pc) || ((tantos_cantados_player == tantos_pc) && (mano)))
            {
              ganador_envido = PC;
              this->limpiar_texto();
              txt.mostrar(450, 350, pantalla, "Los míos...", 0, 55, 255, 200, font);
              txt.mostrar(590, 350, pantalla, itoa(tantos_pc,msj_tantos,10), 0, 55, 255, 200, font);
              esperar(1000);
            }
          }
          else
          {
            this->limpiar_texto();
            txt.mostrar(450, 350, pantalla, "Los míos...", 0, 55, 255, 200, font);
            txt.mostrar(590, 350, pantalla, itoa(tantos_pc,msj_tantos,10), 0, 55, 255, 200, font);
            esperar(1000);
            this->mostrar_msj(-1, font);
            tantos_cantados_player = jugador.ingresar_tantos(fondo, pantalla, font, 1);
            if(tantos_cantados_player == -1) // Son buenas
            {
              this->mostrar_msj(-5, font);
              esperar(800);
              ganador_envido = PC;
            }
            else
            {
              /// Verificar quien ganó el Envido ///
              if((tantos_cantados_player > tantos_pc) || ((tantos_cantados_player == tantos_pc) && (!mano)))
                ganador_envido = HUMANO;
              if((tantos_cantados_player < tantos_pc) || ((tantos_cantados_player == tantos_pc) && (mano)))
                ganador_envido = PC;
            }
          }
          /// Sumar puntos al ganador ///
          if(ganador_envido == HUMANO)
          {
            switch(canto_player)
            {
            case RTA_ENVIDO:
              puntos_envido_player += 2*envido;
              break;
            case RTA_REAL_ENVIDO:
              if(envido > 0)
                puntos_envido_player += 2*envido;
              puntos_envido_player += 3*real;
              break;
            case RTA_FALTA_ENVIDO:
              if(faltas == 1)
              {
                if(puntos_pc > puntos_pl)
                  puntos_envido_player = 30 - puntos_pc;
                else
                  puntos_envido_player = 30 - puntos_pl;
              }
              else if(faltas == 2)
              {
                if((puntos_pc >= 15) || (puntos_pl >= 15))
                {
                  if(puntos_pc > puntos_pl)
                    puntos_envido_player = 30 - puntos_pc;
                  else
                    puntos_envido_player = 30 - puntos_pl;
                }
                else
                {
                  if(puntos_pc > puntos_pl)
                    puntos_envido_player = 15 - puntos_pc;
                  else
                    puntos_envido_player = 15 - puntos_pl;
                }
              }
              break;
            }
          }
          else
          {
            switch(canto_player)
            {
            case RTA_ENVIDO:
              puntos_envido_pc += 2*envido;
              break;
            case RTA_REAL_ENVIDO:
              if(envido > 0)
                puntos_envido_pc += 2*envido;
              puntos_envido_pc += 3*real;
              break;
            case RTA_FALTA_ENVIDO:
              if(faltas == 1)
              {
                if(puntos_pc > puntos_pl)
                  puntos_envido_pc = 30 - puntos_pc;
                else
                  puntos_envido_pc = 30 - puntos_pl;
              }
              if(faltas == 2)
              {
                if((puntos_pc >= 15) || (puntos_pl >= 15))
                {
                  if(puntos_pc > puntos_pl)
                    puntos_envido_pc = 30 - puntos_pc;
                  else
                    puntos_envido_pc = 30 - puntos_pl;
                }
                else
                {
                  if(puntos_pc > puntos_pl)
                    puntos_envido_pc = 15 - puntos_pc;
                  else
                    puntos_envido_pc = 15 - puntos_pl;
                }
              }
              break;
            }
          }
        }
        if(turno == 1)
          canto_pc = RTA_OFF;
        canto_player = RTA_OFF;
      }
      if(jugadas_player != jugadas_pc)
        gana = -1;
      //////////////////// Comprobar quien ganó la jugada si es que hubo //////////////////
      if(gana!=-1)
      {
        switch(gana)
        {
        ///////////////// Gana el humano ///////////////////
        case 2:
          //Cantidad de jugadas
          switch(jugadas_player)
          {
          case 1:
            primera = DE_HUMANO;
            turno = HUMANO;
            break;
          case 2:
            if((primera == DE_HUMANO) || (primera == PARDA))
            {
              if(truco == CANTADO)
                puntos_truco_player += 2;
              else
                puntos_truco_player ++;
              if(retruco == CANTADO)
                puntos_truco_player ++;
              if(vale4 == CANTADO)
                puntos_truco_player ++;
              fin_de_mano = 1;
              this->mostrar_msj(2, font);
            }
            else
            {
              segunda = DE_HUMANO;
              turno = HUMANO;
            }
            break;
          case 3:
            if((primera == DE_HUMANO) || (segunda == DE_HUMANO))
              this->mostrar_msj(3, font);
            if((primera == PARDA) && (segunda == PARDA))
              this->mostrar_msj(4, font);
            if(truco == CANTADO)
              puntos_truco_player += 2;
            else
              puntos_truco_player ++;
            if(retruco == CANTADO)
              puntos_truco_player ++;
            if(vale4 == CANTADO)
              puntos_truco_player ++;
            fin_de_mano = 1;
            break;
          }
          break;
        //////////////////// Gana Pc /////////////////////
        case 1:
          switch(jugadas_player)
          {
          case 1:
            primera = DE_PC;
            turno = PC;
            break;
          case 2:
            if((primera == DE_PC) || (primera == PARDA))
            {
              if(truco == CANTADO)
                puntos_truco_pc += 2;
              else
                puntos_truco_pc ++;
              if(retruco == CANTADO)
                puntos_truco_pc ++;
              if(vale4 == CANTADO)
                puntos_truco_pc ++;
              fin_de_mano = 1;
              this->mostrar_msj(11, font);
            }
            else
            {
              segunda = DE_PC;
              turno = PC;
            }
            break;
          case 3:
            if((primera == DE_PC) || (segunda == DE_PC))
              this->mostrar_msj(6, font);
            if((primera == PARDA) && (segunda == PARDA))
              this->mostrar_msj(7, font);
            if(truco == CANTADO)
              puntos_truco_pc += 2;
            else
              puntos_truco_pc ++;
            if(retruco == CANTADO)
              puntos_truco_pc ++;
            if(vale4 == CANTADO)
              puntos_truco_pc ++;
            fin_de_mano = 1;
            break;
          }
          break;
        //////////////////// Hay parda ///////////////////
        case 0:
          parda = 1;
          switch(jugadas_pc)
          {
          case 1:
            primera = PARDA;
            // Sigue jugando el que es mano //
            if(!mano)
              turno = HUMANO;
            else
              turno = PC;
            break;
          case 2:
            if(primera == PARDA)
            {
              segunda = PARDA;
              // Sigue jugando el que es mano //
              if(!mano)
                turno = HUMANO;
              else
                turno = PC;
              break;
            }
            if(primera == DE_PC)
            {
              this->mostrar_msj(8, font);
              fin_de_mano = 1;
              if(truco == CANTADO)
                puntos_truco_pc += 2;
              else
                puntos_truco_pc ++;
              if(retruco == CANTADO)
                puntos_truco_pc ++;
              if(vale4 == CANTADO)
                puntos_truco_pc ++;
            }
            if(primera == DE_HUMANO)
            {
              this->mostrar_msj(3, font);
              fin_de_mano = 1;
              if(truco == CANTADO)
                puntos_truco_player += 2;
              else
                puntos_truco_player ++;
              if(retruco == CANTADO)
                puntos_truco_player ++;
              if(vale4 == CANTADO)
                puntos_truco_player ++;
            }
            break;
          case 3:
            if(segunda == PARDA) // Gana el que es mano
            {
              if(mano)
              {
                this->mostrar_msj(9, font);
                if(truco == CANTADO)
                  puntos_truco_pc += 2;
                else
                  puntos_truco_pc ++;
                if(retruco == CANTADO)
                  puntos_truco_pc ++;
                if(vale4 == CANTADO)
                  puntos_truco_pc ++;
              }
              else
              {
                this->mostrar_msj(10, font);
                if(truco == CANTADO)
                  puntos_truco_player += 2;
                else
                  puntos_truco_player ++;
                if(retruco == CANTADO)
                  puntos_truco_player ++;
                if(vale4 == CANTADO)
                  puntos_truco_player ++;
              }
            }
            if(primera == DE_PC)
            {
              if(truco == CANTADO)
                puntos_truco_pc += 2;
              else
                puntos_truco_pc ++;
              if(retruco == CANTADO)
                puntos_truco_pc ++;
              if(vale4 == CANTADO)
                puntos_truco_pc ++;
              this->mostrar_msj(5, font);
            }
            if(primera == DE_HUMANO)
            {
              if(truco == CANTADO)
                puntos_truco_player += 2;
              else
                puntos_truco_player ++;
              if(retruco == CANTADO)
                puntos_truco_player ++;
              if(vale4 == CANTADO)
                puntos_truco_player ++;
              this->mostrar_msj(4, font);
            }
            fin_de_mano = 1;
            break;
          }
          break;
        }
      }
      // Si finalizó la mano //
      if((fin_de_mano == 1) || ((jugadas_player == 3) && (jugadas_pc == 3)))
      {
        if(canto_player == AL_MAZO)
          esperar(1000);
        else
          esperar(1500);
        fin_de_mano = 1;
      }
      // Limpiar tantos ingresados después de la pausa //
      jugador.limpiar_texto(fondo, pantalla, 463);
      jugador.limpiar_texto(fondo, pantalla, 463+18);
      // Si se presionó el botón menú //
      if(jugada_player == MENU)
        fin_de_mano = 1;
      // Verificar si el juego ha terminado //
      if((puntos_pc + puntos_truco_pc + puntos_envido_pc) >=30)
        fin_de_mano = 1;
      if((puntos_pl + puntos_truco_player + puntos_envido_player) >=30)
        fin_de_mano = 1;
    }
    // Verificar que los tantos cantados por el humano sean reales //
    if((tantos_cantados_player != tantos_reales_player) && (jugada_player != -1) && (envido_querido==ACEPTADO) && (tantos_cantados_player != -1))
    {
      this->mostrar_msj(-6, font);
      esperar(1500);
      puntos_truco_pc += puntos_truco_player;
      puntos_truco_player = 0;
      puntos_envido_pc += puntos_envido_player;
      puntos_envido_player = 0;
    }
    // Si la Pc ganó el Envido, verificar que haya mostrado los tantos //
    if((puntos_envido_pc > 0) && (envido_querido==ACEPTADO) && (jugada_player!=-1))
    {
      pos_env = this->calcular_tantos(1,1);
      if(tantos_pc < 20)
        if(!carta_pc[pos_env])
        {
          carta_pc[pos_env] = 1;
          carta.mostrar_jugadas_pc(fondo,pantalla,(carta_pc[0]+carta_pc[1]+carta_pc[2]),pos_env,c_naipes);
          tantos_mostrados = 1;
        }
      if(tantos_pc >= 20)
        for(i=0; i<3; i++)
          if(i != pos_env)
            if(!carta_pc[i])
            {
              carta_pc[i] = 1;
              carta.mostrar_jugadas_pc(fondo,pantalla,(carta_pc[0]+carta_pc[1]+carta_pc[2]),i,c_naipes);
              tantos_mostrados = 1;
            }
      if(tantos_mostrados)
      {
        this->mostrar_msj(39, font);
        esperar(2000);
      }
    }
    // Puntaje de los jugadores //
    puntos_pl += puntos_truco_player;
    puntos_pl += puntos_envido_player;
    puntos_pc += puntos_truco_pc;
    puntos_pc += puntos_envido_pc;
    // Verificar si finalizó el juego //
    if(puntos_pl >= 30)
    {
      if(puntos_pl != 30 && puntos_pl != 31)
        puntos_pl = 30;
      this->mostrar_puntaje(puntos_pc,puntos_pl,font);
      this->mostrar_msj(-4, font);
      press_enter(1);
      fin_de_juego = 1;
    }
    if(puntos_pc >= 30)
    {
      if(puntos_pc != 30 && puntos_pc != 31)
        puntos_pc = 30;
      this->mostrar_puntaje(puntos_pc,puntos_pl,font);
      this->mostrar_msj(-3, font);
      press_enter(1);
      fin_de_juego = 1;
    }
    // Cambiar de mano //
    if(!mano)
      mano = 1;
    else
      mano = 0;
    // Si el humano desea volver al menú salimos //
    if(jugada_player == -1)
      fin_de_juego = 1;
  }
  carta.free();
}

//////////////////// Comparar dos naipes para saber cual es la mejor /////////////////////
int Game::comparar(int pos_pc, int pos_humano, int canto_4, TTF_Font *font)
{
  int i, humano, pc;
  for(i=0; i<40; i++)
  {
    if(p_naipes[pos_humano]==jerarquia[i])
    {
      if(i>3)
      {
        if(i<40) humano = 36;  //Los 4
        if(i<36) humano = 32;  //Los 5
        if(i<32) humano = 28;  //Los 6
        if(i<28) humano = 26;  //Los 7
        if(i<26) humano = 22;  //Los 10
        if(i<22) humano = 18;  //Los 11
        if(i<18) humano = 14;  //Los 12
        if(i<14) humano = 11;  //Los 1
        if(i<12) humano = 8;  //Los 2
        if(i<8) humano = 4;    //Los 3
      }
      else
        humano = i;
      break;
    }
  }
  for(i=0; i<40; i++)
  {
    if(c_naipes[pos_pc]==jerarquia[i])
    {
      if(i>3)
      {
        if(i<40) pc = 36;  //Los 4
        if(i<36) pc = 32;  //Los 5
        if(i<32) pc = 28;  //Los 6
        if(i<28) pc = 26;  //Los 7
        if(i<26) pc = 22;  //Los 10
        if(i<22) pc = 18;  //Los 11
        if(i<18) pc = 14;  //Los 12
        if(i<14) pc = 11;  //Los 1
        if(i<12) pc = 8;  //Los 2
        if(i<8) pc = 4;    //Los 3
      }
      else
        pc = i;
      break;
    }
  }
  if(pc == humano)  // Las dos valen lo mismo
    return 0;
  if(pc < humano)  // La mia es mejor
    if(!canto_4)
      return 1;
    else
    {
      this->mostrar_msj(-7, font);
      return 2;
    }
  if(pc > humano)  // La del humano es mejor
    if(!canto_4)
      return 2;
    else
    {
      this->mostrar_msj(-7, font);
      return 1;
    }
}

//////////////////////////////// Limpiar Puntaje ///////////////////////////////
void Game::limpiar_puntaje()
{
  SDL_Rect f, d;
  f = (SDL_Rect) {
    439, 50, 120, 30
  };
  d = (SDL_Rect) {
    439, 50, 0, 0
  };
  SDL_BlitSurface(fondo, &f, pantalla, &d);
}

//////////////////////////////// Limpiar Texto ///////////////////////////////
void Game::limpiar_texto()
{
  SDL_Rect f = {0, 341, 799, 38};
  SDL_Rect d = {0, 341, 0, 0};
  SDL_BlitSurface(fondo, &f, pantalla, &d);
  SDL_Flip(pantalla);
}

/////////////////////////////// Mostrar Puntajes /////////////////////////////
void Game::mostrar_puntaje(int pc, int player, TTF_Font *font)
{
  int x = 440, y = 50;
  char p1[10], p2[10];
  Texto txt;
  this->limpiar_puntaje();
  txt.mostrar(x, y, pantalla, itoa(player,p1,10), 255, 255, 0, 200, font);
  txt.mostrar(x+60, y, pantalla, itoa(pc,p2,10), 255, 255, 0, 200, font);
}

/////////////////////////////// Mostrar Mensajes /////////////////////////////
void Game::mostrar_msj(int num_msj, TTF_Font *font)
{
  Texto txt;
  int y = 340;
  this->limpiar_texto();
  switch(num_msj)
  {
  case -7:
    txt.mostrar(150, y+10, pantalla, "Le cantaste al cuatro! Gano yo!", 255, 255, 0, 200, font);
    break;
  case -6:
    txt.mostrar(140, y+10, pantalla, "Mentiste! Tus tantos no son los cantados!", 255, 255, 0, 200, font);
    break;
  case -5:
    txt.mostrar(140, y+10, pantalla, "Son buenas...", 0, 255, 50, 200, font);
    break;
  case -4:
    txt.mostrar(50, y+10, pantalla, "Me ganaste! Voy a hacerle juicio al que me programó...", 255, 255, 0, 200, font);
    break;
  case -3:
    txt.mostrar(140, y+10, pantalla, "Te gané! Vas a tener que practicar más...", 255, 255, 0, 200, font);
    break;
  case -2:
    txt.mostrar(470, y+10, pantalla, "Son buenas...", 0, 55, 255, 200, font);
    break;
  case -1:
    txt.mostrar(260, y+10, pantalla, "Ingresá tus tantos...", 255, 255, 0, 200, font);
    break;
  case 0:
    txt.mostrar(320, y+10, pantalla, "Vos sos mano", 0, 255, 50, 200, font);
    break;
  case 1:
    txt.mostrar(320, y+10, pantalla, "Yo juego primero", 0, 255, 50, 200, font);
    break;
  case 2:
    txt.mostrar(240, y+10, pantalla, "Con cartas cualquiera gana!", 0, 255, 50, 200, font);
    break;
  case 3:
    txt.mostrar(230, y+10, pantalla, "Y bueno, a veces pierdo también!", 0, 255, 50, 200, font);
    break;
  case 4:
    txt.mostrar(250, y+10, pantalla, "Contra la suerte no se puede!", 0, 255, 50, 200, font);
    break;
  case 5:
    txt.mostrar(320, y+10, pantalla, "Te gané humanoide!", 0, 255, 50, 200, font);
    break;
  case 6:
    txt.mostrar(320, y+10, pantalla, "Te hice de goma!", 0, 255, 50, 200, font);
    break;
  case 7:
    txt.mostrar(250, y+10, pantalla, "La suerte está conmigo!", 0, 255, 50, 200, font);
    break;
  case 8:
    txt.mostrar(320, y+10, pantalla, "Fuiste historia!", 0, 255, 50, 200, font);
    break;
  case 9:
    txt.mostrar(250, y+10, pantalla, "Soy mano así que gano yo!", 0, 255, 50, 200, font);
    break;
  case 10:
    txt.mostrar(280, y+10, pantalla, "Me ganaste de mano!", 0, 255, 50, 200, font);
    break;
  case 11:
    txt.mostrar(320, y+10, pantalla, "Te destrozé!", 0, 255, 50, 200, font);
    break;
  case 20:
    txt.mostrar(170, y+10, pantalla, "Truco!", 0, 255, 50, 200, font);
    break;
  case 21:
    txt.mostrar(140, y+10, pantalla, "Quiero Retruco!", 0, 255, 50, 200, font);
    break;
  case 22:
    txt.mostrar(140, y+10, pantalla, "Quiero Vale 4 !", 0, 255, 50, 200, font);
    break;
  case 23:
    txt.mostrar(170, y+10, pantalla, "Quiero", 0, 255, 50, 200, font);
    break;
  case 24:
    txt.mostrar(170, y+10, pantalla, "Envido!", 0, 255, 50, 200, font);
    break;
  case 25:
    txt.mostrar(150, y+10, pantalla, "Real Envido!", 0, 255, 50, 200, font);
    break;
  case 26:
    txt.mostrar(150, y+10, pantalla, "Falta Envido!", 0, 255, 50, 200, font);
    break;
  case 27:
    txt.mostrar(160, y+10, pantalla, "No Quiero", 0, 255, 50, 200, font);
    break;
  case 28:
    txt.mostrar(120, y+10, pantalla, "Me voy al mazo!", 0, 255, 50, 200, font);
    break;
  case 30:
    txt.mostrar(470, y+10, pantalla, "Truco!", 0, 55, 255, 200, font);
    break;
  case 31:
    txt.mostrar(440, y+10, pantalla, "Quiero Retruco!", 0, 55, 255, 200, font);
    break;
  case 32:
    txt.mostrar(440, y+10, pantalla, "Quiero Vale 4 !", 0, 55, 255, 200, font);
    break;
  case 33:
    txt.mostrar(470, y+10, pantalla, "Quiero", 0, 55, 255, 200, font);
    break;
  case 34:
    txt.mostrar(470, y+10, pantalla, "Envido!", 0, 55, 255, 200, font);
    break;
  case 35:
    txt.mostrar(450, y+10, pantalla, "Real Envido!", 0, 55, 255, 200, font);
    break;
  case 36:
    txt.mostrar(450, y+10, pantalla, "Falta Envido!", 0, 55, 255, 200, font);
    break;
  case 37:
    txt.mostrar(460, y+10, pantalla, "No Quiero", 0, 55, 255, 250, font);
    break;
  case 38:
    txt.mostrar(420, y+10, pantalla, "Me voy al mazo!", 0, 55, 255, 200, font);
    break;
  case 39:
    txt.mostrar(430, y+10, pantalla, "Mis tantos...", 0, 55, 255, 200, font);
    break;
  }
}

//////////////////////////////// Inicializaciones //////////////////////////////////
Game::Game(SDL_Surface *screen)
{
  /* Preparar pantalla y Cargar Sprites */
  pantalla = screen;
  fondo = img.cargar_img("fgame.png");
  bmenu = img.cargar_img("boton_menu.png");
  b_enabled = img.cargar_img("Botones_enabled.png");
  b_disabled = img.cargar_img("Botones_disabled.png");
  /* Mostrar Imagen de Fondo */
  SDL_BlitSurface(fondo, NULL, pantalla, NULL);
  SDL_Rect f = {0, 0, bmenu->w, bmenu->h};
  SDL_Rect d = {604, 10, 0, 0};
  SDL_BlitSurface(bmenu, &f, pantalla, &d);
  SDL_Flip(pantalla);
  /* Coordenadas de los naipes */
  cx1 = 454;
  cx2 = 553;
  cy1 = 410;
  cy2 = 547;
  dist = 109;

/////// Cartas Ordenadas Jerárquicamente /////////
  jerarquia[0] = ANCHO_ESPADA;
  jerarquia[1] = ANCHO_BASTO;
  jerarquia[2] = SIETE_ESPADA;
  jerarquia[3] = SIETE_ORO;
  jerarquia[4] = TRES_BASTO;
  jerarquia[5] = TRES_COPA;
  jerarquia[6] = TRES_ESPADA;
  jerarquia[7] = TRES_ORO;
  jerarquia[8] = DOS_BASTO;
  jerarquia[9] = DOS_COPA;
  jerarquia[10] = DOS_ESPADA;
  jerarquia[11] = DOS_ORO;
  jerarquia[12] = AS_COPA;
  jerarquia[13] = AS_ORO;
  jerarquia[14] = REY_BASTO;
  jerarquia[15] = REY_COPA;
  jerarquia[16] = REY_ESPADA;
  jerarquia[17] = REY_ORO;
  jerarquia[18] = CABALLO_BASTO;
  jerarquia[19] = CABALLO_COPA;
  jerarquia[20] = CABALLO_ESPADA;
  jerarquia[21] = CABALLO_ORO;
  jerarquia[22] = SOTA_BASTO;
  jerarquia[23] = SOTA_COPA;
  jerarquia[24] = SOTA_ESPADA;
  jerarquia[25] = SOTA_ORO;
  jerarquia[26] = SIETE_BASTO;
  jerarquia[27] = SIETE_COPA;
  jerarquia[28] = SEIS_BASTO;
  jerarquia[29] = SEIS_COPA;
  jerarquia[30] = SEIS_ESPADA;
  jerarquia[31] = SEIS_ORO;
  jerarquia[32] = CINCO_BASTO;
  jerarquia[33] = CINCO_COPA;
  jerarquia[34] = CINCO_ESPADA;
  jerarquia[35] = CINCO_ORO;
  jerarquia[36] = CUATRO_BASTO;
  jerarquia[37] = CUATRO_COPA;
  jerarquia[38] = CUATRO_ESPADA;
  jerarquia[39] = CUATRO_ORO;

/////////////////////// Valor de cada naipe en el Envido ////////////////////////////
  valores[0] = valores [10] = valores [20] = valores [30] = TANTO_AS;
  valores[1] = valores [11] = valores [21] = valores [31] = TANTO_DOS;
  valores[2] = valores [12] = valores [22] = valores [32] = TANTO_TRES;
  valores[3] = valores [13] = valores [23] = valores [33] = TANTO_CUATRO;
  valores[4] = valores [14] = valores [24] = valores [34] = TANTO_CINCO;
  valores[5] = valores [15] = valores [25] = valores [35] = TANTO_SEIS;
  valores[6] = valores [16] = valores [26] = valores [36] = TANTO_SIETE;
  valores[7] = valores [17] = valores [27] = valores [37] = TANTO_SOTA;
  valores[8] = valores [18] = valores [28] = valores [38] = TANTO_CABALLO;
  valores[9] = valores [19] = valores [29] = valores [39] = TANTO_REY;
}

////////////////////////////// Liberamos la memoria usada ////////////////////////////////
void Game::free()
{
  SDL_FreeSurface(pantalla);
  SDL_FreeSurface(fondo);
  SDL_FreeSurface(bmenu);
  SDL_FreeSurface(b_enabled);
  SDL_FreeSurface(b_disabled);
}

#endif

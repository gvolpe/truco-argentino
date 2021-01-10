#ifndef COMPU_H
#define COMPU_H

#include "cartas.h"
#include <iostream>

using namespace std;

class Computer
{
private:
  SDL_Surface *img_fondo, *pantalla;
public:
  Computer(SDL_Surface *fondo, SDL_Surface *screen);
  int jugar(int *c_naipes, int *p_naipes, int p_jugada, Carta carta, int *jerarquia, int *carta_j, int parda, int truco_player, int *truco_pc, int accion, int tantos, int envido_cantado, int *pendiente, int primera, int segunda, int tercera, int truco_cantado, int puntos_pl, int puntos_pc, int falta, int canto_truco, int *orden_jugada, int *canto_al_4, int second);
  int buscar_menor(int lim, int *c_naipes, int *jerarquia, int *carta_j);
  int buscar_mayor(int lim, int *c_naipes, int *jerarquia, int *carta_j);
  int evaluar_naipes(int *naipes, int *jerarquia, int accion, int pos); //0: Excelentes    1:Muy buenas    2:Buenas     3:Malas     4:Muy Malas
};


// Constructor //
Computer::Computer(SDL_Surface *fondo, SDL_Surface *screen)
{
  img_fondo = fondo;
  pantalla = screen;
}

//////////// Evalúa el valor de mis naipes para saber si cantar/aceptar Truco, etc./////////////
int Computer::evaluar_naipes(int *naipes, int *jerarquia, int accion, int pos)
{
  int i, j;
  int excel = 0, muybuenas = 0, buenas = 0, malas = 0, muymalas = 0;
  if(accion == JUGAR_VERIFICANDO_NAIPE) // Evaluar todas mis naipes
  {
    /// Establecer cuantas naipes de tal califación tengo ///
    for(i=0; i<3; i++)
    {
      for(j=0; j<4; j++) // Los dos anchos y los dos siete
      {
        if(naipes[i]==jerarquia[j])
          excel++;
      }
      for(j=4; j<12; j++) // Los tres y los dos
      {
        if(naipes[i]==jerarquia[j])
          muybuenas++;
      }
      for(j=12; j<18; j++) // Los anchos falsos y los reyes
      {
        if(naipes[i]==jerarquia[j])
          buenas++;
      }
      for(j=18; j<26; j++) // Los caballos y las sotas
      {
        if(naipes[i]==jerarquia[j])
          malas++;
      }
      for(j=26; j<40; j++) // Los siete, seis, cinvo y cuatro
      {
        if(naipes[i]==jerarquia[j])
          muymalas++;
      }
    }
    /// Evaluar la calificación de mis naipes y retornar un valor ///
    if(muymalas >= 2)
      return MUY_MALAS;
    if(malas >= 2)
      return MALAS;
    if(buenas >= 2)
      return BUENAS;
    if(muybuenas >= 2)
      return MUY_BUENAS;
    if(excel >= 2)
      return EXCELENTES;
    if((excel == 1) && (muybuenas == 1))
      return MUY_BUENAS;
    // Si tengo solo una de alguna de las cinco calificaciones devolvemos que son buenas //
    return BUENAS;
  }
  if(accion == JUGAR_SIN_VERIFICAR_NAIPE) // Evaluar solo Naipe[pos]
  {
    for(j=0; j<4; j++) // Los dos anchos y los dos siete
    {
      if(naipes[pos]==jerarquia[j])
        excel++;
    }
    for(j=4; j<12; j++) // Los tres y los dos
    {
      if(naipes[pos]==jerarquia[j])
        muybuenas++;
    }
    for(j=12; j<18; j++) // Los anchos falsos y los reyes
    {
      if(naipes[pos]==jerarquia[j])
        buenas++;
    }
    for(j=18; j<26; j++) // Los caballos y las sotas
    {
      if(naipes[pos]==jerarquia[j])
        malas++;
    }
    for(j=26; j<40; j++) // Los siete, seis, cinvo y cuatro
    {
      if(naipes[pos]==jerarquia[j])
        muymalas++;
    }
    /// Evaluar la calificación del naipe y retornar un valor ///
    if(muymalas)
      return MUY_MALAS;
    if(malas)
      return MALAS;
    if(buenas)
      return BUENAS;
    if(muybuenas)
      return MUY_BUENAS;
    if(excel)
      return EXCELENTES;
  }
}

///////////////////////// Lógica de la computadora ////////////////////////////
int Computer::jugar(int *c_naipes,
                    int *p_naipes,
                    int p_jugada,
                    Carta carta,
                    int *jerarquia,
                    int *carta_j,
                    int parda,
                    int truco_player,
                    int *truco_pc,
                    int accion,
                    int tantos,
                    int envido_cantado,
                    int *pendiente,
                    int primera,
                    int segunda,
                    int tercera,
                    int truco_cantado,
                    int puntos_pl,
                    int puntos_pc,
                    int falta,
                    int canto_truco,
                    int *orden_jugada,
                    int *canto_al_4,
                    int second)
{
  int i, buscar = 0, candidata=-1, jugada_player = p_naipes[p_jugada], posi;
  int valores = -1, jugadas = (carta_j[0] + carta_j[1] + carta_j[2]);
  int no_cantar = 0, suma = 0;

// Verificar si el humano cantó algo... //
  if(truco_player>-1 && truco_player<8)
  {
    switch(truco_player)
    {
    case TRUCO:
      if((envido_cantado == 0) && (*truco_pc == -1) && (jugadas == 0))
      {
        // Verificar si tengo tantos para el Envido
        if(tantos > 26)
          *truco_pc = ENVIDO;
        if(tantos > 28)
          *truco_pc = REAL_ENVIDO;
        if(tantos > 31)
          *truco_pc = FALTA_ENVIDO;
        if(*truco_pc != -1)
        {
          *pendiente = 1;
          return 3;
        }
      }

      if(puntos_pl == 29)
      {
        *truco_pc = RETRUCO;
        return 3;
      }

      posi = orden_jugada[2];
      if((tercera == 1) && ((c_naipes[posi] == 3) || (c_naipes[posi] == 13) || (c_naipes[posi] == 23) || (c_naipes[posi] == 33)))
      {
        cout << "\n(tercera = 1) Humano le cantó al cuatro!";
        *truco_pc = QUIERO;
        *canto_al_4 = 1;
        return 3;
      }

      valores = this->evaluar_naipes(c_naipes, jerarquia, 1, 0);
      switch(valores)
      {
      case EXCELENTES:
        if((primera == 1) || (primera == 2) || (second == 1))
          *truco_pc = RETRUCO;
        else
          *truco_pc = QUIERO;
        break;
      case MUY_BUENAS:
        if((primera == 1) || (primera == 2) || (second == 1))
          *truco_pc = RETRUCO;
        else
          *truco_pc = QUIERO;
        break;
      case BUENAS:
        if((primera == 1) || (primera == 2) || (second == 1))
          *truco_pc = QUIERO;
        else
          *truco_pc = NO_QUIERO;
        break;
      case MALAS:
        if((primera == 1) || (primera == 2) || (second == 1))
          *truco_pc = QUIERO;
        else
          *truco_pc = NO_QUIERO;
        break;
      case MUY_MALAS:
        *truco_pc = NO_QUIERO;
        break;
      }
      break;
    case RETRUCO:
      if(puntos_pl == 29)
      {
        *truco_pc = VALE4;
        return 3;
      }
      if((primera == 1) || (primera == 2) || (second == 1))
        *truco_pc = QUIERO;
      else
        *truco_pc = NO_QUIERO;
      if(second == 0)
      {
        for(i=0; i<3; i++)
          if(carta_j[i] == 0)
          {
            posi = i;
            break;
          }
        valores = this->evaluar_naipes(c_naipes, jerarquia, 0, posi);
        if(valores == 0)
          *truco_pc = VALE4;
      }
      break;
    case 2:
      if(puntos_pl == 29)
      {
        *truco_pc = QUIERO;
        return 3;
      }
      if((primera == 1) || (primera == 2) || (second == 1))
        *truco_pc = QUIERO;
      else
        *truco_pc = NO_QUIERO;
      break;
    case QUIERO:
      break;
    case ENVIDO:
      if(puntos_pc == 29)
      {
        *truco_pc = FALTA_ENVIDO;
      }
      else
      {
        if(puntos_pl >= 28)
        {
          *truco_pc = QUIERO;
          return 3;
        }
        if(tantos < 25)
        {
          *truco_pc = NO_QUIERO;
        }
        if(tantos > 24)
        {
          *truco_pc = QUIERO;
        }
        if(tantos >= 29)
        {
          *truco_pc = REAL_ENVIDO;
        }
        if(tantos > 31)
        {
          *truco_pc = FALTA_ENVIDO;
        }
        if(puntos_pl == 29)
        {
          *truco_pc = QUIERO;
        }
      }
      break;
    case REAL_ENVIDO:
      if(puntos_pc >= 28)
      {
        *truco_pc = FALTA_ENVIDO;
      }
      else
      {
        if(puntos_pl >= 27)
        {
          *truco_pc = QUIERO;
          return 3;
        }
        if(tantos < 29)
        {
          *truco_pc = NO_QUIERO;
        }
        if(tantos >= 29)
        {
          *truco_pc = QUIERO;
        }
        if(tantos > 31)
        {
          *truco_pc = FALTA_ENVIDO;
        }
        if(puntos_pl >= 28)
        {
          *truco_pc = QUIERO;
        }
      }
      break;
    case FALTA_ENVIDO:
      if(puntos_pl == 29)
      {
        *truco_pc = QUIERO;
        return 3;
      }
      if((tantos > 30) || (puntos_pl >= 28))
      {
        *truco_pc = QUIERO;
      }
      if(tantos <= 30)
      {
        *truco_pc = NO_QUIERO;
      }
      break;
    case NO_QUIERO:
      break;
    case AL_MAZO:
      break;
    }
    if(*truco_pc != -1)
      return 3;
  }
// Jugar naipe //
  if(accion) // Si el humano jugó antes
  {
    if((envido_cantado == 0) && (*truco_pc == -1) && (jugadas == 0))
    {
      // Verificar si tengo tantos para el Envido
      if(tantos > 26)
        *truco_pc = ENVIDO;
      if(tantos > 28)
        *truco_pc = REAL_ENVIDO;
      if(tantos > 31)
        *truco_pc = FALTA_ENVIDO;
      if(*truco_pc != -1)
        return 3;
    }

    if((second == 0) && (!no_cantar)) // Hice segunda
    {
      posi = (orden_jugada[0] + orden_jugada[1]);
      if(posi == 1)
        posi = 2;
      if(posi == 2)
        posi = 1;
      if(posi == 3)
        posi = 0;
      // Verificar si el humano jugó un cuatro en tercera //
      if((tercera == 0) && ((jugada_player == 3) || (jugada_player == 13) || (jugada_player == 23) || (jugada_player == 33)))
      {
        // Verificar si tengo un cuarto antes de cantar //
        if((c_naipes[posi] == 3) || (c_naipes[posi] == 13) || (c_naipes[posi] == 23) || (c_naipes[posi] == 33))
        {
          if(!truco_cantado)
          {
            *truco_pc = 0;
            return 3;
          }
        }
      }

      // Si tengo alguna naipe buena le cantamos truco o retruco //
      valores = this->evaluar_naipes(c_naipes, jerarquia, 0, posi);
      if((valores == 0) || (valores == 1))
      {
        if(!truco_cantado)
        {
          *truco_pc = TRUCO;
          return 3;
        }
        if(valores == 0)
        {
          if((canto_truco == 0) && (truco_player != 3) && (truco_player != 2))  // Si el Humano había cantado Truco
          {
            *truco_pc = RETRUCO;
            return 3;
          }
        }
      }
    }

    ////////// Verificar naipe jugado para saber cual de los míos jugar ///////////
    for(i=0; i<40; i++)
    {
      if(jugada_player == jerarquia[i])
      {
        if(i > 3)
        {
          if(i<40) buscar = 36;  //Los 4
          if(i<36) buscar = 32;  //Los 5
          if(i<32) buscar = 28;  //Los 6
          if(i<28) buscar = 26;  //Los 7
          if(i<26) buscar = 22;  //Los 10
          if(i<22) buscar = 18;  //Los 11
          if(i<18) buscar = 14;  //Los 12
          if(i<14) buscar = 11;  //Los 1
          if(i<12) buscar = 8;  //Los 2
          if(i<8) buscar = 4;    //Los 3
        }
        else
          buscar = i;
        break;
      }
    }

    // Buscar entre mis naipes alguna que le gane a la jugada por el humano
    candidata = this->buscar_menor(buscar, c_naipes, jerarquia, carta_j);

    // Si no tengo ninguna mejor que la jugada
    if(candidata == -1)
      candidata = this->buscar_menor(40, c_naipes, jerarquia, carta_j);
  }
  else // Si juego yo primero
  {
    if((envido_cantado == 0) && (*truco_pc == -1) && (jugadas == 0))
    {
      // Verificar si tengo tantos para el Envido
      if(tantos > 26)
        *truco_pc = ENVIDO;
      if(tantos > 28)
        *truco_pc = REAL_ENVIDO;
      if(tantos > 31)
        *truco_pc = FALTA_ENVIDO;
      if(*truco_pc != -1)
        return 3;
    }

    if((second == 1) && (!no_cantar)) // Hice segunda
    {
      for(i=0; i<3; i++)
        if(carta_j[i] == 0)
        {
          posi = i;
          break;
        }
      valores = this->evaluar_naipes(c_naipes, jerarquia, 0, posi);
      if((valores == 0) || (valores == 1))
      {
        if(!truco_cantado)
        {
          *truco_pc = TRUCO;
          return 3;
        }
        if(valores == 0)
        {
          if((canto_truco == 0) && (truco_player != 3) && (truco_player != 2))  // Si el Humano había cantado Truco
          {
            *truco_pc = RETRUCO;
            return 3;
          }
        }
      }
    }

    valores = this->evaluar_naipes(c_naipes, jerarquia, 1, 0);
    if((valores == 0) || (valores == 1))
    {
      if(!truco_cantado)
        if((primera == 1) || (primera == 2) || (segunda == 1))
        {
          *truco_pc = TRUCO;
          return 3;
        }
    }

    if(segunda == 1)
    {
      posi = orden_jugada[2];
      valores = this->evaluar_naipes(c_naipes, jerarquia, 0, posi);
      if((valores == 0) || (valores == 1))
      {
        if(!truco_cantado)
        {
          *truco_pc = TRUCO;
          return 3;
        }
        if(valores == 0)
        {
          if((canto_truco == 0) && (truco_player != 3) && (truco_player != 2))  // Si el Humano había cantado Truco
            *truco_pc = RETRUCO;
        }
      }
    }

    if(!parda)
      candidata = this->buscar_menor(40, c_naipes, jerarquia, carta_j);
    else
    {
      candidata = this->buscar_mayor(40, c_naipes, jerarquia, carta_j);
      if(!no_cantar)
      {
        for(i=0; i<4; i++)
        {
          if(c_naipes[candidata] == jerarquia[i])
          {
            if(!truco_cantado)
            {
              *truco_pc = TRUCO;
              return 3;
            }
          }
        }
      }
    }
  }
//////////////////// Jugar la naipe que seleccione //////////////////////
  carta_j[candidata] = 1;
  carta.mostrar_jugadas_pc(img_fondo,pantalla,(carta_j[0]+carta_j[1]+carta_j[2]),candidata,c_naipes);
  return candidata;
}

/////////////// Busca la menor de mis naipes aún no jugadas /////////////////
int Computer::buscar_menor(int lim, int *c_naipes, int *jerarquia, int *carta_j)
{
  int i, j, candidata = -1;
  for(i=0; i<lim; i++)
    for(j=0; j<3; j++)
      if(c_naipes[j]==jerarquia[i])
        if(carta_j[j]==0)          // Si el naipe no se jugó
          candidata = j;         // Posición de naipe seleccionada
  return candidata;
}

/////////////// Busca la mayor de mis naipes aún no jugadas /////////////////
int Computer::buscar_mayor(int lim, int *c_naipes, int *jerarquia, int *carta_j)
{
  int i, j, candidata = -1;
  for(i=(lim-1); i>=0; i--)
    for(j=0; j<3; j++)
      if(c_naipes[j]==jerarquia[i])
        if(carta_j[j]==0)				// Si el naipe no se jugó
          candidata = j;				// Posición de naipe seleccionada
  return candidata;
}

#endif

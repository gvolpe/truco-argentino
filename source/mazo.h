#ifndef MAZO_H
#define MAZO_H

class Mazo
{
private:
  int naipe[40]; //Mazo
public:
  Mazo();
  void barajar();
  void repartir(int *player, int *compu, int cant);
};

Mazo::Mazo()
{
  /*  0  a  9: 10 Naipes de Basto
  	10 a 19: 10 Naipes de Copa
  	20 a 29: 10 Naipes de Espada
  	30 a 39: 10 Naipes de Oro  */
  for(int i=0; i<40; i++)
    naipe[i]= i;
}

/**************** Barajar Naipes ******************/
void Mazo::barajar()
{
  int i, cont = 0, carta = 0, aux[40], repet = 0;
// Inicializamos el vector en donde se cargará la baraja mezclada
  for(i=0; i<40; i++)
    aux[i] = -1;
// Barajar las 40 Naipes
  srand(time(NULL));
  while(cont<40)
  {
    repet = 0;
    carta = (rand() % 40);
    // Comprobar que la carta no haya salido
    for(i=0; i<cont; i++)
      if(carta==aux[i])
        repet = 1;
    // Ubicar la carta en la baraja inicial
    if(!repet)
      for(i=0; i<40; i++)
        if(carta==naipe[i])
        {
          aux[cont]=carta;
          cont++;
          break;
        }
  }
// Cargamos la Baraja Mezclada en Naipe[]
  for(i=0; i<40; i++)
    naipe[i] = aux[i];
}
/**************** Repartir Naipes a cada jugador ******************/
void Mazo::repartir(int *player, int *compu, int cant)
{
// Le asignamos cant naipes a c/u (En este juego 3)
  for(int i=0; i<cant; i++)
  {
    player[i] = naipe[i*2]; //Cartas pares (0,2,4) del mazo
    compu[i] = naipe[(i*2)+1];  //Cartas impares (1,3,5) del mazo
  }
}


#endif

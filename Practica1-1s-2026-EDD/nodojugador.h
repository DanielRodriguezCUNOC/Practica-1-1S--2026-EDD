#ifndef NODOJUGADOR_H
#define NODOJUGADOR_H
#include "jugador.h"
class NodoJugador
{
public:

    NodoJugador* siguiente;
    NodoJugador* anterior;
    Jugador jugador;
    NodoJugador();
    NodoJugador(Jugador jugador);
    ~NodoJugador();

};

#endif // NODOJUGADOR_H

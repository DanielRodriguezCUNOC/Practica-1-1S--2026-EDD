#ifndef LISTAJUGADORES_H
#define LISTAJUGADORES_H
#include "nodojugador.h"

class ListaJugadores
{
public:
    ListaJugadores();
    void agregarJugador(Jugador jugador);
    void siguienteTurno();
    void invertirDireccion();
    NodoJugador* jugadorActual;
    bool sentidoHorario;
    ~ListaJugadores();
};

#endif // LISTAJUGADORES_H

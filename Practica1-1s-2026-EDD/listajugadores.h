#ifndef LISTAJUGADORES_H
#define LISTAJUGADORES_H
#include "nodojugador.h"
#include <QString>

class ListaJugadores
{
public:
    ListaJugadores();
    void agregarJugador(QString nombre);
    void siguienteTurno();
    void invertirDireccion();
    NodoJugador* jugadorActual;
    bool sentidoHorario;
    ~ListaJugadores();
};

#endif // LISTAJUGADORES_H

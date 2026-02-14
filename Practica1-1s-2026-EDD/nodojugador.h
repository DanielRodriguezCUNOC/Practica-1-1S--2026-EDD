#ifndef NODOJUGADOR_H
#define NODOJUGADOR_H
#include <QString>
class NodoJugador
{
public:

    NodoJugador* siguiente;
    NodoJugador* anterior;
    QString nombreJugador;
    NodoJugador();
    NodoJugador(QString nombre);
    ~NodoJugador();

};

#endif // NODOJUGADOR_H

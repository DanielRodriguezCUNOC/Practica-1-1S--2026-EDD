#ifndef JUGADOR_H
#define JUGADOR_H
#include <string>
#include "carta.h"
#include "listagenerica.h"
class Jugador
{
public:
    Jugador();
    void setNombreJugador(std::string nombre);
    std::string getNombreJugador();


private:
    std::string nombreJugador;
    bool dijoUno;
    ListaGenerica<Carta*> mano;
};


#endif // JUGADOR_H

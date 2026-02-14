#include "jugador.h"
#include <QString>
Jugador::Jugador() {}

QString Jugador::getNombreJugador(){
    return nombreJugador;
}
void Jugador::setNombreJugador(QString nombre){
    nombreJugador = nombre;
}


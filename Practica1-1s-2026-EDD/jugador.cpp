#include "jugador.h"
Jugador::Jugador() {}

std::string Jugador::getNombreJugador(){
    return nombreJugador;
}
void Jugador::setNombreJugador(std::string nombre){
    nombreJugador = nombre;
}


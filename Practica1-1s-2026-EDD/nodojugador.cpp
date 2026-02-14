#include "nodojugador.h"


//NodoJugador::NodoJugador() {}
NodoJugador::NodoJugador(Jugador nuevoJugador): jugador(nuevoJugador), siguiente(nullptr), anterior(nullptr) {}

NodoJugador::~NodoJugador(){
}

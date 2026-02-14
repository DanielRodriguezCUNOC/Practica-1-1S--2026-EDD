#include "nodojugador.h"
#include <QString>
//NodoJugador::NodoJugador() {}
NodoJugador::NodoJugador(QString nombre): nombreJugador(nombre), siguiente(nullptr), anterior(nullptr) {}

NodoJugador::~NodoJugador(){
}

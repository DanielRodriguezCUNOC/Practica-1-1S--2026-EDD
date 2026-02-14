#include "listajugadores.h"
#include <QString>

ListaJugadores::ListaJugadores(): jugadorActual(nullptr), sentidoHorario(true){}

void ListaJugadores::agregarJugador(QString nombre) {
    NodoJugador* nuevo = new NodoJugador(nombre);

    if (!jugadorActual) {

        jugadorActual = nuevo;
        jugadorActual->siguiente = jugadorActual;
        jugadorActual->anterior = jugadorActual;
    } else {

        // Insertar antes del jugador actual para mantener la circularidad
        NodoJugador* ultimo = jugadorActual->anterior;

        nuevo->siguiente = jugadorActual;
        nuevo->anterior = ultimo;

        ultimo->siguiente = nuevo;
        jugadorActual->anterior = nuevo;
    }
}

void ListaJugadores::siguienteTurno() {
    if (!jugadorActual) return;

    if (sentidoHorario) {
        jugadorActual = jugadorActual->siguiente;
    } else {
        jugadorActual = jugadorActual->anterior;
    }
}

void ListaJugadores::invertirDireccion() {
    sentidoHorario = !sentidoHorario;
}
ListaJugadores::~ListaJugadores(){}

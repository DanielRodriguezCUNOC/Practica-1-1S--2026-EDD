#include "ladoflip.h"
#include "juego.h"

LadoFlip::LadoFlip(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::FLIP, c, n, ruta){}

LadoCarta* LadoFlip::clonar() const {
    return new LadoFlip(*this);
}

void LadoFlip::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
                             const std::string jugadorSeleccionado, int numeroSeleccionado) {
    // Invertimos el estado global del juego
    juego->setLadoOscuroActivo(!juego->getLadoOscuroActivo());

    // Recorremos TODAS las cartas y las volteamos
    // 1. Voltear Mazo
    for(int i = 0; i < juego->getTamanoMazo(); i++) {
        juego->getMazo().obtenerElementoEnPosicion(i).voltear();
    }
    // 2. Voltear Descarte
    for(int i = 0; i < juego->getTamanoDescarte(); i++) {
        juego->getDescarte().obtenerElementoEnPosicion(i).voltear();
    }
    // 3. Voltear Manos de Jugadores
    for(int i = 0; i < juego->getCantidadJugadores(); i++) {
        Jugador* j = juego->getJugadorEnPosicion(i);
        for(int k = 0; k < j->cantidadCartas(); k++) {
            j->getMano().obtenerElementoEnPosicion(k).voltear();
        }
    }
}

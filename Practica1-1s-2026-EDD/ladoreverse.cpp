#include "ladoreverse.h"
#include "juego.h"

LadoReverse::LadoReverse(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::REVERSE, c, n, ruta){}

LadoCarta* LadoReverse::clonar() const {
    return new LadoReverse(*this);
}

void LadoReverse::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
                                const std::string jugadorSeleccionado, int numeroSeleccionado) {
    // Invierte el sentido (de 1 a -1, o de -1 a 1)
    juego->setSentidoJuego(juego->getSentidoJuego() * -1);

    // Regla oficial de UNO: Si solo hay 2 jugadores, Reverse actúa como Salto
    if (juego->getCantidadJugadores() == 2) {
        juego->avanzarTurno();
    }
}

#include "ladocomodinrobarcuatro.h"
#include "juego.h"

LadoComodinRobarCuatro::LadoComodinRobarCuatro(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::COMODIN4, c, n, ruta){}

LadoCarta* LadoComodinRobarCuatro::clonar() const {
    return new LadoComodinRobarCuatro(*this);
}

void LadoComodinRobarCuatro::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
                                 const std::string jugadorSeleccionado, int numeroSeleccionado) {
    // El siguiente jugador pierde el turno y roba 4 cartas
    juego->avanzarTurno();
    Jugador* victima = juego->getJugadorActual();

    for(int i = 0; i < 4; i++) {
        if(juego->mazoEstaVacio()) juego->barajarDescarte();

        Carta robada = juego->robarDelMazo();
        if(robada.esValida())victima->agregarCarta(robada);
    }
}

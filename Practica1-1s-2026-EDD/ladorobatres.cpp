
#include "ladorobatres.h"
#include "juego.h"

LadoRobaTres::LadoRobaTres(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::ROBA3, c, n, ruta){}

LadoCarta* LadoRobaTres::clonar() const {
    return new LadoRobaTres(*this);
}

void LadoRobaTres::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
                              const std::string jugadorSeleccionado, int numeroSeleccionado) {
    // El siguiente jugador pierde el turno y roba 3 cartas
    juego->avanzarTurno();
    Jugador* victima = juego->getJugadorActual();

    for(int i = 0; i < 3; i++) {
        if(juego->mazoEstaVacio()) juego->barajarDescarte();

        Carta robada = juego->robarDelMazo();
        if(robada.esValida())victima->agregarCarta(robada);
    }

}

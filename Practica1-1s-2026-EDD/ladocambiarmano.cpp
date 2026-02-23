#include "ladocambiarmano.h"
#include "juego.h"

LadoCambiarMano::LadoCambiarMano(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::CAMBIARMANO, c, n, ruta){}

LadoCarta* LadoCambiarMano::clonar() const {
    return new LadoCambiarMano(*this);
}

void LadoCambiarMano::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
                                    const std::string jugadorSeleccionado, int numeroSeleccionado) {

    int cartasEnManoVictima = 0;
    juego->avanzarTurno();
    Jugador* victima = juego->getJugadorActual();
    cartasEnManoVictima = victima->getMano().getSize();

    // El jugador manda su mano a la pila de descarte
    while(!victima->getMano().estaVacia()) {
        Carta cartaDescartar = victima->getMano().obtenerElementoEnPosicion(0);
        victima->getMano().eliminarDatoEnPosicion(0);
        juego->agregarADescarte(cartaDescartar);
    }

    // La víctima roba la misma cantidad de cartas que mandó a la pila
    for(int i = 0; i < cartasEnManoVictima; i++) {
        if(juego->mazoEstaVacio()) {
            juego->barajarDescarte();
        }
        Carta robada = juego->robarDelMazo();
        if(robada.esValida()) victima->agregarCarta(robada);
    }
}

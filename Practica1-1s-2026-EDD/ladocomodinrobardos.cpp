
#include "ladocomodinrobardos.h"
#include "juego.h"

LadoComodinRobarDos::LadoComodinRobarDos(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::COMODIN2, c, n, ruta){}

LadoCarta* LadoComodinRobarDos::clonar() const {
    return new LadoComodinRobarDos(*this);
}

void LadoComodinRobarDos::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
                                 const std::string jugadorSeleccionado, int numeroSeleccionado) {
    if (juego->getAcumulacion()) {
        juego->setPenaAcumulada(juego->getPenaAcumulada() + 2);
        juego->setTipoPenaActual(TipoCarta::COMODIN2);
        return;
    }
    juego->avanzarTurno();
    Jugador* victima = juego->getJugadorActual();
    for (int i = 0; i < 2; i++) {
        if (juego->mazoEstaVacio()) juego->barajarDescarte();
        Carta robada = juego->robarDelMazo();
        if (robada.esValida()) victima->agregarCarta(robada);
    }
}

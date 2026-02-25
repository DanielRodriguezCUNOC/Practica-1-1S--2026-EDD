#include "ladocomodinrobarseis.h"
#include "juego.h"
LadoComodinRobarSeis::LadoComodinRobarSeis(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::COMODIN6, c, n, ruta){}

LadoCarta* LadoComodinRobarSeis::clonar() const {
    // Llama al constructor de copia de LadoNumero
    return new LadoComodinRobarSeis(*this);
}

void LadoComodinRobarSeis::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
                                     const std::string jugadorSeleccionado, int numeroSeleccionado){
    if (juego->getAcumulacion()) {
        juego->setPenaAcumulada(juego->getPenaAcumulada() + 6);
        juego->setTipoPenaActual(TipoCarta::COMODIN6);
        return;
    }
    juego->avanzarTurno();
    Jugador* victima = juego->getJugadorActual();
    for (int i = 0; i < 6; i++) {
        if (juego->mazoEstaVacio()) juego->barajarDescarte();
        Carta robada = juego->robarDelMazo();
        if (robada.esValida()) victima->agregarCarta(robada);
    }
}

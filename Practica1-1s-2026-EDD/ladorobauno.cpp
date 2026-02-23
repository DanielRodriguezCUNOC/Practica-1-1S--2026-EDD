#include "ladorobauno.h"
#include "juego.h"

LadoRobaUno::LadoRobaUno(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::ROBA1, c, n, ruta){}

LadoCarta* LadoRobaUno::clonar() const {
    return new LadoRobaUno(*this);
}

void LadoRobaUno::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
                              const std::string jugadorSeleccionado, int numeroSeleccionado) {
    // El siguiente jugador pierde el turno y roba 1 carta
    juego->avanzarTurno();
    Jugador* victima = juego->getJugadorActual();

    if (juego->mazoEstaVacio()) juego->barajarDescarte();

    Carta robada = juego->robarDelMazo();
    if(robada.esValida())victima->agregarCarta(robada);

}

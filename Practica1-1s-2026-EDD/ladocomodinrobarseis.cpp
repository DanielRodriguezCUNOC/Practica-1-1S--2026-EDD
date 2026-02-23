#include "ladocomodinrobarseis.h"
#include "juego.h"
LadoComodinRobarSeis::LadoComodinRobarSeis(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::NUMERO, c, n, ruta){}

LadoCarta* LadoComodinRobarSeis::clonar() const {
    // Llama al constructor de copia de LadoNumero
    return new LadoComodinRobarSeis(*this);
}

void LadoComodinRobarSeis::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
                                     const std::string jugadorSeleccionado, int numeroSeleccionado){
    // El siguiente jugador pierde el turno y roba 6 cartas
    juego->avanzarTurno();
    Jugador* victima = juego->getJugadorActual();

    for(int i = 0; i < 6; i++) {
        if(juego->mazoEstaVacio()) juego->barajarDescarte();

        Carta robada = juego->robarDelMazo();
        if(robada.esValida())victima->agregarCarta(robada);
    }
}

#include "ladosaltotodos.h"
#include "juego.h"

LadoSaltoTodos::LadoSaltoTodos(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::SALTODOS, c, n, ruta){}

LadoCarta* LadoSaltoTodos::clonar() const {
    return new LadoSaltoTodos(*this);
}

void LadoSaltoTodos::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
                                   const std::string jugadorSeleccionado, int numeroSeleccionado) {

    // Damos la vuelta completa a la lista de jugadores
    for(int i = 0; i < juego->getCantidadJugadores() - 1; i++) {
        juego->avanzarTurno();
    }

}

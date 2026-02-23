#include "ladosalto.h"
#include "juego.h"

LadoSalto::LadoSalto(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::SALTO, c, n, ruta){}

LadoCarta* LadoSalto::clonar() const {
    return new LadoSalto(*this);
}

void LadoSalto::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
                              const std::string jugadorSeleccionado, int numeroSeleccionado) {

    juego->avanzarTurno();

}

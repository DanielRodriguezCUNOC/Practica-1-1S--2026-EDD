#include "ladodorsogenerico.h"
#include "juego.h"

LadoDorsoGenerico::LadoDorsoGenerico(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::DORSOGENERICO, c, n, ruta){}

LadoCarta* LadoDorsoGenerico::clonar() const {
    return new LadoDorsoGenerico(*this);
}

void LadoDorsoGenerico::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
                                      const std::string jugadorSeleccionado, int numeroSeleccionado) {
    // El dorso genérico no tiene efecto
}

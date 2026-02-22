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
    Color nuevoColor = juego->convertirStringAColor(colorSeleccionado);
    juego->setColorActivo( nuevoColor);
}

#include "ladocomodincolor.h"
#include "juego.h"

LadoComodinColor::LadoComodinColor(Color c, int n, std::string ruta):
 LadoCarta(TipoCarta::NUMERO, c, n, ruta){}

LadoCarta* LadoComodinColor::clonar() const {
    // Llama al constructor de copia de LadoNumero
    return new LadoComodinColor(*this);
}

void LadoComodinColor::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
const std::string jugadorSeleccionado, int numeroSeleccionado){
    Color nuevoColor = juego->convertirStringAColor(colorSeleccionado);
    juego->setColorActivo( nuevoColor);
}


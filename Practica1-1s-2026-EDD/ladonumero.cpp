#include "ladonumero.h"
#include "juego.h"
LadoNumero::LadoNumero(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::NUMERO, c, n, ruta){}

void LadoNumero::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
const std::string jugadorSeleccionado, int numeroSeleccionado  ){
 //* No aplica efectos
}

LadoCarta* LadoNumero::clonar() const {
    // Llama al constructor de copia de LadoNumero
    return new LadoNumero(*this);
}

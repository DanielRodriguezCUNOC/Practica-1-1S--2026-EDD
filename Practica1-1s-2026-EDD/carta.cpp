#include "carta.h"

Carta::~Carta(){}

std::string Carta::getRutaImagen() const {
    return getLadoActivo().getRutaArchivo();

}

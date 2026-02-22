#include "ladocarta.h"

LadoCarta::LadoCarta(TipoCarta t, Color c, int n, std::string ruta)
    : tipo(t)
    , color(c)
    , numero(n)
    , rutaArchivo(ruta)
{}

TipoCarta LadoCarta::getTipo() const { return tipo; }
Color LadoCarta::getColor() const { return color; }
int LadoCarta::getNumero() const { return numero; }


std::string LadoCarta::getRutaArchivo() const{
    return rutaArchivo;
}

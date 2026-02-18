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

std::string LadoCarta::colorToString() const {
    switch (color) {
    case Color::Rojo: return "ROJO";
    case Color::Amarillo: return "AMARILLO";
    case Color::Azul: return "AZUL";
    case Color::Verde: return "VERDE";
    case Color::Naranja: return "NARANJA";
    case Color::Rosa: return "ROSA";
    case Color::Turquesa: return "TURQUESA";
    case Color::Purpura: return "PURPURA";
    default: return "NEGRO";
    }
}

std::string LadoCarta::tipoToString() const {
    switch (tipo) {
    case TipoCarta::Numero: return std::to_string(numero);
    case TipoCarta::Roba1: return "A+1";
    case TipoCarta::Roba2: return "+2";
    case TipoCarta::Salto: return "SALTO";
    case TipoCarta::Reverse: return "REVERSE";
    case TipoCarta::Flip: return "FLIP";
    case TipoCarta::SaltoTodos: return "SALTOTODOS";
    case TipoCarta::Comodin: return "COMODIN";
    // Agrega el resto de casos aquí...
    default: return "ERROR";
    }
}

std::string LadoCarta::getNombreCarpetaColor() const {
    return colorToString();
}

std::string LadoCarta::getNombreArchivo() const {
    return tipoToString();
}

std::string LadoCarta::getRutaArchivo() const{
    return rutaArchivo;
}

#include "carta.h"

Carta::Carta(LadoCarta frente, LadoCarta dorso)
    : ladoFrente(frente), ladoDorso(dorso), estaVolteada(false) {}

const LadoCarta& Carta::getLadoActivo() const {
    return estaVolteada ? ladoDorso : ladoFrente;
}

void Carta::voltear() {
    estaVolteada = !estaVolteada;
}

bool Carta::getEstaVolteada() const {
    return estaVolteada;
}

std::string Carta::getRutaImagen() const {
    const LadoCarta& activo = getLadoActivo();


    /* Si el color es de los oscuros Carpeta FLIP
       Si es Carpeta NORMAL
    */
    std::string carpetaBase = "NORMAL";

    Color c = activo.getColor();
    if (c == Color::Rosa || c == Color::Naranja ||
        c == Color::Turquesa || c == Color::Violeta) {
        carpetaBase = "FLIP";
    }

    // Construcción: :/assets/[MODO]/[COLOR]/[TIPO].png
    return ":/assets/" + carpetaBase + "/" +
           activo.getNombreCarpetaColor() + "/" +
           activo.getNombreArchivo() + ".png";
}

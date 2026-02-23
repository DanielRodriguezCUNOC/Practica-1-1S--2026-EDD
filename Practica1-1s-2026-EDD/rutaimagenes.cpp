#include "rutaimagenes.h"

RutaImagenes::RutaImagenes() {}

std::string RutaImagenes::generarRuta(TipoCarta tipo, Color color, int numero){

    std::string modo = modoDeJuego(color);

    // Reutilizar números claros desde NORMAL
    if(tipo == TipoCarta::NUMERO && esColorClaro(color)){
        modo = "NORMAL";
    }

    std::string carpetaColor = obtenerCarpetaColor(color);
    std::string archivo = obtenerNombreArchivo(tipo, color, numero, modo);

    return ":/assets/" + modo + "/" + carpetaColor + "/" + archivo;
}

bool RutaImagenes::esColorClaro(Color c){
    return (c == Color::ROJO || c == Color::AZUL ||
            c == Color::VERDE || c == Color::AMARILLO);
}

std::string RutaImagenes::modoDeJuego(Color c){
    if(c == Color::ROSA || c == Color::TURQUESA ||
        c == Color::NARANJA || c == Color::PURPURA)
        return "FLIP";

    return "NORMAL";
}

std::string RutaImagenes::obtenerCarpetaColor(Color c){
    switch(c){
    case Color::ROJO: return "ROJO";
    case Color::AZUL: return "AZUL";
    case Color::VERDE: return "VERDE";
    case Color::AMARILLO: return "AMARILLO";

    case Color::ROSA: return "ROSADO";
    case Color::TURQUESA: return "TURQUESA";
    case Color::NARANJA: return "NARANJA";
    case Color::PURPURA: return "PURPURA";

    case Color::NEGRO: return "COMODINES";
    default: return "VARIOS";
    }
}

std::string RutaImagenes::obtenerPrefijoColor(Color c){
    switch(c){
    case Color::ROJO: return "R";
    case Color::AZUL: return "AZ";
    case Color::VERDE: return "V";
    case Color::AMARILLO: return "A";

    case Color::NARANJA: return "N";
    case Color::PURPURA: return "PU";
    case Color::ROSA: return "P";
    case Color::TURQUESA: return "T";

    default: return "";
    }
}

std::string RutaImagenes::obtenerNombreArchivo(TipoCarta tipo, Color color, int numero, const std::string& modo){

    // -------- CARTAS NUMERO --------
    if(tipo == TipoCarta::NUMERO){
        return obtenerPrefijoColor(color) + std::to_string(numero);
    }

    // -------- NORMAL --------
    if(modo == "NORMAL"){
        switch(tipo){
        case TipoCarta::ROBA2:   return obtenerPrefijoColor(color) + "+2";
        case TipoCarta::BLOQUEO:   return obtenerPrefijoColor(color) + "B";
        case TipoCarta::REVERSE: return obtenerPrefijoColor(color) + "R";

        case TipoCarta::COMODIN4: return "+4";
        case TipoCarta::COMODIN:  return "Comodin-Color";

        case TipoCarta::CAMBIARMANO:  return "Comodin-Descartar-Cartas";
        case TipoCarta::ADIVINARCARTA:return "Comodin-Adivinar-Numero-y-Color";

        case TipoCarta::DORSOGENERICO:return "POSTERIOR";

        default: break;
        }
    }

    // -------- FLIP --------
    if(modo == "FLIP"){
        switch(tipo){
        case TipoCarta::ROBA1: return obtenerPrefijoColor(color) + "+1";
        case TipoCarta::ROBA3: return obtenerPrefijoColor(color) + "+3";

        case TipoCarta::SALTODOS: return obtenerPrefijoColor(color) + "PT";
        case TipoCarta::CAMBIARDIRECCION: return obtenerPrefijoColor(color) + "D";
        case TipoCarta::FLIP: return obtenerPrefijoColor(color) + "F";

        case TipoCarta::COMODIN6: return "Comodin+6";
        case TipoCarta::COMODIN2: return "Comodin+2";
        case TipoCarta::COLORETERNO: return "Comodin-Cartas-Inifinitas-Flip";

        case TipoCarta::ADIVINARCARTA:
            return "Comodin-Adivinar-Numero-y-Color";

        case TipoCarta::CAMBIARMANO:
            return "Comodin-Descartar-Cartas";

        default: break;
        }
    }

    return "DESCONOCIDO";
}

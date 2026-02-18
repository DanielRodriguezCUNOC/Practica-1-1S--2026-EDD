#include "rutaimagenes.h"

RutaImagenes::RutaImagenes() {}

std::string RutaImagenes::generarRuta(TipoCarta tipo, Color color, int numero){

    std::string modo = modoDeJuego(color);

    // Reutilizar números claros desde NORMAL
    if(tipo == TipoCarta::Numero && esColorClaro(color)){
        modo = "NORMAL";
    }

    std::string carpetaColor = obtenerCarpetaColor(color);
    std::string archivo = obtenerNombreArchivo(tipo, color, numero, modo);

    return ":/assets/" + modo + "/" + carpetaColor + "/" + archivo;
}

bool RutaImagenes::esColorClaro(Color c){
    return (c == Color::Rojo || c == Color::Azul ||
            c == Color::Verde || c == Color::Amarillo);
}

std::string RutaImagenes::modoDeJuego(Color c){
    if(c == Color::Rosa || c == Color::Turquesa ||
        c == Color::Naranja || c == Color::Purpura)
        return "FLIP";

    return "NORMAL";
}

std::string RutaImagenes::obtenerCarpetaColor(Color c){
    switch(c){
    case Color::Rojo: return "ROJO";
    case Color::Azul: return "AZUL";
    case Color::Verde: return "VERDE";
    case Color::Amarillo: return "AMARILLO";

    case Color::Rosa: return "ROSADO";
    case Color::Turquesa: return "TURQUESA";
    case Color::Naranja: return "NARANJA";
    case Color::Purpura: return "PURPURA";

    case Color::Negro: return "COMODINES";
    default: return "VARIOS";
    }
}

std::string RutaImagenes::obtenerPrefijoColor(Color c){
    switch(c){
    case Color::Rojo: return "R";
    case Color::Azul: return "AZ";
    case Color::Verde: return "V";
    case Color::Amarillo: return "A";

    case Color::Naranja: return "N";
    case Color::Purpura: return "PU";
    case Color::Rosa: return "P";
    case Color::Turquesa: return "T";

    default: return "";
    }
}

std::string RutaImagenes::obtenerNombreArchivo(TipoCarta tipo, Color color, int numero, const std::string& modo){

    // -------- CARTAS NUMERO --------
    if(tipo == TipoCarta::Numero){
        return obtenerPrefijoColor(color) + std::to_string(numero);
    }

    // -------- NORMAL --------
    if(modo == "NORMAL"){
        switch(tipo){
        case TipoCarta::Roba2:   return obtenerPrefijoColor(color) + "+2";
        case TipoCarta::Salto:   return obtenerPrefijoColor(color) + "B";
        case TipoCarta::Reverse: return obtenerPrefijoColor(color) + "R";

        case TipoCarta::Comodin4: return "+4";
        case TipoCarta::Comodin:  return "Comodin-Color";

        case TipoCarta::CambiarMano:  return "Comodin-Descartar-Cartas";
        case TipoCarta::AdivinarCarta:return "Comodin-Adivinar-Numero-y-Color";

        case TipoCarta::DorsoGenerico:return "POSTERIOR";

        default: break;
        }
    }

    // -------- FLIP --------
    if(modo == "FLIP"){
        switch(tipo){
        case TipoCarta::Roba1: return obtenerPrefijoColor(color) + "+1";
        case TipoCarta::Roba3: return obtenerPrefijoColor(color) + "+3";

        case TipoCarta::SaltoTodos: return obtenerPrefijoColor(color) + "PT";
        case TipoCarta::CambiarDireccion: return obtenerPrefijoColor(color) + "D";
        case TipoCarta::Flip: return obtenerPrefijoColor(color) + "F";

        case TipoCarta::Comodin6: return "Comodin+6";
        case TipoCarta::Comodin2: return "Comodin+2";
        case TipoCarta::ColorEterno: return "Comodin-Cartas-Inifinitas-Flip";

        case TipoCarta::AdivinarCarta:
            return "Comodin-Adivinar-Numero-y-Color";

        case TipoCarta::CambiarMano:
            return "Comodin-Descartar-Cartas";

        default: break;
        }
    }

    return "DESCONOCIDO";
}

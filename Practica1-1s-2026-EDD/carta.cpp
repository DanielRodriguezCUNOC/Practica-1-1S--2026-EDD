#include "carta.h"

Carta::Carta(LadoCarta* claro):
    ladoClaro(claro), ladoOscuro(nullptr), estaVolteada(false){}

Carta::Carta(LadoCarta* claro, LadoCarta* oscuro):
    ladoClaro(claro), ladoOscuro(oscuro), estaVolteada(false){}
Carta::Carta(const Carta& copia){
    this->ladoClaro = (copia.ladoClaro != nullptr) ? copia.ladoClaro->clonar() : nullptr;
    this->ladoOscuro = (copia.ladoOscuro != nullptr) ? copia.ladoOscuro->clonar() : nullptr;
    this->estaVolteada = copia.estaVolteada;
}

Carta& Carta::operator=(const Carta& copia) {
    if (this != &copia) {
        delete ladoClaro;
        delete ladoOscuro;
        this->ladoClaro = (copia.ladoClaro != nullptr) ? copia.ladoClaro->clonar() : nullptr;
        this->ladoOscuro = (copia.ladoOscuro != nullptr) ? copia.ladoOscuro->clonar() : nullptr;
        this->estaVolteada = copia.estaVolteada;
    }
    return *this;
}

Carta::~Carta(){
    delete ladoClaro;
    if(ladoOscuro!= nullptr) delete ladoOscuro;
}

LadoCarta* Carta::getLadoActivo() const{
    if(estaVolteada && ladoOscuro!=nullptr) return ladoOscuro;
    return ladoClaro;
}

bool Carta::esFlip() const{
    return ladoOscuro!=nullptr;
}


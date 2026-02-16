#include "cartaflip.h"

CartaFlip::CartaFlip(LadoCarta claro, LadoCarta oscuro):
    ladoClaro(claro), ladoOscuro(oscuro), estaVolteada(false)
{}

const LadoCarta& CartaFlip:: getLadoActivo()const{
    return estaVolteada ? ladoOscuro : ladoClaro;
}

void CartaFlip::voltear(){
    estaVolteada = !estaVolteada;
}

bool CartaFlip::esFlip()const{
    return true;
}
